// :::::::::::::::::::::::: LICENSE AND COPYRIGHT NOTICE :::::::::::::::::::::::
// Copyright (c) 2014 Nick Foster <bistromath@gmail.com>.  All rights reserved.
// 
// This file is part of Chroma.  Chroma is distributed under the GNU General
// Public License, Version 3.0.  You can read the full terms of use in the
// COPYING file, or online at http://www.gnu.org/licenses/gpl.html.
// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

#include <Chroma/Types/Chromaticity.hpp>
#include <Chroma/Types/spd.hpp>
#include <Chroma/Data/StandardObservers.hpp>
#include <cfloat>
#include <cmath>

//TODO FIXME put in Util.cpp
inline float fsel(float a, float b, float c)
{
    return a >= 0 ? b : c;
}

inline float clip(float val, float min, float max)
{
    val = fsel(val-min, val, min);
    return fsel(val-max, max, val);
}

Chroma::XYZ::XYZ(float x, float y, float z) : X(x), Y(y), Z(z) {;}

Chroma::XYZ::XYZ(Chroma::xyY chrom)
{
    chrom.x = clip(chrom.x, FLT_MIN, 1);
    chrom.y = clip(chrom.y, FLT_MIN, 1);
    Y = chrom.Y;
    float tmp = Y/chrom.y;
    X = tmp*chrom.x;
    Z = tmp*(1-chrom.x-chrom.y);
}

Chroma::XYZ::XYZ(const Chroma::uv &chrom)
{
    float tmp = 2*chrom.u - 8*chrom.v + 4;
    if(tmp < FLT_MIN)
    {
        X=0;
        Y=0;
        Z=0;
    }
    float x = 3*chrom.u / tmp;
    float y = 2*chrom.v / tmp;
    *this = Chroma::xyY(x,y);
}

Chroma::XYZ::XYZ(const Chroma::spd &s)
{
    X = (s*(Chroma::CIE1931_X)).sum();
    Y = (s*(Chroma::CIE1931_Y)).sum();
    Z = (s*(Chroma::CIE1931_Z)).sum();
}

/*
Chroma::XYZ::XYZ(const Chroma::rgb &color, const Chroma::Transform &xform)
{
    _X=_Y=_Z=0; //TODO FIXME MATH
}
*/

Chroma::xyY::xyY(const Chroma::XYZ &xyz)
{
    float sum = xyz.X+xyz.Y+xyz.Z;
    if(sum < FLT_MIN) {
        x=0;
        y=0;
        Y=0;
    } else
    {
        x=xyz.X/sum;
        y=xyz.Y/sum;
        Y=xyz.Y;
    }
}

float Chroma::operator-(const Chroma::xyY &lhs, const Chroma::xyY &rhs)
{
    float dx = lhs.x - rhs.x;
    float dy = lhs.y - rhs.y;
    return sqrtf(dx*dx+dy*dy);
}

Chroma::uv::uv(const Chroma::XYZ &xyz)
{
    float sum = xyz.X+15*xyz.Y+3*xyz.Z;
    if(sum < FLT_MIN)
    {
        u=0;
        v=0;
    } else
    {
        u=4*xyz.X/sum;
        v=6*xyz.Y/sum;
    }
}

Chroma::uv::uv(const Chroma::xyY &xy)
{
    float sum = 12*xy.y - 2*xy.x + 3;
    if(sum < FLT_MIN)
    {
        u=0;
        v=0;
    } else
    {
        u=4*xy.x / sum;
        v=6*xy.y / sum;
    }
}

float Chroma::operator-(const Chroma::uv &lhs, const Chroma::uv &rhs)
{
    float du = lhs.u - rhs.u;
    float dv = lhs.v - rhs.v;
    return sqrtf(du*du+dv*dv);
}

Chroma::UVW::UVW(const Chroma::XYZ &xyz, const XYZ &whitepoint)
{
    W = 25*powf(xyz.Y,1.0f/3.0f)-17;
    float tmp = 13*W;
    Chroma::uv my_uv(xyz);
    Chroma::uv whitepoint_uv(whitepoint);
    U = tmp*(my_uv.u-whitepoint_uv.u);
    V = tmp*(my_uv.v-whitepoint_uv.v);
}

float Chroma::operator-(const Chroma::UVW &lhs, const Chroma::UVW &rhs)
{
    float dU = lhs.U - rhs.U;
    float dV = lhs.V - rhs.V;
    float dW = lhs.W - rhs.W;
    return sqrtf(dU*dU+dV*dV+dW*dW);
}

Chroma::Lab::Lab(const Chroma::XYZ &xyz, const Chroma::XYZ &whitepoint)
{
    const float sigma = 216.0f/24389.0f;
    const float kappa = 24389.0f/27.0f;
    auto f = [&sigma, &kappa](float i) { if(i>sigma) return powf(i, 1.0f/3.0f); else return (kappa*i+16)/116; };
    float x = f(xyz.X/whitepoint.X);
    float y = f(xyz.Y/whitepoint.Y);
    float z = f(xyz.Z/whitepoint.Z);
    L = 116*y - 16;
    a = 500*(x-y);
    b = 200*(y-z);
}

Chroma::Luv::Luv(const Chroma::XYZ &color, const Chroma::XYZ &whitepoint)
{
    float Yquot = color.Y/whitepoint.Y;
    if(Yquot <= 0.008856451679035631)
    {
        L = 903.296 * Yquot;
    } else
    {
        L = 116*powf(Yquot, 1.0/3.0) - 16.0;
    }
    Chroma::uv uv_c(color);
    Chroma::uv uv_w(whitepoint);
    u = 13*L*(uv_c.u - uv_w.u);
    v = 13*L*(uv_c.v - uv_c.v);
}

float Chroma::operator-(const Chroma::Luv &lhs, const Chroma::Luv &rhs)
{
    float dL = lhs.L - rhs.L;
    float du = lhs.u - rhs.u;
    float dv = lhs.v - rhs.v;
    return sqrtf(dL*dL+du*du+dv*dv);
}

bool Chroma::operator==(const Chroma::XYZ &lhs, const Chroma::XYZ &rhs)
{
    return (lhs.X==rhs.X)
        && (lhs.Y==rhs.Y)
        && (lhs.Z==rhs.Z);
}

Chroma::XYZ &Chroma::XYZ::operator+=(const Chroma::XYZ &rhs)
{
    X+=rhs.X;
    Y+=rhs.Y;
    Z+=rhs.Z;
    return *this;
}

Chroma::XYZ &Chroma::XYZ::operator-=(const Chroma::XYZ &rhs)
{
    X-=rhs.X;
    Y-=rhs.Y;
    Z-=rhs.Z;
    return *this;
}

Chroma::XYZ &Chroma::XYZ::operator*=(float rhs)
{
    X*=rhs;
    Y*=rhs;
    Z*=rhs;
    return *this;
}

Chroma::XYZ &Chroma::XYZ::operator/=(float rhs)
{
    X/=rhs;
    Y/=rhs;
    Z/=rhs;
    return *this;
}

Chroma::XYZ Chroma::operator+(Chroma::XYZ lhs, const Chroma::XYZ &rhs)
{
    return lhs+=rhs;
}

Chroma::XYZ Chroma::operator-(Chroma::XYZ lhs, const Chroma::XYZ &rhs)
{
    return lhs-=rhs;
}

Chroma::XYZ Chroma::operator*(Chroma::XYZ lhs, float rhs)
{
    return lhs*=rhs;
}

Chroma::XYZ Chroma::operator/(Chroma::XYZ lhs, float rhs)
{
    return lhs/=rhs;
}
