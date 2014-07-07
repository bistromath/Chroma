#pragma once
#include <Chroma/Types/Chromaticity.hpp>
#include <eigen3/Eigen/Dense>

namespace Chroma {

const Eigen::Matrix3f BradfordAdaptation { {0.8951, 0.2664, -0.1614}, {-0.7502, 1.7135, 0.0367}, {0.0389, -0.0685, 1.0296} };
const Eigen::Matrix3f VonKriesAdaptation { {0.40024, 0.7076, -0.08081}, {-0.2263, 1.16532, 0.0457}, {0.0, 0.0, 0.91822} };
const Eigen::Matrix3f CIECAT02Adaptation { {0.7328, 0.4296, -0.1624}, {-0.7036, 1.6975, 0.0061}, {0.0030, 0.0136, 0.9834} };

Chroma::XYZ CHROMA_API chromatic_adaptation(const Chroma::XYZ &color, const Chroma::XYZ &from, const Chroma::XYZ &to, const Eigen::Matrix3f &adaptation);

} /* namespace Chroma */
