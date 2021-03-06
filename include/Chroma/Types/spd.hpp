// :::::::::::::::::::::::: LICENSE AND COPYRIGHT NOTICE :::::::::::::::::::::::
// Copyright (c) 2014 Nick Foster <bistromath@gmail.com>.  All rights reserved.
// 
// This file is part of Chroma.  Chroma is distributed under the GNU General
// Public License, Version 3.0.  You can read the full terms of use in the
// COPYING file, or online at http://www.gnu.org/licenses/gpl.html.
// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

/*
 * spd.hpp
 * Spectral Power Distribution class.
 *
 * This class implements a container for Spectral Power Distribution data.
 * Fundamentally, this is a vector of pairs of {wavelength, power}.
 * Internally this is represented as a pair of vectors to make things quicker.
*/

#pragma once
#include <Chroma/Config.hpp>
#include <vector>
#include <string>
#include <functional>

namespace Chroma {

class CHROMA_API spd {
public:
    spd();
    /* Create an SPD from a pair of lists {wavelengths, powers} */
    spd(const std::vector<float> &wavelengths, const std::vector<float> &powers);
    spd(float wavelength_start, float wavelength_stop, const std::vector<float> &powers);

    /* Copy constructor */
    spd(const spd &other);
    /* Move constructor */
    spd(spd &&other);

    /* Copy assignment operator */
    spd& operator=(const spd &other);
    spd& operator=(spd &&other);

    /* Unary operators */
    spd &operator+=(const spd &rhs);
    spd &operator-=(const spd &rhs);
    spd &operator*=(const spd &rhs);
    spd &operator/=(const spd &rhs);
    spd &operator*=(const float rhs);
    spd &operator/=(const float rhs);

    /* Type conversions */
//    Chroma::XYZ XYZ() const; /* TODO deprecate */
    float lumens() const;
    float sum() const;

    /* Getters */
    std::vector<float> wavelengths() const;
    const std::vector<float> &powers() const;

    float wavelength_start() const;
    float wavelength_stop() const;
    float wavelength_step() const;

    /* Return a new SPD reshaped to the desired wavelength vector */
    spd reshape(float wavelength_start, float wavelength_stop, float wavelength_step) const;

    /* Return a new SPD normalized to max power 1 */
    spd normalize() const;

    /* Return a new SPD normalized to desired luminosity */
    spd normalize_luminosity(float lumens) const;

private:
    float _wavelength_start;
    float _wavelength_stop;
    float _wavelength_step;

    std::vector<float> _powers;
};

/* Math operations */
Chroma::spd spd_arithmetic(const Chroma::spd &lhs, const Chroma::spd &rhs, std::function<float(const float, const float)> operation);
bool CHROMA_API operator==(const Chroma::spd &lhs, const Chroma::spd &rhs);
bool CHROMA_API operator!=(const Chroma::spd &lhs, const Chroma::spd &rhs);
Chroma::spd CHROMA_API operator+(Chroma::spd lhs, const Chroma::spd &rhs);
Chroma::spd CHROMA_API operator+(const Chroma::spd &lhs, Chroma::spd &&rhs);
Chroma::spd CHROMA_API operator-(Chroma::spd lhs, const Chroma::spd &rhs);
Chroma::spd CHROMA_API operator*(Chroma::spd lhs, const Chroma::spd &rhs);
Chroma::spd CHROMA_API operator*(const Chroma::spd &lhs, Chroma::spd &&rhs);
Chroma::spd CHROMA_API operator/(Chroma::spd lhs, const Chroma::spd &rhs);
Chroma::spd CHROMA_API operator*(Chroma::spd lhs, float rhs);
Chroma::spd CHROMA_API operator/(Chroma::spd lhs, float rhs);

} //namespace Chroma
