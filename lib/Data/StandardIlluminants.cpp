#include <Chroma/Chroma.hpp>
#include <Chroma/Data/StandardIlluminants.hpp>
#include <stdexcept>
#include <algorithm>
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
        xd = 0.244063 + 0.09911e3/cct + 2.9678e6/(cct*cct) - 4.6070e9/(cct*cct*cct);
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

Chroma::spd E_illuminant(void)
{
    return {{350,850},{1,1}};
}

Chroma::spd blackbody(float temp, std::vector<float> wavelengths)
{
    const float h = 6.626070e-34;   /* Planck's constant */
    const float c = 299792458;      /* speed of light */
    const float k = 1.380648e-23;   /* Boltzmann's constant */

    auto I = [&h, &c, &k, &temp](float wavelength)
    {
        float nm = wavelength * 1e-9;
        return ((2*M_PI*h*(c*c))/(powf(nm,5)))*(1.0/(expf((h*c/nm)/(k*temp))-1.0));
    };
    std::vector<float> powers(wavelengths.size());
    std::transform(wavelengths.begin(), wavelengths.end(), powers.begin(), I);
    return {wavelengths, powers};
}

Chroma::xyY blackbody_chromaticity(float temp)
{
    float u = (0.860117757 + 1.54118254e-4*temp + 1.28641212e-7*(temp*temp))
            / (1 + 8.42420235e-4*temp + 7.08145163e-7*(temp*temp));
    float v = (0.317398726 + 4.22806245e-5*temp + 4.20481691e-8*(temp*temp))
            / (1 - 2.89741816e-5*temp + 1.61456053e-7*(temp*temp));
    Chroma::XYZ xyz(Chroma::uv(u,v));
    return xyz.xyY();
}

} /* namespace Chroma */
