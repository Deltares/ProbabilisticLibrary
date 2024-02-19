#pragma once
#include "../../src/combine/upscaling.h"
#include "../../src/combine/combineElements.h"
#include "../utils/testutils.h"

namespace Deltares
{
    namespace ProbLibTests
    {

        class upscale_tests
        {
        public:
            void runAll();
        private:
            void upscaleInTimeTests1();
            void upscaleInTimeTests2();
            void upscaleInTimeTests3();
            void upscaleInTimeTests4();
            void upscaleInTimeTests5();
            void upscaleInTimeTests6();
            void upscaleInTimeTests7();
            void upscaleInTimeTests8();
            void EquivalentAlphaTesting1();
            void EquivalentAlphaTesting2();
            void EquivalentAlphaTesting3();
            void EquivalentAlphaTesting4();
            void EquivalentAlphaTesting5();
            void EquivalentAlphaTesting6();
            void upscaleToLargestBlockTests1();
            void upscaleToLargestBlockTests2();
            void upscaleToLargestBlockTests3();
            upscaling up;
            combineElements cmb;
            testutils ut;
        };
    }
}
