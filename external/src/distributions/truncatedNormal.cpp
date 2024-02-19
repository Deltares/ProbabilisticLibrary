#include "truncatedNormal.h"
#include "../probFuncs/conversions.h"
#include "../utils/probLibException.h"

namespace Deltares {
    namespace ProbLibCore {

        truncatedNormal::truncatedNormal(const double p1, const double p2, const double p3, const double p4)
            : mean(p1), deviation(p2), minimum(p3), maximum(p4)
        {
            transform();
        }

        truncatedNormal::truncatedNormal(const std::string& s, const double p1, const double p2, const double p3, const double p4)
            : distrib(s, 4), mean(p1), deviation(p2), minimum(p3), maximum(p4)
        {
            transform();
        }

        void truncatedNormal::transform()
        {
            inputCheckTruncatedNormal(deviation, minimum, maximum);

            // transform limits to u-space
            double au = (minimum - mean) / deviation; // minimum transformed to u-space
            double bu = (maximum - mean) / deviation; // maximum transformed to u-space

            // detect exceedance probability of limits if it were a normal standard distribution
            pa = conversions::PfromBeta(au);
            qb = conversions::QfromBeta(bu);

            //  the shape of the distribution must be multiplied with a factor, so that the truncated shape has a total area of 1
            factor = 1.0 / (1.0 - pa - qb);
        }

        // validation for truncated normal
        void truncatedNormal::inputCheckTruncatedNormal(const double deviation, const double minimum, const double maximum)
        {
            const double zero = 0.0;   // constant 0.0
            const double diffMinMax = 1e-4;  // smallest allowed difference between minimum and maximum

            if (deviation <= zero)
            {
                throw probLibException("Truncated Normal: deviation must be > 0");
            }
            else if (minimum > maximum)
            {
                throw probLibException("Truncated Normal: minimum must be < maximum");
            }
            else if ((maximum - minimum) < diffMinMax)
            {
                throw probLibException("Truncated Normal: minimum and maximum are too close");
            }
        }

        double truncatedNormal::getValue(const double u) const
        {
            // get the exceedance probability of the requested u
            double p; double q;
            conversions::PQfromBeta(u, p, q);

            double zt;
            if (q < 0.5)
            {
                // zt is the u-value in the truncated distribution,
                // which has the same exceedance probability as u in the standard normal distribution
                // the area in the truncated distribution is: factor * (qzt - qb) = qz
                double qzt = q / factor + qb;
                zt = conversions::betaFromQ(qzt);
            }
            else
            {
                // zt is the u-value in the truncated distribution,
                // which has the same exceeedance probability as u in the standard normal distribution
                // the area in the truncated distribution is: factor * (qzt - pa) = pz
                double pzt = p / factor + pa;
                zt = conversions::betaFromQ(pzt);
                zt = -zt;
            }

            return mean + zt * deviation;
        }
    }

}