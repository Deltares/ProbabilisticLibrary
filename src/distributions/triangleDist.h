#pragma once
#include "distrib.h"

namespace Deltares {
    namespace ProbLibCore {

        class triangleDist : public distrib
        {
        public:
            triangleDist(const double p1, const double p2, const double p3);
            triangleDist(const std::string& s, const double p1, const double p2, const double p3);
            virtual double getValue(const double u) const;
        private:
            void inputChecks();
            const double a;
            const double b;
            const double c;
        };
    }
}
