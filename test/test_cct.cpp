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

BOOST_AUTO_TEST_CASE(test_cct)
{
    Chroma::XYZ test_f4(Chroma::F4_illuminant.XYZ());
    Chroma::XYZ test_d65(Chroma::D_illuminant(6504).XYZ());
    BOOST_CHECK_CLOSE(Chroma::CCT(test_f4, Chroma::CCT_METHOD_ITERATIVE), 2940, 0.1);
    BOOST_CHECK_CLOSE(Chroma::CCT(test_d65, Chroma::CCT_METHOD_ITERATIVE), 6504, 0.1);
}
