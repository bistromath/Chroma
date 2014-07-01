#pragma once
#include <Chroma/Types/spd.hpp>

namespace Chroma {

/* CIE1931 photopic efficiency curve */
const Chroma::spd CIE1931_photopic_eff;

/* CIE1931 standard observer X, Y, Z curves */
const Chroma::spd CIE1931_X;
const Chroma::spd CIE1931_Y;
const Chroma::spd CIE1931_Z;

/* D standard illuminant coefficients */
const Chroma::spd D_illuminant_S0;
const Chroma::spd D_illuminant_S1;
const Chroma::spd D_illuminant_S2;

/* F standard illuminants */
const Chroma::spd F1_illuminant;
const Chroma::spd F2_illuminant;
const Chroma::spd F3_illuminant;
const Chroma::spd F4_illuminant;
const Chroma::spd F5_illuminant;
const Chroma::spd F6_illuminant;
const Chroma::spd F7_illuminant;
const Chroma::spd F8_illuminant;
const Chroma::spd F9_illuminant;
const Chroma::spd F10_illuminant;
const Chroma::spd F11_illuminant;
const Chroma::spd F12_illuminant;

/* Color rendering index tiles */
const Chroma::spd CRI_TCS01;
const Chroma::spd CRI_TCS02;
const Chroma::spd CRI_TCS03;
const Chroma::spd CRI_TCS04;
const Chroma::spd CRI_TCS05;
const Chroma::spd CRI_TCS06;
const Chroma::spd CRI_TCS07;
const Chroma::spd CRI_TCS08;
const Chroma::spd CRI_TCS09;
const Chroma::spd CRI_TCS10;
const Chroma::spd CRI_TCS11;
const Chroma::spd CRI_TCS12;
const Chroma::spd CRI_TCS13;
const Chroma::spd CRI_TCS14;

/* Color quality scale tiles */
const Chroma::spd CQS_Q1;
const Chroma::spd CQS_Q2;
const Chroma::spd CQS_Q3;
const Chroma::spd CQS_Q4;
const Chroma::spd CQS_Q5;
const Chroma::spd CQS_Q6;
const Chroma::spd CQS_Q7;
const Chroma::spd CQS_Q8;
const Chroma::spd CQS_Q9;
const Chroma::spd CQS_Q10;
const Chroma::spd CQS_Q11;
const Chroma::spd CQS_Q12;
const Chroma::spd CQS_Q13;
const Chroma::spd CQS_Q14;
const Chroma::spd CQS_Q15;

} /* namespace Chroma */
