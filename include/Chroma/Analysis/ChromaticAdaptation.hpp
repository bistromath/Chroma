#pragma once
#include <Chroma/Types/Chromaticity.hpp>
#include <eigen3/Eigen/Dense>

namespace Chroma {

const std::vector<float> BradfordTransform { 0.8951, -0.7502, 0.0389, 0.2664, 1.7135, -0.0685, -0.1614, 0.0367, 1.0296 };
const std::vector<float> VonKriesTransform { 0.40024, -0.2263, 0.0, 0.7076, 1.16532, 0.0, -0.08081, 0.0457, 0.91822 };
const std::vector<float> CIECAT02Transform { 0.7328, -0.7036, 0.0030, 0.4296, 1.6975, 0.0136, -0.1624, 0.0061, 0.9834 };

Chroma::XYZ CHROMA_API chromatic_adaptation(const Chroma::XYZ &color, const Chroma::XYZ &from, const Chroma::XYZ &to, const std::vector<float> &adaptation);
Eigen::Matrix3f CHROMA_API calculate_chromatic_adaptation_transform(const Chroma::XYZ &from, const Chroma::XYZ &to, const std::vector<float> &adaptation);
Chroma::XYZ CHROMA_API apply_chromatic_adaptation_transform(const Chroma::XYZ &color, const Eigen::Matrix3f &transform);

} /* namespace Chroma */
