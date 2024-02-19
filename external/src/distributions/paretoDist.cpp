#include <cmath>
#include "paretoDist.h"
#include "../utils/basic_math.h"
#include "../probFuncs/conversions.h"

namespace Deltares {
    namespace ProbLibCore {

        // Inverse Pareto distribution
        // Computes the inverse CDF of the Pareto distribution
        // <b> Note: </b> The form of the equation given here is not referenced
        //
        // CDF: \f$ F(x) = 1 - {1 \over 2} [1 + (y/s) (x-a)]^{-1/y}, a > 0 \f$
        //
        // NB: Must be \f$ F(x; a, s, y) = 1 -  [1 + (y/s) (x-a)]^{-1/y}, for x > a \f$  ????
        //
        // Inverse:  \f$ x = a + {s \over y} [(2 (1-F)^{-y} - 1] \f$
        double paretoDist::getValue(const double u) const
        {
            const double q = max(qMin, conversions::QfromBeta(u));

            if (fabs(y) < epsShapePar)
            {
                //      Shape close to zero, so that GP is virtually an exponential distribution
                return a - s * log(q);
            }
            else
            {
                //      Location parameter is different from zero. True GP distribution
                double pProb = 1.0 - q; //   ! Probability of non-exceedence:
                if (pProb < epsProb)
                {
                    //          qProb is near 1, use series expansion and obtain 2nd order accuracy in approximation
                    return a - s * log(q) * (1.0 - 0.5 * y * log(q));
                }
                else
                {
                    double arg = min(argMax, max(-argMax, y * log(q)));
                    //          As a result, arg is within range (-argMax, argMax) before computing exp(-arg)
                    return a + s * (exp(-arg) - 1.0) / y;
                }
            }
        }
    }
}
