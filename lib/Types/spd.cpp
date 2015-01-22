// :::::::::::::::::::::::: LICENSE AND COPYRIGHT NOTICE :::::::::::::::::::::::
// Copyright (c) 2014 Nick Foster <bistromath@gmail.com>.  All rights reserved.
// 
// This file is part of Chroma.  Chroma is distributed under the GNU General
// Public License, Version 3.0.  You can read the full terms of use in the
// COPYING file, or online at http://www.gnu.org/licenses/gpl.html.
// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

/*
 * SPD type implementation.
 *
 * Nick Foster, 2014
 */

#include <Chroma/Types/spd.hpp>
#include <Chroma/Data/PhotopicEfficiency.hpp>
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <functional>

Chroma::spd::spd()
{}

Chroma::spd::spd(const std::vector<float> &wavelengths, const std::vector<float> &powers)
{
    if(wavelengths.size() != powers.size()) throw std::runtime_error("Wavelength and power vectors must be the same length");

    /* Check to see that wavelength array is monotonic and equally spaced.
     * We require this to make linear interpolation for reshaping much (much!) faster.
     * Besides, any spectrophotometric data you're likely to use meets these criteria.
     */
    float wave_spacing = wavelengths[1] - wavelengths[0];
    for(size_t i=1; i<wavelengths.size(); i++)
    {
        if(wavelengths[i]-wavelengths[i-1] != wave_spacing)
        {
            throw std::runtime_error("Wavelength array must be equally spaced.");
        }
        if(wavelengths[i] <= wavelengths[i-1])
        {
            throw std::runtime_error("Wavelength array must be monotonically increasing.");
        }
    }
    _wavelength_start = wavelengths[0];
    _wavelength_stop = wavelengths[wavelengths.size()-1];
    _wavelength_step = wave_spacing;
    _powers = powers;
}

Chroma::spd::spd(float wavelength_start, float wavelength_stop, const std::vector<float> &powers)
    : _wavelength_start(wavelength_start),
      _wavelength_stop(wavelength_stop),
      _wavelength_step((wavelength_stop-wavelength_start) / (powers.size()-1)),
      _powers(powers)
{
}

Chroma::spd::spd(const Chroma::spd &other)
    : _wavelength_start(other.wavelength_start()),
      _wavelength_stop(other.wavelength_stop()),
      _wavelength_step(other.wavelength_step()),
      _powers(other.powers())
{
}

/* TODO: find out if STL will automatically move rvalues in the copy constructor */
Chroma::spd::spd(Chroma::spd &&other)
    : _wavelength_start(std::move(other.wavelength_start())),
      _wavelength_stop(std::move(other.wavelength_stop())),
      _wavelength_step(std::move(other.wavelength_step())),
      _powers(std::move(other.powers()))
{
}

Chroma::spd& Chroma::spd::operator=(const spd &other)
{
    _wavelength_start = other.wavelength_start();
    _wavelength_stop = other.wavelength_stop();
    _wavelength_step = other.wavelength_step();
    _powers = other.powers();
    return *this;
}

Chroma::spd& Chroma::spd::operator=(spd &&other)
{
    _wavelength_start = std::move(other.wavelength_start());
    _wavelength_stop = std::move(other.wavelength_stop());
    _wavelength_step = std::move(other.wavelength_step());
    _powers = std::move(other.powers());
    return *this;
}

std::vector<float> Chroma::spd::wavelengths(void) const
{
    std::vector<float> wavelength_vec(_powers.size());
    for(size_t i=0; i<wavelength_vec.size(); i++) {
        wavelength_vec[i] = _wavelength_start + _wavelength_step*i;
    }
    return wavelength_vec;
}

const std::vector<float> &Chroma::spd::powers(void) const
{
    return _powers;
}

float Chroma::spd::wavelength_start(void) const
{
    return _wavelength_start;
}

float Chroma::spd::wavelength_step(void) const
{
    return _wavelength_step;
}

float Chroma::spd::wavelength_stop(void) const
{
    return _wavelength_stop;
}

bool Chroma::operator==(const Chroma::spd &lhs, const Chroma::spd &rhs)
{
    return (lhs.wavelength_start() == rhs.wavelength_start()
            and lhs.wavelength_step() == rhs.wavelength_step()
            and lhs.powers() == rhs.powers());
}

bool Chroma::operator!=(const Chroma::spd &lhs, const Chroma::spd &rhs)
{
    return not (lhs==rhs);
}

Chroma::spd Chroma::spd_arithmetic(const Chroma::spd &l, const Chroma::spd &r, std::function<float(const float, const float)> operation)
{
    /* Anything gross in here is an effort to avoid copy-construction */
    const Chroma::spd *lhs, *rhs;
    Chroma::spd temp;
    if(l.powers().size() < 2 or r.powers().size() < 2)
    {
        throw std::runtime_error("Attempting to do math on an empty SPD.");
    }
    if(   l.wavelength_step() != r.wavelength_step()
       or l.wavelength_start() != r.wavelength_start()
       or l.wavelength_stop() != r.wavelength_stop())
    {
        /* we're going to reshape to match the one with the best resolution. */
        if(l.wavelength_step() < r.wavelength_step())
        {
            lhs = &l;
            temp = r.reshape(l.wavelength_start(), l.wavelength_stop(), l.wavelength_step());
            rhs = const_cast <const Chroma::spd *>(&temp);
        }
        else
        {
            temp = l.reshape(r.wavelength_start(), r.wavelength_stop(), r.wavelength_step());
            lhs = const_cast <const Chroma::spd *>(&temp);
            rhs = &r;
        }
    }
    else
    {
        lhs = &l;
        rhs = &r;
    }
    std::vector<float> result(lhs->powers().size());
    for(size_t i=0; i<result.size(); i++)
    {
        result[i] = operation(lhs->powers()[i], rhs->powers()[i]);
    }
    return {lhs->wavelength_start(), lhs->wavelength_stop(), result};
}

Chroma::spd &Chroma::spd::operator+=(const Chroma::spd &rhs)
{
    Chroma::spd result = spd_arithmetic(*this, rhs, std::plus<float>());
    _wavelength_start = result.wavelength_start();
    _wavelength_stop = result.wavelength_stop();
    _wavelength_step = result.wavelength_step();
    _powers = result.powers();
    return *this;
}

Chroma::spd &Chroma::spd::operator-=(const Chroma::spd &rhs)
{
    Chroma::spd result = spd_arithmetic(*this, rhs, std::minus<float>());
    _wavelength_start = result.wavelength_start();
    _wavelength_stop = result.wavelength_stop();
    _wavelength_step = result.wavelength_step();
    _powers = result.powers();
    return *this;
}

Chroma::spd &Chroma::spd::operator*=(const Chroma::spd &rhs)
{
    Chroma::spd result = spd_arithmetic(*this, rhs, std::multiplies<float>());
    _wavelength_start = result.wavelength_start();
    _wavelength_stop = result.wavelength_stop();
    _wavelength_step = result.wavelength_step();
    _powers = result.powers();
    return *this;
}

Chroma::spd &Chroma::spd::operator/=(const Chroma::spd &rhs)
{
    Chroma::spd result = spd_arithmetic(*this, rhs, std::divides<float>());
    _wavelength_start = result.wavelength_start();
    _wavelength_stop = result.wavelength_stop();
    _wavelength_step = result.wavelength_step();
    _powers = result.powers();
    return *this;
}

Chroma::spd &Chroma::spd::operator*=(float rhs)
{
    std::transform(_powers.begin(), _powers.end(), _powers.begin(), std::bind1st(std::multiplies<float>(), rhs));
    return *this;
}

Chroma::spd Chroma::operator+(Chroma::spd lhs, const Chroma::spd &rhs) { return lhs+=rhs; }
Chroma::spd Chroma::operator+(const Chroma::spd &lhs, Chroma::spd &&rhs) { return rhs+=lhs; }
Chroma::spd Chroma::operator-(Chroma::spd lhs, const Chroma::spd &rhs) { return lhs-=rhs; }
Chroma::spd Chroma::operator*(Chroma::spd lhs, const Chroma::spd &rhs) { return lhs*=rhs; }
Chroma::spd Chroma::operator*(const Chroma::spd &lhs, Chroma::spd &&rhs) { return rhs*=lhs; }
Chroma::spd Chroma::operator/(Chroma::spd lhs, const Chroma::spd &rhs) { return lhs/=rhs; }
Chroma::spd Chroma::operator*(Chroma::spd lhs, float rhs) { return lhs*=rhs; }
Chroma::spd Chroma::operator/(Chroma::spd lhs, float rhs) { return lhs*=(1.0/rhs); }

Chroma::spd Chroma::spd::normalize(void) const
{
    float max = *std::max_element(_powers.begin(), _powers.end());
    return *this/(max);
}

Chroma::spd Chroma::spd::normalize_luminosity(float lumens) const
{
    return *this*lumens/sum();
}

float Chroma::spd::sum(void) const
{
    return std::accumulate(_powers.begin(), _powers.end(), 0.0);
}

float Chroma::spd::lumens(void) const
{
    const float photopic_conv = 683;
    Chroma::spd p(*this*(Chroma::CIE1931_photopic_eff));
    return photopic_conv * p.sum();
}

/* Perform piecewise linear interpolation to reshape the SPD to a new set of wavelengths.
 * Caveat: Old (and new) wavelength vectors must be monotonically increasing and equally
 * spaced. The constructor checks this for you.
 */
Chroma::spd Chroma::spd::reshape(float wavelength_start, float wavelength_stop, float wavelength_step) const
{
    if(wavelength_start < _wavelength_start or wavelength_stop > _wavelength_start+(_wavelength_step*_powers.size()))
    {
//        std::cerr << "Chroma::reshape: Warning: Attempting to interpolate outside dataset. Some bins will be set to zero." << std::endl;
    }

    /* Do the actual interpolation */
    std::vector<float> new_powers(std::ceil((wavelength_stop-wavelength_start)/wavelength_step)+1);
    for(size_t i=0; i<new_powers.size(); i++)
    {
        /* get the index into the old array of each new wavelength */
        float wavelength = wavelength_start+i*wavelength_step;
        int old_index = (wavelength - _wavelength_start) / _wavelength_step;
        float old_wavelength = _wavelength_start+old_index*_wavelength_step;
        if(old_index < 0) new_powers[i] = 0;
        else if(size_t(old_index) > _powers.size()-1) new_powers[i] = 0;
        else new_powers[i] = _powers[old_index] + (_powers[old_index+1]-_powers[old_index])*((wavelength-old_wavelength)/(_wavelength_step));
    }
    return {wavelength_start, wavelength_stop, new_powers};
}
