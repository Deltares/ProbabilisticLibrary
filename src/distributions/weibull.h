#pragma once
#include "distrib.h"

namespace Deltares {
    namespace ProbLibCore {

        class weibull : public distrib
        {
        public:
            weibull(const double p1, const double p2, const double p3) : a(p1), b(p2), c(p3) {};
            weibull(const std::string& s, const double p1, const double p2, const double p3) :
                distrib(s, 3), a(p1), b(p2), c(p3) {};
            virtual double getValue(const double u) const;
        private:
            const double a;
            const double b;
            const double c;
        };

        class conditionalWeibull : public distrib
        {
        public:
            conditionalWeibull(const double p1, const double p2, const double p3, const double p4) : a(p1), b(p2), c(p3), d(p4) {};
            conditionalWeibull(const std::string& s, const double p1, const double p2, const double p3, const double p4) :
                distrib(s, 4), a(p1), b(p2), c(p3), d(p4) {};
            virtual double getValue(const double u) const;
        private:
            const double a;
            const double b;
            const double c;
            const double d;
        };
    }
}
