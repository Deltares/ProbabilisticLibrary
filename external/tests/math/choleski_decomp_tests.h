#pragma once
#include "../../src/utils/matrix.h"

using namespace Deltares::ProbLibCore;

namespace Deltares
{
    namespace ProbLibTests
    {

        class choleski_decomp_tests
        {
        public:
            void testCholeskyDecompositionUnityMatrix();
            void testCholeskyDecompositionCase2();
            void testCholeskyDecompositionCase3();
            void testCholeskyDecompositionCase4();
            void testCholeskyDecompositionCase5();
        private:
            Matrix convert1dmatrix(const std::initializer_list<double>& m) const;
            void performTest(const std::initializer_list<double>& m, const std::initializer_list<double>& ref) const;
            const double margin = 1e-9;
        };
    }
}
