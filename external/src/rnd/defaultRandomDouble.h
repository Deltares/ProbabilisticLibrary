#pragma once
#include <random>
#include "randomForSampling.h"

namespace Deltares {
    namespace ProbLibCore {

        class defaultRandomDouble : public randomForSampling
        {
        public:
            defaultRandomDouble();
            defaultRandomDouble(int seed);
            virtual double get_uniform();
            virtual void restart();
        private:
            const int sd = 0;
        };
    }
}
