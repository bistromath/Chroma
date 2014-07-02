#include <boost/test/unit_test.hpp>
#include <stdexcept>
#include <iostream>

#include <Chroma/Types/spd.hpp>
#include <Chroma/Data/StandardIlluminants.hpp>

BOOST_AUTO_TEST_CASE(test_d_illum)
{
    Chroma::xyY d50_chromaticity = Chroma::D_illuminant_chromaticity(5000);
    BOOST_CHECK_CLOSE(d50_chromaticity.x, 0.34629, 1e-3); /* both checked against colorimetry.py */
    BOOST_CHECK_CLOSE(d50_chromaticity.y, 0.35898, 1e-3);

    Chroma::xyY d80_chromaticity = Chroma::D_illuminant_chromaticity(8000);
    BOOST_CHECK_CLOSE(d80_chromaticity.x, 0.29376, 1e-3);
    BOOST_CHECK_CLOSE(d80_chromaticity.y, 0.30911, 1e-3);

    Chroma::spd d50 = Chroma::D_illuminant(5000);
    Chroma::XYZ d50XYZ = d50.XYZ();
    BOOST_CHECK_CLOSE(d50XYZ.x(), d50_chromaticity.x, 1e-3);
    BOOST_CHECK_CLOSE(d50XYZ.y(), d50_chromaticity.y, 1e-3);
}

BOOST_AUTO_TEST_CASE(test_e_illum)
{
    Chroma::spd E = Chroma::spd({300,900}, {1, 1});
    Chroma::XYZ E_chrom = E.XYZ();
    BOOST_CHECK_CLOSE(E_chrom.x(), 1.0/3.0, 1e-3);
    BOOST_CHECK_CLOSE(E_chrom.y(), 1.0/3.0, 1e-3);
}
