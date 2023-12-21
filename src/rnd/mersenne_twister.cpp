#include "mersenne_twister.h"

namespace Deltares {
    namespace ProbLibCore {

        mersenne_twister::mersenne_twister() {}

        mersenne_twister::mersenne_twister(unsigned long long sd) : s(sd)
        {
            generator.seed(s);
        }

        double mersenne_twister::get_uniform()
        {
            std::uniform_real_distribution<double> distribution(0.0, 1.0);
            double ri = distribution(generator);
            return ri;
        }

        void mersenne_twister::restart()
        {
            generator.seed(s);
        }
    }
}
