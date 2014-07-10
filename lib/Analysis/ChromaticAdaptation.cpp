#include <Chroma/Chroma.hpp>
#include <Chroma/Analysis/ChromaticAdaptation.hpp>
#include <eigen3/Eigen/Dense>

Eigen::Matrix3f Chroma::calculate_chromatic_adaptation_transform(const Chroma::XYZ &from,
                                                                 const Chroma::XYZ &to,
                                                                 const std::vector<float> &adaptation=Chroma::BradfordTransform)
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
    return M;
}

Chroma::XYZ Chroma::apply_chromatic_adaptation_transform(const Chroma::XYZ &color, const Eigen::Matrix3f &transform)
{
    Eigen::Vector3f res = transform * Eigen::Vector3f(color.X, color.Y, color.Z);
    return Chroma::XYZ(res(0), res(1), res(2));
}

Chroma::XYZ Chroma::chromatic_adaptation(const Chroma::XYZ &color,
                                         const Chroma::XYZ &from,
                                         const Chroma::XYZ &to,
                                         const std::vector<float> &adaptation=Chroma::BradfordTransform)
{
    Eigen::Matrix3f M = Chroma::calculate_chromatic_adaptation_transform(from, to, adaptation);
    return Chroma::apply_chromatic_adaptation_transform(color, M);
}

