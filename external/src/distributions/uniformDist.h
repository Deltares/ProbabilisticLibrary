#pragma once
#include "distrib.h"
#include "../utils/probLibException.h"
#include "../probFuncs/conversions.h"

namespace Deltares {
    namespace ProbLibCore {

        class uniformDist : public distrib
        {
        public:
            uniformDist(const double p1, const double p2)
                : a(p1), b(p2)
            {
                validate();
            };
            uniformDist(const std::string& s, const double p1, const double p2)
                : distrib(s, 2), a(p1), b(p2)
            {
                validate();
            };

            virtual double getValue(const double u) const
            {
                double p = conversions::PfromBeta(u);
                return  a + p * (b - a);
            }

        private:
            void validate()
            {
                if (b <= a)
                {
                    throw probLibException("2nd parameter of uniform distribution must be > a");
                }
            }
            const double a;
            const double b;
        };
    }
}
