#include <cmath>
#include "rayleighDist.h"
#include "../probFuncs/conversions.h"
#include "../utils/basic_math.h"

namespace Deltares {
    namespace ProbLibCore {

        // Inverse Rayleigh distribution
        // Computes the inverse CDF of the Rayleigh distribution
        // Rayleigh distribution:
        //
        // CDF: \f$ F(x) = 1 - e^-{(x - b)^2/2 a^2}, a > 0 \f$
        //
        // Inverse:  \f$ x = \sqrt {-2 a^2 \ln(1-F)} + b \f$
        double rayleighDist::getValue(const double u) const
        {
            const double q = max(qMin, conversions::QfromBeta(u));

            return sqrt(-2.0 * a * a * log(q)) + b;
        }

        // Inverse Rayleigh N distribution
        // Computes the inverse CDF of the Rayleigh N distribution
        // Rayleigh N distribution:
        //
        // CDF: \f$ F(x) = (1-exp(-x^2/(2\sigma^2))^N, \sigma,N > 0 \f$
        //
        // Inverse:  \f$ x =  \sqrt{-2\sigma^2 log(1-F^{1/N})}\f$
        double rayleighNDist::getValue(const double u) const
        {
            double p; double q;
            conversions::PQfromBeta(u, p, q);

            double argLog;
            if (q > threshold)
            {
                //
                // default implementation
                //
                double p = 1.0 - q;
                argLog = 1.0 - pow(p, 1.0 / N);
            }
            else
            {
                //
                // taylor expansion for 1 - ( 1 - q ) ^ x gives q x for small q x,
                //    avoid log(0) for large N
                //
                argLog = max(q / N, tiny);
            }
            return sqrt(-2.0 * a * a * log(argLog));
        }
    }
}
