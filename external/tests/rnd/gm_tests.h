#pragma once

namespace Deltares
{
    namespace ProbLibTests
    {
        class gm_tests
        {
        public:
            void test_first_value();
            void test_last_value();
        private:
            const double margin = 10E-12;
        };
    }
}
