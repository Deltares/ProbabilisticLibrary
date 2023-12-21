#include "lsf.h"
#include <cmath>

namespace Deltares {
    namespace ProbLibCore {

        void lsf::setVarCoeff(const double Z0Fac, const double varCoeffFInp, const double varCoeffNFinp)
        {
            if (Z0Fac > 0)
            {
                varCoeffF2 = pow(varCoeffFInp, 2);
                varCoeffNF2 = pow(varCoeffNFinp, 2);
            }
            else
            {
                varCoeffF2 = pow(varCoeffNFinp, 2);
                varCoeffNF2 = pow(varCoeffFInp, 2);
            }
        }
    }
}
