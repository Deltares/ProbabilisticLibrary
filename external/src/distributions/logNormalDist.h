#pragma once
#include "distrib.h"

namespace Deltares {
    namespace ProbLibCore {

        class logNormalDist : public distrib
        {
        public:
            logNormalDist(const double p1, const double p2, const double p3) : a(p1), b(p2), c(p3) {};
            logNormalDist(const std::string s, const double p1, const double p2, const double p3) :
                distrib(s, 3), a(p1), b(p2), c(p3) {};
            virtual double getValue(const double u) const;
        private:
            const double a;
            const double b;
            const double c;
        };

        class logNormalIIDist : public distrib
        {
        public:
            logNormalIIDist(const double a, const double b, const double p3);
            logNormalIIDist(const std::string& s, const double a, const double b, const double p3);
            virtual double getValue(const double u) const;
        private:
            void transform(const double a, const double b);
            double c;
            double sigma;
            double mu;
        };
    }
}
