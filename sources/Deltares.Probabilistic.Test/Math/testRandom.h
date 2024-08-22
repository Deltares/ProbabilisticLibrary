#pragma once
#include <gtest/gtest.h>

namespace Deltares
{
    namespace Probabilistic
    {
        namespace Test
        {
            class testRandom
            {
            public:
                void allRandomTests();
            private:
                void georgeMarsagliaTest1();
                void georgeMarsagliaTest2();
                void mersenneTwisterTest1();
                void initializationTest();
                const double margin = 1e-12;
            };
        }
    }
}

