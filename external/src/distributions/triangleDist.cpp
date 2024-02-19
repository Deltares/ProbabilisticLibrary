#include <cmath>
#include "triangleDist.h"
#include "../probFuncs/conversions.h"
#include "../utils/probLibException.h"

namespace Deltares {
    namespace ProbLibCore {

        triangleDist::triangleDist(const double p1, const double p2, const double p3) : a(p1), b(p2), c(p3)
        {
            inputChecks();
        }

        triangleDist::triangleDist(const std::string& s, const double p1, const double p2, const double p3) :
            distrib(s, 3), a(p1), b(p2), c(p3)
        {
            inputChecks();
        }

        void triangleDist::inputChecks()
        {
            if (b < a)
            {
                throw probLibException("Parameter 2 of triangle dist. should be > parameter 1");
            }
            else if (c < b)
            {
                throw probLibException("Parameter 3 of triangle dist. should be > parameter 2");
            }
            else if (fabs(c - a) < qMin)
            {
                throw probLibException("Parameter 3 of triangle dist. should differ from parameter 1");
            }
        };

        // Inverse triangular distribution
        // Computes the inverse CDF of a triangular distribution
        // Triangular distribution:
        //
        // CDF: \f$ F(x) = {(x - a)^2 \over {(b - a) (c - a)}} \f$
        //
        // Inverse:
        // For x values lower than b, which corresponds to p values lower than (b-a)/(c-a):
        // \f$ x = a + \sqrt{F (b-a) (c-a)} \f$
        // For x values greater than b, which corresponds to p values greater than (b-a)/(c-a):
        // \f$ x = c - \sqrt{(1-F) (c-a) (c-b)} \f$
        double triangleDist::getValue(const double u) const
        {
            double p; double q;
            conversions::PQfromBeta(u, p, q);

            if (p < (b - a) / (c - a))
            {
                return a + sqrt(p * (b - a) * (c - a));
            }
            else
            {
                return c - sqrt(q * (c - b) * (c - a));
            }
        }
    }
}
