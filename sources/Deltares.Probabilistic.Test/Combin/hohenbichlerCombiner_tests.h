#pragma once

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
                const double margin = 1.0e-7;
            };
        }
    }
}
