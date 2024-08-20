#pragma once

namespace Deltares
{
    namespace Probabilistic
    {
        namespace Test
        {
            class vector1D_tests
            {
            public:
                void allVector1Dtests() const;
            private:
                void vector_sum_test() const;
                void vector_normalize_test() const;
                void minmax_test() const;
            };
        }
    }
}
