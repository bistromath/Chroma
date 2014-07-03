#include <Chroma/Chroma.hpp>
#include <cfloat> //TODO FLT_MIN C++?
#include <cmath> //TODO powf C++?

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
Chroma::XYZ::XYZ(const Chroma::xyY &chrom)
{
    clip(chrom.x, FLT_MIN, 1);
    clip(chrom.y, FLT_MIN, 1);
    Y = chrom.Y;
    float tmp = Y/chrom.y;
    X = tmp*chrom.x;
    Z = tmp*(1-chrom.x-chrom.y);
}

Chroma::XYZ::XYZ(const Chroma::uv &chrom)
{
    float tmp = clip(2*chrom.u - 8*chrom.v + 4, FLT_MIN, FLT_MAX);
    float x = 3*chrom.u / tmp;
    float y = 2*chrom.v / tmp;
    Chroma::XYZ wat(Chroma::xyY(x,y)); //FIXME why do i have to do this arrgghh
    X = wat.X;
    Y = wat.Y;
    Z = wat.Z;
}

/*
Chroma::XYZ::XYZ(const Chroma::rgb &color, const Chroma::Transform &xform)
{
    _X=_Y=_Z=0; //TODO FIXME MATH
}
*/

Chroma::xyY Chroma::XYZ::xyY(void) const
{
    float sum = X+Y+Z;
    if(sum < FLT_MIN) return Chroma::xyY{0,0,0};
    return {X/sum, Y/sum, Y};
}

float Chroma::XYZ::x(void) const
{
    return xyY().x;
}

float Chroma::XYZ::y(void) const
{
    return xyY().y;
}

Chroma::uv Chroma::XYZ::uv(void) const
{
    float sum = X+15*Y+3*Z;
    if(sum < FLT_MIN) return Chroma::uv(0, 0);
    return {4*X/sum, 6*Y/sum};
}

Chroma::UVW Chroma::XYZ::UVW(const XYZ &whitepoint) const
{
    float W = 25*powf(Y,1.0f/3.0f)-17;
    float tmp = 13*W;
    auto my_uv = uv();
    Chroma::uv whitepoint_uv = whitepoint.uv();
    float U = tmp*(my_uv.u-whitepoint_uv.u);
    float V = tmp*(my_uv.v-whitepoint_uv.v);
    return {U,V,W};
}

Chroma::Lab Chroma::XYZ::Lab(const XYZ &whitepoint) const
{
    float sigma = 216.0f/24389.0f;
    float kappa = 24389.0f/27.0f;
    auto f = [&sigma, &kappa](float i) { if(i>sigma) return powf(i, 1.0f/3.0f); else return (kappa*i+16)/116; };
    float x = f(X/whitepoint.X);
    float y = f(Y/whitepoint.Y);
    float z = f(Z/whitepoint.Z);
    float L = 116*y - 16;
    float a = 500*(x-y);
    float b = 200*(y-z);
    return {L,a,b};
}

bool Chroma::operator==(const Chroma::XYZ &lhs, const Chroma::XYZ &rhs)
{
    return (lhs.X==rhs.X)
        && (lhs.Y==rhs.Y)
        && (lhs.Z==rhs.Z);
}

Chroma::XYZ Chroma::operator+(const Chroma::XYZ &lhs, const Chroma::XYZ &rhs)
{
    return {lhs.X+rhs.X, lhs.Y+rhs.Y, lhs.Z+rhs.Z};
}

Chroma::XYZ Chroma::operator-(const Chroma::XYZ &lhs, const Chroma::XYZ &rhs)
{
    return {lhs.X-rhs.X, lhs.Y-rhs.Y, lhs.Z-rhs.Z};
}

Chroma::XYZ Chroma::operator*(const Chroma::XYZ &lhs, float rhs)
{
    return {lhs.X*rhs, lhs.Y*rhs, lhs.Z*rhs};
}

Chroma::XYZ Chroma::operator*(float lhs, const Chroma::XYZ &rhs)
{
    return rhs*lhs;
}

Chroma::XYZ Chroma::operator/(const Chroma::XYZ &lhs, float rhs)
{
    return {lhs.X/rhs, lhs.Y/rhs, lhs.Z/rhs};
}
