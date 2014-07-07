#include <Chroma/Chroma.hpp>
#include <Chroma/Analysis/ChromaticAdaptation.hpp>
#include <eigen3/Eigen/Dense>

/* TODO: There is definitely a better way to do this */
Chroma::XYZ Chroma::chromatic_adaptation(const Chroma::XYZ &color, const Chroma::XYZ &from, const Chroma::XYZ &to, const std::vector<float> &adaptation)
{
    Eigen::Vector3f src;
    src << from.X, from.Y, from.Z;
    Eigen::Vector3f dst;
    dst << to.X, to.Y, to.Z;
    Eigen::Matrix3f adapt_m(adaptation.data());
    Eigen::Array3f crd_s = adapt_m * src;
    Eigen::Array3f crd_d = adapt_m * dst;
    Eigen::Vector3f quot = crd_d / crd_s;
    Eigen::Matrix3f M = adapt_m.inverse() * quot.asDiagonal() * adapt_m;
    Eigen::Vector3f res = M * Eigen::Vector3f(color.X, color.Y, color.Z);
    return Chroma::XYZ(res(0), res(1), res(2));
}

