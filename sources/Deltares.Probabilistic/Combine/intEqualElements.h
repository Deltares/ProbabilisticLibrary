#pragma once
#include <math.h>
#include "../Statistics/StandardNormal.h"

namespace Deltares {
    namespace Reliability {

        class intEqualElements
        {
        public:
            //
            // Method for combining failure probabilities over equal elements (core of upscaling methodes)
            //
            template <typename T>
            double integrateEqualElements(const double beta, const double rhoT, const T nrElements)
            {
                // beta       : Reliability index of a single time element
                // rhoT       : Autocorrelation coefficients for each of the variables, over the elements to be combined (space or time)
                // nrElements : Number of time elements (e.g. tidal periods)
                // returns    : Reliability index of combined elements

                // v        : Correlated variable which is integrated over for the failure probability computation (standard normally distributed)
                // betaStar : Modified reliability index (for first element) taking into account the correlation
                // p        : Non-exceedance probability of the first element
                // pfT      : Failure probability of combined elements
                // vDelta   : Step size of the numerical integration
                // vDensity : Density function (standard normal) evaluated at v, for use in the numerical integration of the probability

                const int numSteps = 90001;  // Number of grid points in the integration (intervals= numSteps- 1)
                const double LB = -30.0;  // Lower bound of v-values in the numerical integration
                const double UB = 30.0;  // Upper bound of v-values in the numerical integration
                //
                // Perform integration
                //
                // Get delta v
                const double vDelta = (UB - LB) / double(numSteps - 1);
                //
                // Find first non-zero term in numerical integration
                //
                size_t iStart = 0;
                if (rhoT > 0.0)
                {
                    const double betaStart = 8.293;  // lowest beta with PfromBeta == 1.0
                    const double vStart = (beta - betaStart * sqrt(1.0 - rhoT)) / sqrt(rhoT);
                    const double rStart = (vStart - LB) / vDelta;
                    if (rStart >= 1.0)
                    {
                        iStart = (size_t)rStart;
                    }
                }
                //
                // Numerically integrate the probability over v in [LB, UB]
                //
                double PfT = 0.0;
                for (size_t i = iStart; i < numSteps; i++)
                {
                    double v = LB + vDelta * double(i);
                    double betaStar = (beta - sqrt(rhoT) * v) / sqrt(1.0 - rhoT);
                    double p = Deltares::Statistics::StandardNormal::getPFromU(betaStar);
                    double vDensity = exp(-v * v / 2.0) / sqrt(2.0 * M_PI);
                    PfT += (1.0 - pow(p, nrElements)) * vDensity * vDelta;
                }

                //
                // Get beta based on failure probability
                //
                double betaT = Deltares::Statistics::StandardNormal::getUFromQ(PfT);
                //
                // Correct for omitting boundaries (resulting value betaT cannot be higher than beta)
                betaT = std::min(betaT, beta);
                return betaT;
            }
        };
    }
};

