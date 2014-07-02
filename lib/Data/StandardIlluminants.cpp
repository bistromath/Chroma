#include <Chroma/Chroma.hpp>
#include <Chroma/Data/StandardIlluminants.hpp>
#include <stdexcept>
#include <cmath>

namespace Chroma {

Chroma::xyY D_illuminant_chromaticity(float cct)
{
    if(cct < 4000 or cct > 25000)
    {
        throw std::runtime_error("D illuminant CCT must be between 4000K and 25000K");
    }

    float xd;
    if(cct <= 7000)
    {
        xd = 0.244603 + 0.09911e3/cct + 2.9678e6/(cct*cct) - 4.6070e9/(cct*cct*cct);
    }
    else
    {
        xd = 0.237040 + 0.24748e3/cct + 1.9018e6/(cct*cct) - 2.0064e9/(cct*cct*cct);
    }
    float yd = -3.000*(xd*xd) + 2.870*xd - 0.275;

    return Chroma::xyY(xd, yd);
}

Chroma::spd D_illuminant(float cct)
{
    Chroma::xyY chrom = D_illuminant_chromaticity(cct);
    float M = 0.0241 + 0.2562*chrom.x - 0.7341*chrom.y;
    float M1 = (-1.3515 - 1.7703*chrom.x + 5.9114*chrom.y) / M;
    float M2 = (0.0300 - 31.4424*chrom.x + 30.0717*chrom.y) / M;

    return D_illuminant_S0 + D_illuminant_S1*M1 + D_illuminant_S2*M2;
}

} /* namespace Chroma */
