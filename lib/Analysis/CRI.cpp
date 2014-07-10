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
    Chroma::spd ref_spd;
    std::cout << "CCT: " << cct << std::endl;

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

    /* This syntax is bullshit... */
    float duv = Chroma::uv(ref_xyz) - Chroma::uv(spec_xyz);
    if(duv > 5.4e-3)
    {
        std::cerr << "Warning: duv too high for meaningful CRI result (" << duv << ")." << std::endl;
    }

    /* TODO: you could probably create this list in the data header */
    std::vector<Chroma::spd> TCS = {Chroma::CRI_TCS01, Chroma::CRI_TCS02, Chroma::CRI_TCS03, Chroma::CRI_TCS04,
                                    Chroma::CRI_TCS05, Chroma::CRI_TCS06, Chroma::CRI_TCS07, Chroma::CRI_TCS08};

    auto transform = Chroma::calculate_chromatic_adaptation_transform(spec_xyz, ref_xyz, Chroma::VonKriesTransform);

    std::vector<float> cri_results(8);
    for( size_t index=0; index<TCS.size(); ++index )
    {
        /* illuminate the tile with both the reference source and the source under test */
        Chroma::XYZ ref_illum(ref_spd * TCS[index]);
        Chroma::XYZ test_illum(spec * TCS[index]);
        /* Chromatically adapt the test result to the reference illuminant */
        Chroma::XYZ test_illum_chrom = Chroma::apply_chromatic_adaptation_transform(test_illum, transform);
        /* Get the CIE1964 delta-E distance between the two */
        Chroma::UVW ref_uvw(ref_illum, test_illum);
        Chroma::UVW test_uvw(test_illum_chrom, test_illum);
        float dE = test_uvw - ref_uvw;
        float Ri = 100-4.6*dE;
        cri_results[index] = Ri;
    }

    float Ra = std::accumulate(cri_results.begin(), cri_results.end(), 0.0) / cri_results.size();

    return Ra;
}
