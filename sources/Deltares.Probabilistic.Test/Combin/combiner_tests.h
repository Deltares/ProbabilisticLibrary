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
                void allCombinerTests() const;
            private:
                void HohenbichlerCombinerTest() const;
                void Hohenbichler2CombinerTest() const;
                void HohenbichlerCombiner1StochTest() const;
                void Hohenbichler2Combiner1StochTest() const;
                void DirectionalSamplingCombinerTest() const;
                void ImportanceSamplingCombinerTest() const;
                void ImportanceSamplingCombinerAndTest() const;
                void ImportanceSamplingCombinerInvertedTest() const;
                void tester(Reliability::Combiner* comb, const double beta, const Reliability::alphaBeta& ref, const Reliability::combineAndOr AndOr) const;
                void tester1stoch(Reliability::Combiner* comb, const double rho, const double beta, const Reliability::alphaBeta& ref, const Reliability::combineAndOr AndOr) const;
                const double margin = 1.0e-4;
            };
        }
    }
}
