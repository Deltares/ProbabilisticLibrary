#include "identity.h"
#include <iostream>

namespace Deltares {
    namespace ProbLibCore {

        identity::identity(const int nrStochs) : nrStochasts(nrStochs) {}

        void identity::updateU(vector1D& u) const { ; }

        int identity::cntIndependent() const
        {
            return nrStochasts;
        }
    }
}
