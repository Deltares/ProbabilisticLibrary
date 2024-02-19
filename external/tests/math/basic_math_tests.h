#pragma once

namespace Deltares
{
    namespace ProbLibTests
    {
        class basic_math_tests
        {
        public:
            void test_min();
            void test_max();
            void test_interp();
            void test_sign();
        private:
            const double margin = 1e-12;
            const float marginf = 1e-7f;
        };
    }
}
