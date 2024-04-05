#pragma once
#include "../../Deltares.Probabilistic/Combine/intEqualElements.h"

namespace Deltares
{
    namespace Probabilistic
    {
        namespace Test
        {
            class intEqualElements_tests
            {
            public:
                void allIntegrateEqualElementsTests();
            private:
                void integrateEqualElementsTests1();
                void integrateEqualElementsTests2();
                void integrateEqualElementsTests3();
                void integrateEqualElementsTests4();
                Deltares::Reliability::intEqualElements intEqElm;
            };
        }
    }
}
