#pragma once

namespace Deltares
{
    namespace ProbLibTests
    {

        class parseAndRunTests
        {
        public:
            void runAllTests();
        private:
            void parse_run_form_with_correlation();
            void test_parse_distributions();
        };
    }
}
