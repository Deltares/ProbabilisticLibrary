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
                const double margin = 1e-12;
            };
        }
    }
}

