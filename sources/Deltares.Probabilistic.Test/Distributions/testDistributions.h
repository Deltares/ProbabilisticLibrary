#pragma once
#include <gtest/gtest.h>

namespace Deltares
{
    namespace Probabilistic
    {
        namespace Test
        {
            class testDistributions
            {
            public:
                void allDistributionsTests();
            private:
                void testConditionalWeibull();
                const double margin = 1e-9;
            };
        }
    }
}

