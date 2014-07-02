/*
 * SPD type implementation.
 *
 * Nick Foster, 2014
 */

#include <Chroma/Types/spd.hpp>
#include <Chroma/Data/PhotopicEfficiency.hpp>
#include <Chroma/Data/StandardObservers.hpp>
#include <stdexcept>
#include <algorithm>
#include <iostream>

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
    _wavelengths = wavelengths;
    _powers = powers;
}

Chroma::spd::spd(const Chroma::spd &other)
    : _wavelengths(other.wavelengths()),
      _powers(other.powers())
{
}

/* TODO: find out if STL will automatically move rvalues in the copy constructor */
Chroma::spd::spd(Chroma::spd &&other)
    : _wavelengths(std::move(other.wavelengths())),
      _powers(std::move(other.powers()))
{
}

Chroma::spd& Chroma::spd::operator=(const spd &other)
{
    _wavelengths = other.wavelengths();
    _powers = other.powers();
    return *this;
}

Chroma::spd& Chroma::spd::operator=(spd &&other)
{
    _wavelengths = std::move(other.wavelengths());
    _powers = std::move(other.powers());
    return *this;
}

const std::vector<float> &Chroma::spd::wavelengths(void) const
{
    return _wavelengths;
}

const std::vector<float> &Chroma::spd::powers(void) const
{
    return _powers;
}

bool Chroma::operator==(const Chroma::spd &lhs, const Chroma::spd &rhs)
{
    return (lhs.wavelengths() == rhs.wavelengths() and lhs.powers() == rhs.powers());
}

bool Chroma::operator!=(const Chroma::spd &lhs, const Chroma::spd &rhs)
{
    return not (lhs==rhs);
}

Chroma::spd Chroma::spd_arithmetic(const Chroma::spd &l, const Chroma::spd &r, std::string operation)
{
    /* This byzantine construction is an attempt to avoid copy-construction
     * while also avoiding rewrite of the arithmetic logic at the bottom.
     * We can't use references because they have to be initialized here and
     * we don't know what they will be yet. We can't use regular Chroma::spd
     * and depend on the move assignment operator because other is const and
     * will thus copy. We can't just use pointers because we can't cast away
     * constness.
     *
     * We create a temporary object to hold the reshaped side (whichever that is)
     * and a couple of const pointers to point to the correct sides and maintain
     * order for non-commutative operations. We safely const_cast the temporary
     * object to assign the pointer.
     */
    const Chroma::spd *lhs, *rhs;
    Chroma::spd temp;
    if(l.wavelengths().size() < 2 or r.wavelengths().size() < 2)
    {
        throw std::runtime_error("Attempting to do math on an empty SPD.");
    }
    if(l.wavelengths() != r.wavelengths())
    {
        /* we're going to reshape to match the one with the best resolution. */
        float lhsstep = l.wavelengths()[1]-l.wavelengths()[0];
        float rhsstep = r.wavelengths()[1] - r.wavelengths()[0];
        if(lhsstep < rhsstep)
        {
            lhs = &l;
            temp = r.reshape(l.wavelengths());
            rhs = const_cast <const Chroma::spd *>(&temp);
        }
        else
        {
            temp = l.reshape(r.wavelengths());
            lhs = const_cast <const Chroma::spd *>(&temp);
            rhs = &r;
        }
    }
    else
    {
        lhs = &l;
        rhs = &r;
    }
    std::vector<float> result(lhs->wavelengths().size());
    for(size_t i=0; i<result.size(); i++)
    {
        if     (operation == "+") result[i] = lhs->powers()[i] + rhs->powers()[i];
        else if(operation == "-") result[i] = lhs->powers()[i] - rhs->powers()[i];
        else if(operation == "*") result[i] = lhs->powers()[i] * rhs->powers()[i];
        else if(operation == "/") result[i] = lhs->powers()[i] / rhs->powers()[i];
    }
    return {lhs->wavelengths(), result};
}

Chroma::spd Chroma::operator+(const Chroma::spd &lhs, const Chroma::spd &rhs)
{
    return spd_arithmetic(lhs, rhs, "+");
}

Chroma::spd Chroma::operator-(const Chroma::spd &lhs, const Chroma::spd &rhs)
{
    return spd_arithmetic(lhs, rhs, "-");
}

Chroma::spd Chroma::operator*(const Chroma::spd &lhs, const Chroma::spd &rhs)
{
    return spd_arithmetic(lhs, rhs, "*");
}

Chroma::spd Chroma::operator/(const Chroma::spd &lhs, const Chroma::spd &rhs)
{
    return spd_arithmetic(lhs, rhs, "/");
}

Chroma::spd Chroma::operator*(const Chroma::spd &lhs, float rhs)
{
    std::vector<float> result(lhs.wavelengths().size());
    for(size_t i=0; i<result.size(); i++)
    {
        result[i] = lhs.powers()[i] * rhs;
    }
    return {lhs.wavelengths(), result};
}

Chroma::spd Chroma::operator*(float lhs, const Chroma::spd &rhs)
{
    return rhs*lhs;
}

Chroma::spd Chroma::operator/(const Chroma::spd &lhs, const float rhs)
{
    std::vector<float> result(lhs.wavelengths().size());
    for(size_t i=0; i<result.size(); i++)
    {
        result[i] = lhs.powers()[i] / rhs;
    }
    return {lhs.wavelengths(), result};
}

Chroma::spd Chroma::spd::normalize(void) const
{
    float max = *std::max_element(_powers.begin(), _powers.end());
    return *this/(max);
}

float Chroma::spd::sum(void) const
{
    return std::accumulate(_powers.begin(), _powers.end(), 0.0);
}

Chroma::XYZ Chroma::spd::XYZ(void) const
{
    Chroma::spd X(*this*(Chroma::CIE1931_X));
    Chroma::spd Y(*this*(Chroma::CIE1931_Y));
    Chroma::spd Z(*this*(Chroma::CIE1931_Z));
    return {X.sum(),Y.sum(),Z.sum()};
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
Chroma::spd Chroma::spd::reshape(const std::vector<float> &new_wavelengths) const
{
    if(_wavelengths.size() < 2 or new_wavelengths.size() < 2)
    {
        throw std::runtime_error("Invalid wavelength array"); //TODO: pick a better error
    }

    if(new_wavelengths[0] < _wavelengths[0] or new_wavelengths[new_wavelengths.size()-1] > _wavelengths[_wavelengths.size()-1])
    {
        std::cerr << "Chroma::reshape: Warning: Attempting to interpolate outside dataset. Some bins will be set to zero." << std::endl;
    }

    /* Do the actual interpolation */
    std::vector<float> new_powers(new_wavelengths.size());
    size_t step = _wavelengths[1] - _wavelengths[0];
    for(size_t i=0; i<new_powers.size(); i++)
    {
       int old_index = (new_wavelengths[i] - _wavelengths[0]) / step;
       if(old_index < 0) new_powers[i] = 0;
       else if(size_t(old_index) > _wavelengths.size()-1) new_powers[i] = 0;
       else new_powers[i] = _powers[old_index] + (_powers[old_index+1]-_powers[old_index])*((new_wavelengths[i]-_wavelengths[old_index])/(_wavelengths[old_index+1]-_wavelengths[old_index]));
    }
    return {new_wavelengths, new_powers};
}
