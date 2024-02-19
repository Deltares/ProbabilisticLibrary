#include <cmath>
#include "../extern/asa063.hpp"
#include "../extern/asa109.hpp"
#include "distBeta.h"
#include "../probFuncs/conversions.h"
#include "../utils/probLibException.h"

using namespace ASA::conversions;

namespace Deltares {
    namespace ProbLibCore {

        distBeta::distBeta(const double p1, const double p2, const double p3, const double p4) :
            a(p1), b(p2), alpha(p3), beta(p4)
        {
            transform();
        }

        distBeta::distBeta(const std::string& s, const double p1, const double p2, const double p3, const double p4) :
            distrib(s, 4), a(p1), b(p2), alpha(p3), beta(p4)
        {
            transform();
        }

        void distBeta::transform()
        {
            double lnGammaA = lgamma(alpha);
            double lnGammaB = lgamma(beta);
            double lnGammaC = lgamma(alpha + beta);
            lnComplBetaFunc = lnGammaA + lnGammaB - lnGammaC;
        };

        double distBeta::getValue(const double u) const
        {
            double p; double q;
            conversions::PQfromBeta(u, p, q);
            return distributionBetaInvPQ(p, q);
        }

        double distBeta::distributionBetaInvPQ(const double p, const double q) const
        {
            double betaInverse;
            int ifault;
            if (p <= 0.5)
            {
                betaInverse = xinbta(alpha, beta, lnComplBetaFunc, p, ifault);
                betaInverse = a + (b - a) * betaInverse;
            }
            else
            {
                betaInverse = xinbta(beta, alpha, lnComplBetaFunc, q, ifault);
                betaInverse = b - (b - a) * betaInverse;
            }
            if (ifault != 0) throw probLibException("error in xinbta");
            return betaInverse;
        }

        double distBeta4p::getValue(const double u, const double p1, const double p2,
            const double p3, const double p4) const
        {
            auto d = distBeta(p1, p2, p3, p4);
            return d.getValue(u);
        }
    }
}
