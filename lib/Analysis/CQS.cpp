#include <Chroma/Chroma.hpp>
#include <Chroma/Data/StandardIlluminants.hpp>
#include <Chroma/Data/ColorRenderingIndexTiles.hpp>
#include <Chroma/Analysis/ChromaticAdaptation.hpp>
#include <Chroma/Analysis/CQS.hpp>
#include <algorithm>
#include <iostream>

float Chroma::CQS(Chroma::spd spec)
{
    Chroma::XYZ spec_xyz(spec);
    float cct = Chroma::CCT(spec_xyz); /* McCamy estimation good to ~10K */
    Chroma::spd ref_spd;

    if(cct < 5000)
    {
        ref_spd = Chroma::blackbody(cct, spec.wavelengths());
    } else
    {
        ref_spd = Chroma::D_illuminant(cct);
    }

    Chroma::XYZ ref_xyz(ref_spd);

    /* TODO: you could probably create this list in the data header */
    std::vector<Chroma::spd> TCS = {Chroma::CQS_Q1, Chroma::CQS_Q2, Chroma::CQS_Q3, Chroma::CQS_Q4,
                                    Chroma::CQS_Q5, Chroma::CQS_Q6, Chroma::CQS_Q7, Chroma::CQS_Q8,
                                    Chroma::CQS_Q9, Chroma::CQS_Q10, Chroma::CQS_Q11, Chroma::CQS_Q12,
                                    Chroma::CQS_Q13, Chroma::CQS_Q14, Chroma::CQS_Q15};

    /* Calculate a chromatic adaptation transform matrix to use in the loop below */
    auto transform = Chroma::calculate_chromatic_adaptation_transform(spec_xyz, ref_xyz, Chroma::CIECAT02Transform);

    std::vector<float> cqs_results(15);
    for( size_t index=0; index<TCS.size(); ++index )
    {
        /* illuminate the tile with both the reference source and the source under test */
        Chroma::XYZ ref_illum(ref_spd * TCS[index]);
        Chroma::XYZ test_illum(spec * TCS[index]);
        /* Chromatically adapt the test result to the reference illuminant */
        Chroma::XYZ test_illum_chrom = Chroma::apply_chromatic_adaptation_transform(test_illum, transform);

        auto test_illum_Lab = Chroma::Lab(test_illum_chrom);
        auto ref_illum_Lab  = Chroma::Lab(ref_illum_chrom);

        float ref_cstar  = sqrtf(ref_illum_Lab.a*ref_illum_Lab.a + ref_illum_Lab.b*ref_illum_Lab.b);
        float test_cstar = sqrtf(test_illum_Lab.a*test_illum_Lab.a + test_illum_Lab.b*test_illum_Lab.b);
        float dcstar = test_cstar - ref_cstar;
        float deltaE  = test_illum_Lab - ref_illum_Lab; // - operator calculates Euclidean distance

        float dEprime = dE;
        if(dcstar < 0)
        {
            dEprime = sqrtf(deltaE*deltaE+dcstar*dcstar);
        }

        cqs_results[index] = dEprime;
    }

    /* square 'em all for RMS calc */
    for(size_t i=0; i<cqs_results.size(); i++)
    {
        cqs_results[i] *= cqs_results[i];
    }

    float dErms = sqrtf(std::accumulate(cqs_results.begin(), cqs_results.end(), 0.0));
    float Ra = std::accumulate(cri_results.begin(), cri_results.end(), 0.0) / cri_results.size();

    return Ra;
}
