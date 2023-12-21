#pragma once

namespace Deltares
{
    namespace ProbLibTests
    {

        class conversion_tests
        {
        public:
            void all_conversion_tests();
        private:
            void returnTimeFromBetaTests();
            void freqFromBetaTests();
            void logqFromBetaTests();
        };
    }
}
