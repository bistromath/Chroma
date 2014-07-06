#include <Chroma/Types/Chromaticity.hpp>
#include <Chroma/Analysis/CCT.hpp>
#include <cfloat>
#include <cmath>
#include <iostream>

/* Estimate correlated color temperature in degrees Kelvin of a chromaticity */
float Chroma::CCT(const Chroma::xyY &chrom, Chroma::CCT_method method=Chroma::CCT_METHOD_MCCAMY)
{
    /* The method here is really easy in scipy -- use fmin to find the minimum distance
     * to the Planckian locus. Have to reimplement that here.
     * Well, here's a better way. We have a good approximation to the Planckian
     * locus in CIE1960 space, so we'll solve exactly by taking the derivative of that approximation.
     * The approximation in uv space is:
        u = (0.860117757 + 1.54118254e-4*temp + 1.28641212e-7*(temp**2)) \
        / (1 + 8.42420235e-4*temp + 7.08145163e-7*(temp**2))
        v = (0.317398726 + 4.22806245e-5*temp + 4.20481691e-8*(temp**2)) \
        / (1 - 2.89741816e-5*temp + 1.61456053e-7*(temp**2))
     * ...which is a parametric curve that I'm going to have to reformulate into Cartesian space.
     * Let's just abstract things...
     * u = (u0_n + u1_n*t + u2_n*t^2) / (u0_d + u1_d*t + u2_d*t^2)
     * v = (v0_n + v1_n*t + v2_n*t^2) / (v0_d + v1_d*t + v2_d*t^2)
     * This is a pair of rational polynomials which can be expressed implicitly using
     * resultants, which I have no idea how to use.
     * The resulting equation determined from curve fitting instead is:
     * v = (0.3877*u-0.06044)/(u-0.1448)
     * Now we get to formulate this as a distance from any given point.
     * d^2(u,v) = (u0-u)^2 + (v0-v)^2
     * sub in v from above:
     * d^2(u) = (u0-u)^2 + (v0-(0.3877*u-0.06044)/(u-0.1448))^2
     * Now find the derivative with respect to u, and set it to zero.
     * First expand...
     * d^2(u) = u0^2-2*u0*u+u^2 + v0^2-2*v0*((0.3877*u-0.06044)/(u-0.1448))+((0.3877*u-0.06044)/(u-0.1448))^2
     * then use Octave and differentiate... ;)
     * d'^2(u)=-2*((-0.06044+0.3877*u)*(-0.1448+u)^(-1)-v0)*(-0.3877*(-0.1448+u)^(-1)+(-0.06044+0.3877*u)*(-0.1448+u)^(-2))-2*u0+2*u
     *
     * Horrible. The u in the denominator means there's a pole. This is actually worse-behaved than the original function. It's not
     * continuous, therefore there's no continuous derivative, therefore you can't solve. This isn't fucking rocket science.
     *
     * The pole occurs where the asymptote of the Planckian locus occurs. You need to restrict the solution set to CCTs in 2500,10000 and
     * the corresponding valid range of u,v. To do this, we shift left (-u) and say u_n = (u+0.16).
     *
     * There are modern approximations in the (x,y) chromaticity space that are probably better than anything you're going to come up with, but when you move
     * away from the locus itself (duv) it gets progressively less accurate and falls apart below ~3kK. As this is intended to be a reference library
     * it should probably achieve some semblance of accuracy, while allowing a faster estimation if desired.
     *
     * Let's provide two methods: an approximation, and an iterative method.
     * The approximation can be McCamy or whatever modern equivalent is fast enough and more accurate.
     * The iterative method should be the method adopted in colorimetry.py, which iterates on CCT and calculates duv from the approximation.
     * Newton's method should be super fast, if we can calculate the derivative w/respect to CCT of the distance.
     * We can do that from the above implicitized equation, if we really care to, or we can bring CCT back in and try to calculate the derivative
     * of the (distance of) the original parameterized estimate.
     *
     * u = (u0_n + u1_n*t + u2_n*t^2) / (u0_d + u1_d*t + u2_d*t^2)
     * v = (v0_n + v1_n*t + v2_n*t^2) / (v0_d + v1_d*t + v2_d*t^2)
     *
     * d^2(u,v) = (u0-u)^2+(v0-v)^2
     * from matlab:
     * d'^2(u,v) =- 2*(u0 - (u2n*t^2 + u1n*t + u0n)/(u2d*t^2 + u1d*t + u0d))*((u1n + 2*t*u2n)/(u2d*t^2 + u1d*t + u0d) - ((u1d + 2*t*u2d)*(u2n*t^2 + u1n*t + u0n))/(u2d*t^2 + u1d*t + u0d)^2) - 2*(v0 - (v2n*t^2 + v1n*t + v0n)/(v2d*t^2 + v1d*t + v0d))*((v1n + 2*t*v2n)/(v2d*t^2 + v1d*t + v0d) - ((v1d + 2*t*v2d)*(v2n*t^2 + v1n*t + v0n))/(v2d*t^2 + v1d*t + v0d)^2)
     *
     *
     * well isn't that just a kick in the pants.
     *
     * auto approx = [](coeffs, t) { return coeffs[2]*t*t+coeffs[1]*t+coeffs[0]; };
     * auto unum = [&uncoeff](t) { return approx(uncoeff, t); };
     * auto uden = [&udcoeff](t) { return approx(udcoeff, t); };
     * auto vnum = [&vncoeff](t) { return approx(vncoeff, t); };
     * auto vden = [&vdcoeff](t) { return approx(vdcoeff, t); };
     *
     * auto d_prime = [&](t,u0,v0) { return -2*(u0-unum(t)/uden(t))*((uncoeffs[1]+2*t*uncoeffs[2])/uden(t) - (udcoeffs[1]+2*t*udcoeffs[2])/(uden(t)*uden(t)))
     *                                      -2*(v0-vnum(t)/vden(t))*((vncoeffs[1]+2*t*vncoeffs[2])/vden(t) - (vdcoeffs[1]+2*t*vdcoeffs[2])/(vden(t)*vden(t)));
     *                             };
     *
     * ...one more thing. we aren't looking for distance=0. we're looking for distance minimized; i.e., d'=0. so we need d'(t) and d''(t)
     * d''(t) is a huge, horrible thing and the secant method is probably easier.
     *
     * So. Do we run /function minimization/ on d(t), or /secant root finding/ on d'(t)?
     * Since we have extra info about d(t) (i.e., its analytic derivative), we might as well
     * use that information.
     *
     * Let's start with a guess based on McCamy, and then run just a little bit of optimization to get the last bit of precision out.
     */

     /* McCamy estimation, based on the common epicenter of isotherms in xyY space */
    Chroma::xyY epicenter(0.3320, 0.1858);
    float n = (chrom.x - epicenter.x) / (chrom.y - epicenter.y);
    float cct = -449*n*n*n + 3525*n*n - 6823.3*n + 5520.33;

    if(method == Chroma::CCT_METHOD_ITERATIVE) /* refine the estimate */
    {
        /* here we move to the CIE1960 u,v space. we have an analytic expression
         * estimating the Planckian locus in CIE1960 from Krystek, and from this
         * we derive an analytic expression d(u0,v0,cct) describing (squared) distance from the
         * Planckian locus. From that, we calculate the derivative and use the secant method
         * to drive it to zero.
         *
         * I'm of two minds regarding this. Should probably implement this driving duv to minimum,
         * to compare with this method. The analytic expression for the derivative of the Krystek
         * approximation is pretty involved to calculate.
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
        while(i<4)
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
