/*
 * SPD type implementation.
 *
 * Nick Foster, 2014
 */

#include <Chroma/Types/spd.hpp>
#include <bits/algorithmfwd.h>
#include <cassert> //TODO FIXME
#include <stdexcept>

Chroma::spd::spd(const std::vector<float> &wavelengths, const std::vector<float> &powers) :
    _wavelengths(wavelengths),
    _powers(powers)
{
    if(wavelengths.size() != powers.size()) throw std::runtime_error("Wavelength and power vectors must be the same length");
}

const std::vector<float> Chroma::spd::wavelengths(void) const
{
    return _wavelengths;
}

const std::vector<float> Chroma::spd::powers(void) const
{
    return _powers;
}

bool Chroma::spd::operator==(const Chroma::spd &other)
{
    if(_wavelengths.size() != other.wavelengths().size()) return false;
    for(size_t i=0; i<this->_wavelengths.size(); i++) 
    {
        if(_wavelengths[i] != other.wavelengths()[i]) return false;
        if(_powers[i] != other.powers()[i]) return false;
    }
    return true;
}

bool Chroma::spd::operator!=(const Chroma::spd &other)
{
    return not operator==(other);
}

Chroma::spd Chroma::spd::arithmetic(const Chroma::spd &other, std::string operation)
{
    assert(_wavelengths.size() == other.wavelengths().size()); //TODO NEED RESHAPE
    for(size_t i=0; i<_wavelengths.size(); i++) assert(_wavelengths[i] == other.wavelengths()[i]);
    std::vector<float> result(_wavelengths.size());
    for(size_t i=0; i<result.size(); i++) 
    {
        if     (operation == "+") result[i] = _powers[i] + other.powers()[i];
        else if(operation == "-") result[i] = _powers[i] - other.powers()[i];
        else if(operation == "*") result[i] = _powers[i] * other.powers()[i];
        else if(operation == "/") result[i] = _powers[i] / other.powers()[i];
    }
    return Chroma::spd(_wavelengths, result);
}

Chroma::spd Chroma::spd::operator+(const Chroma::spd &other)
{
    return arithmetic(other, "+");
}

Chroma::spd Chroma::spd::operator-(const Chroma::spd &other)
{
    return arithmetic(other, "-");
}

Chroma::spd Chroma::spd::operator*(const Chroma::spd &other)
{
    return arithmetic(other, "*");
}

Chroma::spd Chroma::spd::operator/(const Chroma::spd &other)
{
    return arithmetic(other, "/");
}

Chroma::spd Chroma::spd::operator*(const float other)
{
    std::vector<float> result(_wavelengths.size());
    for(size_t i=0; i<result.size(); i++)
    {
        result[i] = _powers[i] * other;
    }
    return Chroma::spd(_wavelengths, result);
}

Chroma::spd Chroma::spd::operator/(const float other)
{
    std::vector<float> result(_wavelengths.size());
    for(size_t i=0; i<result.size(); i++)
    {
        result[i] = _powers[i] / other;
    }
    return Chroma::spd(_wavelengths, result);
}

Chroma::spd Chroma::spd::normalize(void)
{
    float max = 1;//std::max(_powers.begin(), _powers.end())[0]; //TODO FIXME
    return operator/(max);
}

