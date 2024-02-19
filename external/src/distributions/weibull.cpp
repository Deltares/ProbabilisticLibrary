#include <cmath>
#include "weibull.h"
#include "../probFuncs/conversions.h"
#include "../utils/basic_math.h"

namespace Deltares {
    namespace ProbLibCore {

        // Inverse weibull distribution
        // Computes the inverse CDF of the Weibull distribution
        // Weibull distribution:
        //
        // CDF: \f$ F(x) = 1 - e^{-{[(x-c)/(a-c)]}^b} \f$
        //
        // Inverse:  \f$ x = [-\ln(1-F)]^{1/b} (a - c) + c \f$
        double weibull::getValue(const double u) const
        {
            double q = conversions::QfromBeta(u);
            q = max(q, qMin);

            return pow(-log(q), 1.0 / b) * a + c;
        }

        // Computes the inverse CDF of the conditional Weibull distribution
        // Conditional Weibull distribution:
        // CDF: \f$ F(x) = 1 - e^{[(w/a)^b -(x/a)^b]} \f$
        //
        // Inverse:  \f$ x = a [(w/a)^b-ln(f/L)]^{1/b} \f$
        // where f = frequency and L = (number of independent peaks>w)/number of years
        //
        // \f$ (w/a)^b-ln(f/L) \f$ should be non-negative
        // if \f$ [(w/a)^b-ln(f/L)] \f$ < 0 then \f$ x = 0 \f$
        double conditionalWeibull::getValue(const double u) const
        {
            double p; double q;
            conversions::PQfromBeta(u, p, q);

            double f; // Exceedance frequency
            if (q <= 3.34e-8)
            {
                f = max(q, qMin);
            }
            else
            {
                double pd = max(p, qMin);
                f = -log(pd);
            }

            double base = pow(c / a, b) - log(f / d);
            if (base <= 0.0)
            {
                return 0.0;
            }
            else
            {
                return a * pow(base, (1.0 / b));
            }
        }
    }
}
