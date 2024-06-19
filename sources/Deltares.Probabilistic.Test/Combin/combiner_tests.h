#pragma once
#include "../../Deltares.Probabilistic/Combine/combiner.h"
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
                void HohenbichlerCombinerTest();
                void DirectionalSamplingCombinerTest();
                void ImportanceSamplingCombinerTest();
                void ImportanceSamplingCombinerAndTest();
            private:
                void tester(Reliability::Combiner* comb, const double beta, const Reliability::alphaBeta& ref, const Reliability::combineAndOr AndOr) const;
                double margin = 1.0e-7;
            };
        }
    }
}
