#include <Chroma/Chroma.hpp>
#include <Chroma/Analysis/ChromaticAdaptation.hpp>
#include <eigen3/Eigen/Dense>

/* TODO: There is definitely a better way to do this */
Chroma::XYZ Chroma::chromatic_adaptation(const Chroma::XYZ &color, const Chroma::XYZ &from, const Chroma::XYZ &to, const Eigen::Matrix3f &adaptation)
{
    Eigen::Vector3f src { from.X, from.Y, from.Z };
    Eigen::Vector3f dst { to.X, to.Y, to.Z };
    Eigen::Array3f crd_s = adaptation * src;
    Eigen::Array3f crd_d = adaptation * dst;
    Eigen::Vector3f quot = crd_d / crd_s;
    Eigen::Matrix3f M = adaptation.inverse() * quot.asDiagonal() * adaptation;
    Eigen::Vector3f res = M * Eigen::Vector3f(color.X, color.Y, color.Z);
    return Chroma::XYZ(res(0), res(1), res(2));
}

