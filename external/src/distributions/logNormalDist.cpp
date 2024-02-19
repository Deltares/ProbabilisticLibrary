#include <cmath>
#include "logNormalDist.h"
#include "../utils/probLibException.h"

namespace Deltares {
    namespace ProbLibCore {

        logNormalIIDist::logNormalIIDist(const double a, const double b, const double p3) : c(p3)
        {
            transform(a, b);
        }

        logNormalIIDist::logNormalIIDist(const std::string& s, const double a, const double b, const double p3) :
            distrib(s, 3), c(p3)
        {
            transform(a, b);
        }

        void logNormalIIDist::transform(const double a, const double b)
        {
            if ((a - c) < qMin)
            {
                throw probLibException("Parameter 1 of logNormalInverseII should be larger then parameter 3");
            }

            // Compute standard deviation and mean of log-transformed variable
            // from mean and standard deviation of actual variable
            sigma = sqrt(log(1.0 + pow(b / (a - c), 2)));
            mu = log(a - c) - 0.50 * sigma * sigma;
        };

        double logNormalIIDist::getValue(const double u) const
        {
            double mu_u_sigma = mu + u * sigma;

            if (mu_u_sigma > upperLog)
            {
                return exp(upperLog) + c;
            }
            else
            {
                return exp(mu_u_sigma) + c;
            }
        }

        // Computes the inverse CDF of the lognormal distribution for a given
        // standard normally distributed u-value
        //
        // Input is the mu and sigma of the log-transformed
        // variable. The inverse is then computed as follows:
        //
        // logNormalInverseII = \f$ e^{\mu + \sigma u} + \epsilon \f$
        //
        // Lognormal Distribution
        //
        // PDF:      \f$
        //                   f(x) = {1 \over {(x-\epsilon) \sqrt{2 \pi \sigma^2}}}
        //                          e^\left[{{-(\ln(x-\epsilon)-\mu)^2} \over {2 \sigma^2}}\right]
        //           \f$
        //
        // Inverse:  no explicit form
        double logNormalDist::getValue(const double u) const
        {
            double argExp = a + u * b;

            if (argExp > upperLog)
            {
                return exp(upperLog) + c;
            }
            else
            {
                return exp(argExp) + c;
            }
        }
    }
}
