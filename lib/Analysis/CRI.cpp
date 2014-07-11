#include <Chroma/Chroma.hpp>
#include <Chroma/Data/StandardIlluminants.hpp>
#include <Chroma/Data/ColorRenderingIndexTiles.hpp>
#include <Chroma/Analysis/ChromaticAdaptation.hpp>
#include <Chroma/Analysis/CRI.hpp>
#include <algorithm>
#include <iostream>

float Chroma::CRI(Chroma::spd spec)
{
    /* Normalize luminance of SPD to Y=100 */
    Chroma::XYZ spec_xyz(spec);
    spec *= (100./spec_xyz.Y);
    spec_xyz *= (100./spec_xyz.Y);

    float cct = Chroma::CCT(spec_xyz); /* McCamy estimation good to ~10K */
    std::cout << "CCT: " << cct << std::endl;
    Chroma::spd ref_spd;

    if(cct < 5000)
    {
        ref_spd = Chroma::blackbody(cct, spec.wavelengths());
    } else
    {
        ref_spd = Chroma::D_illuminant(cct);
    }

    Chroma::XYZ ref_xyz(ref_spd);
    ref_spd *= (100./ref_xyz.Y);
    ref_xyz *= (100./ref_xyz.Y);

    std::cout << "Test X: " << spec_xyz.X << " Y: " << spec_xyz.Y << " Z: " << spec_xyz.Z << std::endl;

    /* TODO: you could probably create this list in the data header */
    std::vector<Chroma::spd> TCS = {Chroma::CRI_TCS01, Chroma::CRI_TCS02, Chroma::CRI_TCS03, Chroma::CRI_TCS04,
                                    Chroma::CRI_TCS05, Chroma::CRI_TCS06, Chroma::CRI_TCS07, Chroma::CRI_TCS08};

    /* Calculate a chromatic adaptation transform matrix to use in the loop below */
    auto transform = Chroma::calculate_chromatic_adaptation_transform(spec_xyz, ref_xyz, Chroma::VonKriesTransform);

    float duv = Chroma::uv(ref_xyz) - Chroma::uv(spec_xyz);
    std::cout << "Ref uv: " << Chroma::uv(ref_xyz).u << ", " << Chroma::uv(ref_xyz).v << std::endl;
    std::cout << "Test uv: " << Chroma::uv(spec_xyz).u << ", " << Chroma::uv(spec_xyz).v << std::endl;
    if(duv > 5.4e-3)
    {
        std::cerr << "Warning: duv too high for meaningful CRI result (" << duv << ")." << std::endl;
    }

    std::vector<float> cri_results(8);
    for( size_t index=0; index<TCS.size(); ++index )
    {
        /* illuminate the tile with both the reference source and the source under test */
        Chroma::XYZ ref_illum(ref_spd * TCS[index]);
        Chroma::XYZ test_illum(spec * TCS[index]);
        /* Chromatically adapt the test result to the reference illuminant */
        Chroma::XYZ test_illum_chrom = Chroma::apply_chromatic_adaptation_transform(test_illum, transform);
        /* Get the CIE1964 delta-E distance between the two */
        Chroma::UVW ref_illum_UVW(ref_illum, ref_xyz);
        Chroma::UVW test_illum_UVW(test_illum_chrom, ref_xyz);

        std::cout << "R" << index+1 << " Test U: " << Chroma::UVW(test_illum, ref_xyz).U << " V: " << Chroma::UVW(test_illum, ref_xyz).V << " W: " << Chroma::UVW(test_illum, ref_xyz).W << std::endl;
        std::cout << "R" << index+1 << " Test_chrom U: " << test_illum_UVW.U << " V: " << test_illum_UVW.V << " W: " << test_illum_UVW.W << std::endl;
        std::cout << "R" << index+1 << " ref_chrom U: " << ref_illum_UVW.U << " V: " << ref_illum_UVW.V << " W: " << ref_illum_UVW.W << std::endl;
        float dE = test_illum_UVW - ref_illum_UVW;
        float Ri = 100-4.6*dE;
        cri_results[index] = Ri;
        std::cout << "R" << index+1 << ": " << Ri << std::endl;
    }

    float Ra = std::accumulate(cri_results.begin(), cri_results.end(), 0.0) / cri_results.size();

    return Ra;
}
