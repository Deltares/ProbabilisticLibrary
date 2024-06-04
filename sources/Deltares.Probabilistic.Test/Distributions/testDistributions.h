#pragma once
#include <gtest/gtest.h>
#include "../../Deltares.Probabilistic/Statistics/Stochast.h"

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
                void testConditionalWeibullCdfPdf2();
                double getPdfNumerical(Deltares::Statistics::Stochast& s, const double x);
                const double margin = 1e-9;
            };
        }
    }
}

