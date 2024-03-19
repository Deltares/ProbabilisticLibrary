#pragma once
#include <gtest/gtest.h>

namespace Deltares
{
    namespace Probabilistic
    {
        namespace Test
        {
            class testNumericSupport
            {
            public:
                void allNumericSupportTests();
            private:
                void testLinearInterpolate();
                void testLogLinearInterpolate();
                void testHarmonicInterpolate();
                const double margin = 1e-9;
            };
        }
    }
}

