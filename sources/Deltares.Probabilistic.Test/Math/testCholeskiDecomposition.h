#pragma once
#include "../../Deltares.Probabilistic/Math/matrix.h"

namespace Deltares
{
    namespace Probabilistic
    {
        namespace Test
        {
            class choleski_decomp_tests
            {
            public:
                void allCholeskyDecompositionTests();
            private:
                void testCholeskyDecompositionUnityMatrix();
                void testCholeskyDecompositionCase2();
                void testCholeskyDecompositionCase3();
                void testCholeskyDecompositionCase4();
                void testCholeskyDecompositionCase5();
                Numeric::Matrix convert1dmatrix(const std::initializer_list<double>& m) const;
                void performTest(const std::initializer_list<double>& m, const std::initializer_list<double>& ref) const;
                const double margin = 1e-9;
            };
        }
    }
}
