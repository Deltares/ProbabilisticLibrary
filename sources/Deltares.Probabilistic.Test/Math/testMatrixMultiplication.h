#pragma once

namespace Deltares
{
    namespace Probabilistic
    {
        namespace Test
        {
            class matmul_tests
            {
            public:
                void all_matmul_tests() const;
            private:
                void matmul_test1() const;
                void matmul_test2() const;
                void matvec_test() const;
                const double margin = 1e-12;
            };
        }
    }
}
