#pragma once
#include "zmodel.h"
#include "../distributions/distrib.h"
#include "lsfResult.h"

namespace Deltares {
    namespace ProbLibCore {

        class lsf
        {
        public:
            virtual lsfResult calc(const zmodel& m, const stochSettings& s) = 0;
            void setVarCoeff(const double Z0Fac, const double varCoeffFInp, const double varCoeffNFinp);
            virtual ~lsf() {};
        protected:
            size_t nrStochasts = 0;
            double varCoeffF2 = -999.0; // effective square of variation coefficient for failure
            double varCoeffNF2 = -999.0; // effective square of variation coefficient for non failure
        };
    }
}
