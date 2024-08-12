#pragma once

namespace Deltares
{
    namespace Probabilistic
    {
        namespace Test
        {
            class matinv_tests
            {
            public:
                void all_matinv_tests() const;
            private:
                void matinv_test1() const;
                void matinv_singular() const;
                const double margin = 1e-12;
            };
        }
    }
}
