#pragma once
#include "distrib.h"
#include <string>

namespace Deltares {
    namespace ProbLibCore {

        // header file for simple distributions:
        // - normal
        // - deterministic
        // - uspace (normal with mean=0 and sd=1)

        class normalDist : public distrib
        {
        public:
            normalDist(const double p1, const double p2) : a(p1), b(p2) {};
            normalDist(const std::string& s, const double p1, const double p2) : distrib(s, 2), a(p1), b(p2) {};
            virtual double getValue(const double u) const;
        private:
            const double a;
            const double b;
        };

        class uSpace : public distrib
        {
        public:
            uSpace(const std::string& s) : distrib(s, 0) {};
            uSpace() = default;
            virtual double getValue(const double u) const;
        };

        class deterministicDist : public distrib
        {
        public:
            deterministicDist(const double p1) : a(p1) {};
            deterministicDist(const std::string& s, const double p1) : distrib(s, 1), a(p1) {};
            virtual double getValue(const double u) const { return a; };
            virtual bool isDeterminist() const { return true; }
        private:
            const double a;
        };

        class normalDist4p : public distrib4p
        {
        public:
            virtual double getValue(const double u, const double p1, const double p2,
                const double p3, const double p4) const;
        };
    }
}
