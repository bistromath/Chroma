#pragma once
#include <Chroma/Config.hpp>
#include <Chroma/Types/Chromaticity.hpp>

namespace Chroma {

enum CHROMA_API CCT_method {
    CCT_METHOD_MCCAMY,
    CCT_METHOD_ITERATIVE
};

float CHROMA_API CCT(const Chroma::xyY &chrom, Chroma::CCT_method method=CCT_METHOD_MCCAMY, float tol=0.1);

} /* namespace Chroma */
