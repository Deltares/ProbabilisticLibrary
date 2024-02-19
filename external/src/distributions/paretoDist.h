#pragma once
#include "distrib.h"

namespace Deltares {
    namespace ProbLibCore {

        class paretoDist : public distrib
        {
        public:
            paretoDist(const double p1, const double p2, const double p3) : a(p1), s(p2), y(p3) {};
            paretoDist(const std::string& s, const double p1, const double p2, const double p3) :
                distrib(s, 3), a(p1), s(p2), y(p3) {};
            virtual double getValue(const double u) const;
        private:
            const double a; // location parameter - threshold
            const double s; // scale parameter
            const double y; // shape parameter
            const double epsProb = 1.0e-16; // Criterion for non-exceedence probability P to use a series expansion for P very close to zero
            const double epsShapePar = 1.0e-6;  // Criterion for shape parameter to switch to exponential distribution
            const double argMax = 8.0e1;   // maximal argument for exp()
        };
    }
}
