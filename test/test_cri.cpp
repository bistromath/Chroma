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
#include <Chroma/Analysis/CRI.hpp>

BOOST_AUTO_TEST_CASE(test_cri)
{
    float cri = Chroma::CRI(Chroma::F4_illuminant);
    BOOST_CHECK_CLOSE(cri, 51, 1);
    cri = Chroma::CRI(Chroma::D_illuminant(6500));
    BOOST_CHECK_CLOSE(cri, 100, 0.1);
    cri = Chroma::CRI(Chroma::F5_illuminant);
    BOOST_CHECK_CLOSE(cri, 72, 1);
}
