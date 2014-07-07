// :::::::::::::::::::::::: LICENSE AND COPYRIGHT NOTICE :::::::::::::::::::::::
// Copyright (c) 2014 Nick Foster <bistromath@gmail.com>.  All rights reserved.
// 
// This file is part of Chroma.  Chroma is distributed under the GNU General
// Public License, Version 3.0.  You can read the full terms of use in the
// COPYING file, or online at http://www.gnu.org/licenses/gpl.html.
// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

/*
 * XYZ chromaticity class
 *
 * Implements a perceptual color class using XYZ as the base class.
 * All other color spaces (UVW, Lab, RGB) are calculated from this class.
 */

#pragma once
#include <Chroma/Config.hpp>
#include <Chroma/Types/Chromaticity.hpp>
#include <Chroma/Types/spd.hpp>
#include <Chroma/Data/Transforms.hpp>

namespace Chroma {

/* Forward declarations. They're at the bottom right now, and may be moved elsewhere later. */
struct xyY;
struct uv;
struct UVW;
struct Lab;
struct rgb;

struct CHROMA_API XYZ {
    /* Constructors */
    XYZ(float x, float y, float z);
    XYZ(const xyY &chrom);
    XYZ(const uv &chrom);
    XYZ(const Chroma::spd &s);
//    XYZ(rgb &color, const Chroma::Transform &xform);
    //TODO add constructors for RGB, UVW, Lab

    float X, Y, Z;
};

/* Operators */
bool CHROMA_API operator==(const Chroma::XYZ &lhs, const Chroma::XYZ &rhs);
Chroma::XYZ CHROMA_API operator+(const Chroma::XYZ &lhs, const Chroma::XYZ &rhs);
Chroma::XYZ CHROMA_API operator-(const Chroma::XYZ &lhs, const Chroma::XYZ &rhs);
Chroma::XYZ CHROMA_API operator*(const Chroma::XYZ &lhs, float rhs);
Chroma::XYZ CHROMA_API operator*(float lhs, const Chroma::XYZ &rhs);
Chroma::XYZ CHROMA_API operator/(const Chroma::XYZ &lhs, float rhs);

struct CHROMA_API rgb
{
    float r, g, b;
    rgb(float R, float G, float B) : r(R), g(G), b(B){;}
};

struct CHROMA_API uv
{
    float u, v;
    uv(float U, float V) : u(U), v(V){;}
    uv(const Chroma::XYZ &xyz);
    uv(const Chroma::xyY &xy);
};

/* Is this a terrible idea? This is currently calculating CIE1960 du'v'. */
float CHROMA_API operator-(const uv &lhs, const uv &rhs);

struct CHROMA_API xyY
{
    float x, y, Y=1;
    xyY(float xx, float yy, float YY=1) : x(xx), y(yy), Y(YY) {;}
    xyY(const Chroma::XYZ &xyz);
};

/* Same for xyY -- there's no sensible reason to add/subtract chromaticities */
float CHROMA_API operator-(const xyY &lhs, const xyY &rhs);

struct CHROMA_API UVW
{
    float U, V, W;
    UVW(float UU, float VV, float WW) : U(UU), V(VV), W(WW){;}
    UVW(const Chroma::XYZ &xyz, const Chroma::XYZ &whitepoint);
};

float CHROMA_API operator-(const UVW &lhs, const UVW &rhs);

struct CHROMA_API Lab
{
    float L, a, b;
    Lab(float LL, float aa, float bb) : L(LL), a(aa), b(bb){;}
    Lab(const Chroma::XYZ &xyz, const Chroma::XYZ &whitepoint);
};

} /* namespace Chroma */
