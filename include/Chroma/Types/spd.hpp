/*
 * spd.hpp
 * Spectral Power Distribution class.
 *
 * Copyright 2014 Nick Foster
 *
 * This class implements a container for Spectral Power Distribution data.
 * Fundamentally, this is a vector of pairs of {wavelength, power}.
 * Internally this is represented as a pair of vectors to make things quicker.
*/

#pragma once
#include <Chroma/Config.hpp>
#include <vector>
#include <string>
#include <Chroma/Types/XYZ.hpp>
#include <Chroma/Data/PhotopicEfficiency.hpp>
#include <Chroma/Data/StandardObservers.hpp>

namespace Chroma {

class CHROMA_API spd {
public:
    /* Create an SPD from a pair of lists {wavelengths, powers} */
    spd(const std::vector<float> &wavelengths, const std::vector<float> &powers);

    /* Copy constructor */
//    spd(spd &other);

    /* Math operations */
    bool operator==(const spd &other);
    bool operator!=(const spd &other);
    spd operator+(const spd &other);
    spd operator-(const spd &other);
    spd operator*(const spd &other);
    spd operator/(const spd &other);
    spd operator*(const float other);
    spd operator/(const float other);

    /* Type conversions */
    //XYZ XYZ();
    float lumens();

    /* Getters */
    const std::vector<float> wavelengths() const;
    const std::vector<float> powers() const;

    /* Return a new SPD reshaped to the desired wavelength vector */
    spd reshape(std::vector<float> new_wavelengths);

    /* Return a new SPD normalized to max power 1 */
    spd normalize();

    /* Return a new SPD normalized to desired lumenosity */
    spd normalize_lumenosity(float lumens);

private:
//    std::vector<std::pair<float, float> > data;
    const std::vector<float> _wavelengths;
    const std::vector<float> _powers;

    spd arithmetic(const spd &other, std::string operation);
};

} //namespace Chroma
