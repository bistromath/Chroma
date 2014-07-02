/*
 * spd.hpp
 * Spectral Power Distribution class.
 *
 * Copyright 2014 Nick Foster
 *
 * This class implements a container for Spectral Power Distribution data.
 * Fundamentally, this is a vector of pairs of {wavelength, power}.
 * Internally this is represented as a pair of vectors to make things quicker.
 *
 * Let's talk about reshaping. There are cases where you really want the ability
 * to reshape on the fly -- this makes doing things like illuminating or getting
 * luminosity much easier. There are downsides:
 *
 * * Computational cost goes up -- linear interp is not fast.
 *
 * ...that might be it. As long as we provide the ability to reshape an SPD manually,
 * we're ok -- the user can force a reshape to ensure that reshaping isn't done every time.
*/

#pragma once
#include <Chroma/Config.hpp>
#include <vector>
#include <string>
#include <Chroma/Types/XYZ.hpp>

namespace Chroma {

class CHROMA_API spd {
public:
    spd();
    /* Create an SPD from a pair of lists {wavelengths, powers} */
    spd(const std::vector<float> &wavelengths, const std::vector<float> &powers);

    /* Copy constructor */
    spd(const spd &other);
    /* Move constructor */
    spd(spd &&other);

    /* Copy assignment operator */
    spd& operator=(const spd &other);
    spd& operator=(spd &&other);

    /* Type conversions */
    Chroma::XYZ XYZ() const;
    float lumens() const;
    float sum() const;

    /* Getters */
    const std::vector<float> &wavelengths() const;
    const std::vector<float> &powers() const;

    /* Return a new SPD reshaped to the desired wavelength vector */
    spd reshape(const std::vector<float> &new_wavelengths) const;

    /* Return a new SPD normalized to max power 1 */
    spd normalize() const;

    /* Return a new SPD normalized to desired lumenosity */
    spd normalize_lumenosity(float lumens) const;

private:
//    std::vector<std::pair<float, float> > data;
    std::vector<float> _wavelengths;
    std::vector<float> _powers;
};

/* Math operations */
Chroma::spd spd_arithmetic(const Chroma::spd &lhs, const Chroma::spd &rhs, std::string operation);
bool CHROMA_API operator==(const Chroma::spd &lhs, const Chroma::spd &rhs);
bool CHROMA_API operator!=(const Chroma::spd &lhs, const Chroma::spd &rhs);
Chroma::spd CHROMA_API operator+(const Chroma::spd &lhs, const Chroma::spd &rhs);
Chroma::spd CHROMA_API operator-(const Chroma::spd &lhs, const Chroma::spd &rhs);
Chroma::spd CHROMA_API operator*(const Chroma::spd &lhs, const Chroma::spd &rhs);
Chroma::spd CHROMA_API operator/(const Chroma::spd &lhs, const Chroma::spd &rhs);
Chroma::spd CHROMA_API operator*(const Chroma::spd &lhs, float rhs);
Chroma::spd CHROMA_API operator*(float lhs, const Chroma::spd &rhs);
Chroma::spd CHROMA_API operator/(const Chroma::spd &lhs, float rhs);

} //namespace Chroma
