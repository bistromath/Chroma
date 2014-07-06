#include <Chroma/Types/Chromaticity.hpp>
#include <Chroma/Analysis/CCT.hpp>
#include <cfloat>
#include <cmath>
#include <iostream>

/* Estimate correlated color temperature in degrees Kelvin of a chromaticity */
float Chroma::CCT(const Chroma::xyY &chrom, Chroma::CCT_method method, float tol)
{
     /* McCamy estimation, based on the common epicenter of isotherms in xyY space */
    Chroma::xyY epicenter(0.3320, 0.1858);
    float n = (chrom.x - epicenter.x) / (chrom.y - epicenter.y);
    float cct = -449*n*n*n + 3525*n*n - 6823.3*n + 5520.33;

    if(method == Chroma::CCT_METHOD_ITERATIVE) /* refine the estimate */
    {
        /* here we move to the CIE1960 u,v space. We have an analytic expression
         * estimating the Planckian locus in CIE1960 from Krystek, and from this
         * we derive an analytic expression d(u0,v0,cct) describing (squared) distance from the
         * Planckian locus. From that, we calculate the derivative and use the secant method
         * to drive it to zero.
         *
         * Historical note: originally I tried determining an analytic expression for the actual
         * CCT of a given u,v chromaticity, using resultants/eliminants on Krystek's approximation,
         * and solving for the derivative=0. Here be dragons -- the function is not well-behaved and
         * I couldn't derive a closed form expression. So we iterate.
         *
         * TODO: Any root-finding algorithm (Newton, secant, etc.) requires an analytical expression
         * you're aiming to drive to zero. In this case, we're driving the derivative of the distance
         * from your illuminant to the Planckian locus to zero. There are two other possible methods
         * to accomplish the same thing:
         * 1) Convex minimization on the distance to the locus, or
         * 2) Newton's method
         *
         * #1 requires only an analytic expression for the distance to the locus, which is easy
         * to describe and cheap to compute but throws away a lot of information about the curve,
         * and #2 requires the second derivative of the same expression, which is a horrible bear
         * to compute. As a middle ground I've chosen the secant method, which requires only the
         * first derivative. While I suspect #2 is never going to be viable, it's worth benchmarking
         * the secant method vs. a good convex minimization algorithm to see which one ends up faster.
         */
        const float uncoeffs[3] = {0.860117757, 1.54118254e-4, 1.28641212e-7};
        const float udcoeffs[3] = {1, 8.42420235e-4, 7.08145163e-7};
        const float vncoeffs[3] = {0.317398726, 4.22806245e-5, 4.20481691e-8};
        const float vdcoeffs[3] = {1, -2.89741816e-5, 1.61456053e-7};
        const Chroma::uv uvchrom(chrom);

        auto locus_dist_prime = [&uncoeffs, &udcoeffs, &vncoeffs, &vdcoeffs, &uvchrom] (const float cct)
        {
            float cct2 = cct*cct;
            float u_num = uncoeffs[0]+uncoeffs[1]*cct+uncoeffs[2]*cct2;
            float u_den = udcoeffs[0]+udcoeffs[1]*cct+udcoeffs[2]*cct2;
            float v_num = vncoeffs[0]+vncoeffs[1]*cct+vncoeffs[2]*cct2;
            float v_den = vdcoeffs[0]+vdcoeffs[1]*cct+vdcoeffs[2]*cct2;
            return -2*(uvchrom.u-u_num/u_den)*((uncoeffs[1]+2*cct*uncoeffs[2])/u_den - ((udcoeffs[1]+2*cct*udcoeffs[2])*u_num)/(u_den*u_den))
                   -2*(uvchrom.v-v_num/v_den)*((vncoeffs[1]+2*cct*vncoeffs[2])/v_den - ((vdcoeffs[1]+2*cct*vdcoeffs[2])*v_num)/(v_den*v_den));
        };

        float x0 = cct-50;
        float fx0 = locus_dist_prime(x0);
        float fx1;

        size_t i=0;
        while(i<10 and (cct-x0) > tol)
        {
            fx1 = locus_dist_prime(cct);
            if(fabs(fx1-fx0) <= FLT_MIN) break;
            float d = (cct-x0) / (fx1-fx0) * fx1;
            x0 = cct;
            fx0 = fx1;
            cct -= d;
            i++;
        }
    }

    return cct;
}
