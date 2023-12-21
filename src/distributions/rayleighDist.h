#pragma once
#include "distrib.h"

namespace Deltares {
    namespace ProbLibCore {

        class rayleighDist : public distrib
        {
        public:
            rayleighDist(const double p1, const double p2) : a(p1), b(p2) {};
            rayleighDist(const std::string& s, const double p1, const double p2) : distrib(s, 2), a(p1), b(p2) {};
            virtual double getValue(const double u) const;
        private:
            const double a; // distribution parameter (a in above equation)
            const double b; // location parameter (horizontal shifting)
        };

        class rayleighNDist : public distrib
        {
        public:
            rayleighNDist(const double p1, const double p2) : a(p1), N(p2) {};
            rayleighNDist(const std::string& s, const double p1, const double p2) : distrib(s, 2), a(p1), N(p2) {};
            virtual double getValue(const double u) const;
        private:
            const double a; // sigma, distribution parameter
            const double N; // N, distribution parameter
            const double threshold = 1e-6;
        };
    }
}
