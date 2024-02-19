#pragma once
#include "distrib.h"

namespace Deltares {
    namespace ProbLibCore {

        class exponentialDist : public distrib
        {
        public:
            exponentialDist(const double p1, const double p2) : a(p1), b(p2) {};
            exponentialDist(const std::string& s, const double p1, const double p2) : distrib(s, 2), a(p1), b(p2) {};
            virtual double getValue(const double u) const;
            double distributionBetaInvPQ(const double p, const double q) const;
        private:
            const double a; // rate parameter
            const double b; // represents lower endpoint of the distribution
        };
    }
}