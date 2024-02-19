#include <cmath>
#include "exponentialDist.h"
#include "../probFuncs/conversions.h"
#include "../utils/basic_math.h"

namespace Deltares {
    namespace ProbLibCore {

        // Inverse shifted exponential distribution
        // Computes the inverse CDF of the exponential distribution for a given exceedance probability
        // exponentialInverse = distParameter2 - log(1-p)/distParameter1
        // Exponential distribution:
        //
        // CDF:      \f$F(x) = 1 - e^{-a(x-b)}\f$
        //
        // Inverse:  \f$x(F) = b - \ln(1-F) / a\f$
        double exponentialDist::getValue(const double u) const
        {
            // Constraint: cannot compute log of values too close to zero
            const double q = max(qMin, conversions::QfromBeta(u));

            // Compute
            return b - log(q) / a;
        }

    }
}