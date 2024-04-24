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
                void testMinMaxFunctions();
                const double margin = 1e-9;
            };
        }
    }
}

