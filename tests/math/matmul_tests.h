#pragma once

namespace Deltares
{
    namespace ProbLibTests
    {
        class matmul_tests
        {
        public:
            void matmul_test1();
            void matmul_test2();
            void matvec_test();
        private:
            const double margin = 1e-12;
        };
    }
}
