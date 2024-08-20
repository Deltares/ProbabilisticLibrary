#pragma once
#include <gtest/gtest.h>

namespace Deltares
{
    namespace Probabilistic
    {
        namespace Test
        {
            class TestSensitivity
            {
            public:
                void allSensitivityTests();
            private:
                void testCrudeMonteCarloAddOne();
                void testCrudeMonteCarloLinear();
                void testCrudeMonteCarloLinearManySamples();
                void testCrudeMonteCarloLinearAutoSamples();

                void testImportanceSamplingAddOne();

                const double margin = 0.02;
            };
        };
    }
}

