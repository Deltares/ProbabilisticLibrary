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
                void testConditionalWeibullNonIntegerShape();
                void testConditionalWeibullMeanDeviation();
                void testConditionalWeibullCdfPdf();
                const double margin = 1e-9;
            };
        }
    }
}

