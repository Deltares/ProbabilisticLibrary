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
                void AllSensitivityTests();
            private:
                void TestAddOneCrudeMonteCarlo();
                void TestLinear();
                void TestLinearManySamples();
                void TestLinearAutoSamples();
                const double margin = 0.02;
            };
        };
    }
}

