// :::::::::::::::::::::::: LICENSE AND COPYRIGHT NOTICE :::::::::::::::::::::::
// Copyright (c) 2014 Nick Foster <bistromath@gmail.com>.  All rights reserved.
// 
// This file is part of Chroma.  Chroma is distributed under the GNU General
// Public License, Version 3.0.  You can read the full terms of use in the
// COPYING file, or online at http://www.gnu.org/licenses/gpl.html.
// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

#include <boost/test/unit_test.hpp>
#include <stdexcept>
#include <iostream>

#include <Chroma/Types/spd.hpp>
#include <Chroma/Data/StandardIlluminants.hpp>

BOOST_AUTO_TEST_CASE(test_spd_construct)
{
    std::vector<float> w(100);
    std::vector<float> p(100);
    for(int i=0; i<100; i++) {
        w[i] = 350+5*i;
        p[i] = 0.1;
    }
    Chroma::spd spd1(w, p);
    BOOST_CHECK(spd1.wavelengths() == w);
    BOOST_CHECK(spd1.powers() == p);

    std::vector<float> w2(101);
    BOOST_CHECK_THROW(Chroma::spd(w2, p), std::runtime_error); //TODO require specific exception

    /* Test copy, copy assignment, move, and move assignment operators */
    std::cout << "Starting copy constructor" << std::endl;
    Chroma::spd spd2(spd1);
    std::cout << "Ending copy constructor" << std::endl;

    /* Test move assignment */
    std::cout << "Starting move assignment" << std::endl;
    spd1 = Chroma::spd({1,2,3,4,5}, {10,20,30,40,50});
    std::cout << "Ending move assignment" << std::endl;

    /* Test copy assignment */
    std::cout << "Starting copy assignment" << std::endl;
    spd1 = spd2;
    std::cout << "Ending copy assignment" << std::endl;
}

BOOST_AUTO_TEST_CASE(test_spd_equals)
{
    std::vector<float> w1(100);
    std::vector<float> w2(101);
    std::vector<float> p1(100);
    std::vector<float> p2(100);
    std::vector<float> p3(101);
    for(int i=0; i<100; i++) {
        w1[i] = 350+5*i;
        p1[i] = i;
        p2[i] = i+1;
    }
    for(int i=0; i<101; i++) {
        w2[i] = 350+5*i;
        p3[i] = i+2;
    }

    Chroma::spd spd1(w1, p1), spd2(w1, p2), spd3(w2, p3), spd4(w1, p1);
    BOOST_CHECK(spd1 == spd4);
    BOOST_CHECK(spd1 != spd2);
    BOOST_CHECK(spd1 != spd3);
    BOOST_CHECK(spd2 != spd3);
}

BOOST_AUTO_TEST_CASE(test_spd_math)
{
    std::vector<float> w1(100), w2(100), w3(200);
    std::vector<float> p1(100), p2(100), p3(200);

    for(int i=0; i<100; i++) {
        w1[i] = 350+5*i;
        p1[i] = i+1;
        w2[i] = 350+5*i;
        p2[i] = i+2;
    }
    for(int i=0; i<200; i++) {
        w3[i] = 350+2.5*i;
        p3[i] = i/2+1;
    }

    Chroma::spd spd1(w1, p1), spd2(w2, p2), spd3(w3, p3);
    auto spd1_p_2 = spd1+spd2;
    auto spd1_p_3 = spd1+spd3;
    auto spd1_m_2 = spd1-spd2;
    auto spd1_m_3 = spd1-spd3;
    auto spd1_t_2 = spd1*spd2;
    auto spd1_t_3 = spd1*spd3;
    auto spd1_d_2 = spd1/spd2;
    auto spd1_d_3 = spd1/spd3;

    std::vector<float> result(100), result2(200);

    for(size_t i=0; i<100; i++) {
        BOOST_CHECK(spd1_p_2.powers()[i] == spd1.powers()[i] + spd2.powers()[i]);
        BOOST_CHECK(spd1_m_2.powers()[i] == spd1.powers()[i] - spd2.powers()[i]);
        BOOST_CHECK(spd1_t_2.powers()[i] == spd1.powers()[i] * spd2.powers()[i]);
        BOOST_CHECK(spd1_d_2.powers()[i] == spd1.powers()[i] / spd2.powers()[i]);
    }

    BOOST_CHECK_EQUAL(spd1_p_3.wavelengths().size(), 200);
    BOOST_CHECK_EQUAL(spd1_m_3.wavelengths().size(), 200);
    BOOST_CHECK_EQUAL(spd1_t_3.wavelengths().size(), 200);
    BOOST_CHECK_EQUAL(spd1_d_3.wavelengths().size(), 200);

    for(size_t i=0; i<200; i+=2) {
        BOOST_CHECK(spd1_p_3.powers()[i] == spd1.powers()[i/2] + spd3.powers()[i]);
        BOOST_CHECK(spd1_m_3.powers()[i] == spd1.powers()[i/2] - spd3.powers()[i]);
        BOOST_CHECK(spd1_t_3.powers()[i] == spd1.powers()[i/2] * spd3.powers()[i]);
        BOOST_CHECK(spd1_d_3.powers()[i] == spd1.powers()[i/2] / spd3.powers()[i]);
    }
}

BOOST_AUTO_TEST_CASE(test_spd_lumenosity)
{
    const float lumenosity = Chroma::F4_illuminant.lumens();
    BOOST_CHECK_CLOSE(lumenosity, 199800.8, 1e-1); /* checked against colorimetry.py */
}

BOOST_AUTO_TEST_CASE(test_spd_xyz)
{
    const Chroma::XYZ result = Chroma::F4_illuminant.XYZ();
    std::cout << "X: " << result.X << " Y: " << result.Y << " Z: " << result.Z << std::endl;
    Chroma::xyY chromaticity(result);
    BOOST_CHECK_CLOSE(chromaticity.x, 0.44018, 1e-3); /* both checked against colorimetry.py */
    BOOST_CHECK_CLOSE(chromaticity.y, 0.40329, 1e-3);
}

