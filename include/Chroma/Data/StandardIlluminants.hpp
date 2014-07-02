#pragma once
#include <Chroma/Chroma.hpp>
#include <Chroma/Data/StandardIlluminantsData.hpp>

namespace Chroma {

Chroma::xyY CHROMA_API D_illuminant_chromaticity(float cct);
Chroma::spd CHROMA_API D_illuminant(float cct);

} /* namespace Chroma */
