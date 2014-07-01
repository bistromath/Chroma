/*
 * XYZ chromaticity class
 *
 * Nick Foster, 2014
 *
 * Implements a perceptual color class using XYZ as the base class.
 * All other color spaces (UVW, Lab, RGB) are calculated from this class.
 */

#pragma once
#include <Chroma/Chroma.hpp>
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
//    XYZ(rgb &color, const Chroma::Transform &xform);
    //TODO add constructors for UVW, Lab

    /* CIE1931 chromaticity conversions */
    float x(void) const;
    float y(void) const;
    Chroma::xyY xyY(void) const;

    /* CIE1960 u', v' chromaticity conversions */
    Chroma::uv uv(void) const;

    /* CIE1964 UVW conversions */
    Chroma::UVW UVW(const XYZ &whitepoint) const;

    /* CIELAB conversions */
    float L(const XYZ &whitepoint) const;
    float a(const XYZ &whitepoint) const;
    float b(const XYZ &whitepoint) const;
    Chroma::Lab Lab(const XYZ &whitepoint) const;

    /* RGB conversions */
    //rgb_t rgb(const Chroma::Transform &transform);

    /* Operators */
    bool operator==(const XYZ &other);
    XYZ operator+(const XYZ &other);
    XYZ operator-(const XYZ &other);
    XYZ operator*(const float other);
    XYZ operator/(const float other);

    float X, Y, Z;
};

/* Simple containers to allow constructor overloading convenience and to enforce typing */
struct CHROMA_API rgb {
    float r, g, b;
    rgb(float R, float G, float B) : r(R), g(G), b(B){;}
};

struct CHROMA_API uv {
    float u, v;
    uv(float U, float V) : u(U), v(V){;}
};

struct CHROMA_API xyY {
    float x, y, Y=1;
    xyY(float xx, float yy, float YY=1) : x(xx), y(yy), Y(YY) {;}
};

struct CHROMA_API UVW {
    float U, V, W;
    UVW(float UU, float VV, float WW) : U(UU), V(VV), W(WW){;}
};

struct CHROMA_API Lab {
    float L, a, b;
    Lab(float LL, float aa, float bb) : L(LL), a(aa), b(bb){;}
};

}
