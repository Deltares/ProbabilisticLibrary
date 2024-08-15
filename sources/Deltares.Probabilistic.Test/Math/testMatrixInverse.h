#pragma once

#include "../../Deltares.Probabilistic/Math/matrix.h"

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
                void matinv_singular_test() const;
                void positive_definite_tests() const;
                Deltares::Numeric::Matrix get3x3posDefiniteMatrix() const;
                Deltares::Numeric::Matrix get2x2singularMatrix() const;
                Deltares::Numeric::Matrix get2x2symmetrixMatrix() const;
                const double margin = 1e-12;
            };
        }
    }
}
