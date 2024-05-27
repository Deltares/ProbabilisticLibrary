#pragma once
#include <gtest/gtest.h>

namespace Deltares
{
    namespace Probabilistic
    {
        namespace Test
        {
            class testKMean
            {
            public:
                void allKMeanTests();
            private:
                void test1();
                const double margin = 1e-2;
            };
        }
    }
}

