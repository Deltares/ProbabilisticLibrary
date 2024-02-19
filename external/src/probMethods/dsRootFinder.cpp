#include "dsRootFinder.h"
#include <math.h>
#include "../utils/basic_math.h"

namespace Deltares {
    namespace ProbLibCore {

        void dsRootFinder::init(const double z)
        {
            z0 = z;
        }

        std::pair<bool, double> dsRootFinder::FindRoot(FuncWrapper& func, const double rA, const double rB)
        {
            //
            // Calculate two points to start extrapolation to z = 0 (note z0 is already brought in as input)
            //
            double rPrev = rA;
            double zPrev = z0;
            double r = maxStepSize;
            double z = func.GetValue(r);
            //
            // determine if z function is starting negative and heading positive (does this really ever happen?)
            //
            if (z0 < 0.0)
            {
                zPrev = -zPrev;
                z = -z;
            }
            //
            // Initialize Dr to a high value in the case that r = 3 already leads to a zero crossing
            double Dr = 40.0;
            double slope = 1.0;
            double slopeNew = 1.0;
            double rNew = r;
            double zNew;
            //
            // Find where z passes zero (or converges to zero)
            //
            for (;;)
            {
                // check on change in sign between z and zPrev
                if (z * zPrev <= 0.0) break;
                //
                // Extrapolate to find r where z = 0
                //
                Dr = r - rPrev;
                double Dz = z - zPrev;
                slope = Dz / Dr;
                rNew = r - z * (1.0 / slope);
                //
                // Make sure step size of r is not more than 'maxStepSize' (to aid in robustness)
                //
                if (rNew - r > maxStepSize)
                {
                    if (r + 2.0 * maxStepSize > rB + 0.5)
                    {
                        // avoid gap between 18 and 20 with maxStepSize = 3 and maximumLengthU = 20
                        rNew = rB;
                    }
                    else
                    {
                        rNew = r + maxStepSize;
                    }
                }

                // I'm not modifying this code yet, I can't imagine when this happens, that rNew < r...
                if (rNew < r)
                {
                    if (settings.iterationMethod == DSiterationMethods::DirSamplingIterMethodFaster)
                    {
                        //
                        // If zNew > z (or equivalently if rNew < r), jump to r = maximumLengthU
                        //
                        rNew = rB;
                        zNew = func.GetValue(rNew);
                        if (zNew > 0) break;
                        rNew = r + maxStepSize;
                    }
                    else
                    {
                        //
                        // Make sure we can handle a local maximum in Z function,
                        // ensure r always moves to greater values
                        //
                        rNew = r + maxStepSize;
                    }
                }
                //
                // Check if rNew is greater than maximumLengthU.
                // At that point, assumed Pf = 0 in that direction, exit current loop
                //
                if (rNew > rB) break;
                // Check step size, if very small, then z is converging on one side towards zero, exit current loop.
                //
                if (fabs(rNew - r) <= settings.epsilonDu) break;
                //
                // Calculate z value for new value of r
                //
                zNew = func.GetValue(rNew);
                if (z0 < 0.0) zNew = -zNew;
                //
                // Update values of r, rPrev, z, zPrev
                zPrev = z;
                rPrev = r;
                z = zNew;
                r = rNew;

                slopeNew = (z - zPrev) / (r - rPrev);
            }

            //
            // If slope on each size of Z=0 is very different, use bisection
            //
            bool useBisection;
            switch (settings.iterationMethod)
            {
            case DSiterationMethods::DirSamplingIterMethodFastBisection:
            case DSiterationMethods::DirSamplingIterMethodFasterBisection:
                useBisection = true;
                break;
            case DSiterationMethods::DirSamplingIterMethodFastAuto:
            case DSiterationMethods::DirSamplingIterMethodFasterAuto:
                useBisection = (fabs(slopeNew) > fabs(slopeCrit * slope));
                break;
            default:
                useBisection = false;
                break;
            }

            //
            // Case where either no failure in current direction or converged on one side
            //
            if (fabs(z) < almostZero)
            {
                // note this check is not in the originating Fortran code
                return { true, rNew };
            }
            else if (z >= 0.0)
            {
                return { false, min(rNew, rB) };
            }
            //
            // Case for bounded z=0 between two points, start loop for determining z = 0
            //
            double r1 = rPrev;
            double r2 = r;
            double z1 = zPrev;
            double z2 = z;

            Dr = r2 - r1;
            double Dz = z2 - z1;

            do
            {
                //
                // Interpolate for new estimate of rNew
                //
                if (useBisection)
                {
                    rNew = 0.5 * (r1 + r2);
                }
                else
                {
                    Dr = r2 - r1;
                    Dz = z2 - z1;
                    rNew = r2 - z2 * Dr / Dz;
                }
                //
                // Check step size, if very small, exit current loop.
                //
                if (fabs(rNew - rPrev) <= settings.epsilonDu)
                {
                    if (useBisection)
                    {
                        //
                        // This is to interpolate one time at the end of the bisection method, to get closer to z=0
                        //
                        useBisection = false;
                        Dr = r2 - r1;
                        Dz = z2 - z1;
                        rNew = r2 - z2 * Dr / Dz;
                    }
                    else
                    {
                        break;
                    }
                }
                //
                // Calculate new value of z
                zNew = func.GetValue(rNew);
                if (z0 < 0.0) zNew = -zNew;
                //
                // Update values
                //
                if (zNew < 0.0)
                {
                    z2 = zNew;
                    r2 = rNew;
                    rPrev = r2;
                }
                else if (zNew > 0.0)
                {
                    z1 = zNew;
                    r1 = rNew;
                    rPrev = r1;
                }
                else  // zNew == 0.0
                {
                    break;
                }

            } while (Dr > settings.epsilonDu);

            return { true, rNew };
        }
    }
}
