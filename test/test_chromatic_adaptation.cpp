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

#include <Chroma/Chroma.hpp>
#include <Chroma/Data/StandardIlluminants.hpp>
#include <Chroma/Analysis/ChromaticAdaptation.hpp>

BOOST_AUTO_TEST_CASE(test_chromatic_adaptation)
{
    Chroma::XYZ a_illum(1.0985, 1.00, 0.3558);
    Chroma::XYZ c_illum(0.98074, 1.0000, 1.18232);
    Chroma::XYZ ccred(0.315756, 0.162732, 0.015905);
    Chroma::XYZ result = Chroma::chromatic_adaptation(ccred, a_illum, c_illum, Chroma::BradfordAdaptation);
    std::cout << "X: " << result.X << " Y: " << result.Y << " Z: " << result.Z << std::endl;
    float error = Chroma::uv(result) - Chroma::uv(c_illum);
    std::cout << "Error: " << error << std::endl;
    BOOST_CHECK(error < 0.25);
}
