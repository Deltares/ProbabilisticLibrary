#pragma once
#include "../../Deltares.Probabilistic/Combine/Combiner.h"
#include "../../Deltares.Probabilistic/Combine/alphaBeta.h"

namespace Deltares
{
    namespace Probabilistic
    {
        namespace Test
        {
            class CombinerTest
            {
            public:
                void allCombinerTests();
            private:
                void HohenbichlerCombinerTest();
                void DirectionalSamplingCombinerTest();
                void ImportanceSamplingCombinerTest();
                void tester(Reliability::Combiner* comb, const Reliability::alphaBeta& ref);
                const double margin = 1.0e-7;
            };
        }
    }
}
