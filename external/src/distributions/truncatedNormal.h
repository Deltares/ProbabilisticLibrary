#pragma once
#include "distrib.h"

namespace Deltares {
    namespace ProbLibCore {

        class truncatedNormal : public distrib
        {
        public:
            truncatedNormal(const double p1, const double p2, const double p3, const double p4);
            truncatedNormal(const std::string& s, const double p1, const double p2, const double p3, const double p4);
            virtual double getValue(const double u) const;
        private:
            void transform();
            void inputCheckTruncatedNormal(const double deviation, const double minimum, const double maximum);
            const double mean;      // a parameter: mean
            const double deviation; // b parameter: deviation
            const double minimum;   // c parameter: minimum
            const double maximum;   // d parameter: maximum
            double pa;     // exceedance probability of minimum if it was normal
            double qb;     // exceedance non-probability of maximum if it was normal
            double factor; // factor such that the truncated shape has a total area of 1
        };
    }
}
