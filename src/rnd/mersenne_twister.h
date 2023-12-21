#pragma once
#include <random>
#include "randomForSampling.h"

namespace Deltares {
    namespace ProbLibCore {

        class mersenne_twister : public randomForSampling
        {
        public:
            mersenne_twister();
            mersenne_twister(unsigned long long s);
            virtual double get_uniform();
            virtual void restart();
        private:
            std::mt19937_64 generator;
            const unsigned long long s = 0;
        };
    }
}
