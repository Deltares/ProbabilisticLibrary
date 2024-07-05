#pragma once
#include "Randomizers/RandomValueGenerator.h"
#include <string>

namespace Deltares
{
    namespace Numeric
    {
        /**
         * MersenneTwister : C++ and Python default
         * GeorgeMarsaglia: Fortran default
         * ModifiedKnuthSubtractive: .Net default
         */
        enum RandomValueGeneratorType { MersenneTwister, GeorgeMarsaglia, ModifiedKnuthSubtractive };
        

        class Random
        {
        private:
            inline static bool repeatable = true;
            inline static int seed = 0;
            inline static int seedB = 0;
            inline static RandomValueGeneratorType generatorType = RandomValueGeneratorType::MersenneTwister;
            inline static RandomValueGenerator* randomValueGenerator = nullptr;

        public:
            static void initialize(RandomValueGeneratorType generatorType, bool repeatable = true, int seed = 0, int seedB = 0);
            static double next();
            static void restart();

            static std::string getRandomGeneratorTypeString(Deltares::Numeric::RandomValueGeneratorType method);
            static Deltares::Numeric::RandomValueGeneratorType getRandomGeneratorType(std::string method);
        };
    }
}

