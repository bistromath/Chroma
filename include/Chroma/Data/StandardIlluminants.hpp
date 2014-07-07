// :::::::::::::::::::::::: LICENSE AND COPYRIGHT NOTICE :::::::::::::::::::::::
// Copyright (c) 2014 Nick Foster <bistromath@gmail.com>.  All rights reserved.
// 
// This file is part of Chroma.  Chroma is distributed under the GNU General
// Public License, Version 3.0.  You can read the full terms of use in the
// COPYING file, or online at http://www.gnu.org/licenses/gpl.html.
// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

#pragma once
#include <Chroma/Chroma.hpp>
#include <Chroma/Data/StandardIlluminantsData.hpp>

namespace Chroma {

Chroma::spd CHROMA_API A_illuminant(void);
Chroma::xyY CHROMA_API D_illuminant_chromaticity(float cct);
Chroma::spd CHROMA_API D_illuminant(float cct);
Chroma::spd CHROMA_API E_illuminant(void);
Chroma::spd CHROMA_API blackbody(float cct, std::vector<float> wavelengths);
Chroma::xyY CHROMA_API blackbody_chromaticity(float temp);

} /* namespace Chroma */
