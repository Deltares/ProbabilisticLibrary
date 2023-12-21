#pragma once
#include "../../src/combine/intEqualElements.h"

using namespace Deltares::ProbLibCore;

namespace Deltares
{
    namespace ProbLibTests
    {

        class intEqualElements_tests
        {
        public:
            void integrateEqualElementsTests1();
            void integrateEqualElementsTests2();
            void integrateEqualElementsTests3();
            void integrateEqualElementsTests4();
        private:
            intEqualElements intEqElm;
        };
    }
}
