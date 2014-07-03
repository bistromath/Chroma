#include <boost/test/unit_test.hpp>
#include <stdexcept>
#include <iostream>

#include <Chroma/Types/spd.hpp>
#include <Chroma/Data/StandardIlluminants.hpp>

BOOST_AUTO_TEST_CASE(test_d_illum)
{
    Chroma::xyY d50_chromaticity = Chroma::D_illuminant_chromaticity(5000);
    BOOST_CHECK_CLOSE(d50_chromaticity.x, 0.34575, 1e-1);
    BOOST_CHECK_CLOSE(d50_chromaticity.y, 0.35856, 1e-1);

    Chroma::xyY d80_chromaticity = Chroma::D_illuminant_chromaticity(8000);
    BOOST_CHECK_CLOSE(d80_chromaticity.x, 0.29376, 1e-1);
    BOOST_CHECK_CLOSE(d80_chromaticity.y, 0.30911, 1e-1);

    Chroma::spd d50 = Chroma::D_illuminant(5000);
    Chroma::XYZ d50XYZ = d50.XYZ();
    BOOST_CHECK_CLOSE(d50XYZ.x(), d50_chromaticity.x, 1e-1);
    BOOST_CHECK_CLOSE(d50XYZ.y(), d50_chromaticity.y, 1e-1);
}

BOOST_AUTO_TEST_CASE(test_e_illum)
{
    Chroma::XYZ E_chrom = Chroma::E_illuminant().XYZ();
    BOOST_CHECK_CLOSE(E_chrom.x(), 1.0/3.0, 1e-1);
    BOOST_CHECK_CLOSE(E_chrom.y(), 1.0/3.0, 1e-1);
}

BOOST_AUTO_TEST_CASE(test_blackbody)
{
    std::vector<float> wavelengths = Chroma::F4_illuminant.wavelengths();
    Chroma::spd bb50 = Chroma::blackbody(5000, wavelengths);
    Chroma::xyY bb50_chroma = bb50.XYZ().xyY();
    BOOST_CHECK_CLOSE(bb50_chroma.x, 0.34520, 1e-1);
    BOOST_CHECK_CLOSE(bb50_chroma.y, 0.35169, 1e-1);
}
