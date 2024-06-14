#pragma once
#include "../../Deltares.Probabilistic/Combine/Combiner.h"
#include "../../Deltares.Probabilistic/Combine/alphaBeta.h"

namespace Deltares
{
    namespace Probabilistic
    {
        namespace Test
        {
            class HohenbichlerCombinerTest
            {
            public:
                void allHohenbichlerCombinerTests();
            private:
                void test1();
                void test2();
                void test3();
                void tester(Reliability::Combiner* comb, const Reliability::alphaBeta& ref);
                const double margin = 1.0e-7;
            };
        }
    }
}
