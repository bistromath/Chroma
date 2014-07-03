#pragma once
#include <Chroma/Chroma.hpp>
#include <Chroma/Data/StandardIlluminantsData.hpp>

namespace Chroma {

Chroma::xyY CHROMA_API D_illuminant_chromaticity(float cct);
Chroma::spd CHROMA_API D_illuminant(float cct);
Chroma::spd CHROMA_API E_illuminant(void);
Chroma::spd CHROMA_API blackbody(float cct, std::vector<float> wavelengths);
Chroma::xyY CHROMA_API blackbody_chromaticity(float temp);

} /* namespace Chroma */
