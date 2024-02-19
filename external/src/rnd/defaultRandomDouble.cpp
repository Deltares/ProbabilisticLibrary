#include "defaultRandomDouble.h"

namespace Deltares {
    namespace ProbLibCore {

        defaultRandomDouble::defaultRandomDouble() {}

        defaultRandomDouble::defaultRandomDouble(int seed) : sd(seed)
        {
            srand(seed);
        }

        double defaultRandomDouble::get_uniform()
        {
            auto r = (double)rand() / (double)RAND_MAX;
            return r;
        }

        void defaultRandomDouble::restart()
        {
            srand(sd);
        }
    }
}
