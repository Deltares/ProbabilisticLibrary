#include <math.h>
#include "gtest/gtest.h"
#include "testCholeskiDecomposition.h"
#include "../Utils/testutils.h"

using namespace Deltares::Numeric;

namespace Deltares
{
    namespace Probabilistic
    {
        namespace Test
        {
            void choleski_decomp_tests::allCholeskyDecompositionTests()
            {
                testCholeskyDecompositionUnityMatrix();
                testCholeskyDecompositionCase2();
                testCholeskyDecompositionCase3();
                testCholeskyDecompositionCase4();
                testCholeskyDecompositionCase5();
            }

            void choleski_decomp_tests::testCholeskyDecompositionUnityMatrix()
            {
                const size_t nStochasts = 6;

                auto correlation_matrix = Matrix(nStochasts, nStochasts);
                for (size_t i = 0; i < nStochasts; i++)
                {
                    correlation_matrix(i, i) = 1.0;
                }

                auto expected_Cholesky = correlation_matrix;

                auto cholesky = correlation_matrix.CholeskyDecomposition();

                for (size_t i = 0; i < nStochasts; i++)
                {
                    for (size_t j = 0; j < nStochasts; j++)
                    {
                        EXPECT_NEAR(expected_Cholesky(i, j), cholesky(i, j), margin);
                    }
                }
            }

            void choleski_decomp_tests::performTest(const std::initializer_list<double>& m,
                const std::initializer_list<double>& ref) const
            {
                auto correlation_matrix = testutils::convert1dmatrix(m);
                size_t nStochasts;
                size_t col;
                correlation_matrix.get_dims(nStochasts, col);
                auto expected_Cholesky = testutils::convert1dmatrix(ref);
                expected_Cholesky.Transpose();
                auto cholesky = correlation_matrix.CholeskyDecomposition();

                for (size_t i = 0; i < nStochasts; i++)
                {
                    for (size_t j = 0; j < nStochasts; j++)
                    {
                        EXPECT_NEAR(expected_Cholesky(i, j), cholesky(i, j), margin);
                    }
                }
            }

            void choleski_decomp_tests::testCholeskyDecompositionCase2()
            {
                auto correlation_matrix1d =
                { 1.00, -0.85, -0.78,  0.68, -0.87,  0.42,
                 -0.85,  1.00,  0.79, -0.71,  0.89, -0.43,
                 -0.78,  0.79,  1.00, -0.45,  0.66, -0.71,
                  0.68, -0.71, -0.45,  1.00, -0.71,  0.09,
                 -0.87,  0.89,  0.66, -0.71,  1.00, -0.17,
                  0.42, -0.43, -0.71,  0.09, -0.17,  1.00 };

                auto expected_Cholesky1d =
                { 1.00, 0.000000000, 0.000000000,  0.000000000, 0.000000000, 0.00000000,
                -0.85, 0.526782688, 0.000000000,  0.000000000, 0.000000000, 0.00000000,
                -0.78, 0.241086131, 0.577475088,  0.000000000, 0.000000000, 0.00000000,
                 0.68,-0.250577711, 0.243838762,  0.644479223, 0.000000000, 0.00000000,
                -0.87, 0.285696557,-0.151482686, -0.015320060, 0.371881391, 0.00000000,
                 0.42,-0.138577067,-0.604339474, -0.128728642, 0.380422084, 0.52714151 };

                performTest(correlation_matrix1d, expected_Cholesky1d);
            }

            void choleski_decomp_tests::testCholeskyDecompositionCase3()
            {
                auto correlation_matrix1d =
                { 1.0000,  0.1000, -0.2608,  0.7539,  0.0140,  0.2256,
                  0.1000,  1.0000,  0.0572,  0.7196, -0.2814,  0.2407,
                 -0.2608,  0.0572,  1.0000, -0.1409,  0.3473,  0.0741,
                  0.7539,  0.7196, -0.1409,  1.0000, -0.1729,  0.3714,
                  0.0140, -0.2814,  0.3473, -0.1729,  1.0000, -0.0581,
                  0.2256,  0.2407,  0.0741,  0.3714, -0.0581,  1.0000 };

                auto expected_Cholesky1d =
                { 1.0000,  0.000000000, 0.000000000,  0.000000000,  0.000000000, 0.000000000,
                  0.1000,  0.994987437, 0.000000000,  0.000000000,  0.000000000, 0.000000000,
                 -0.2608,  0.083699549, 0.961757633,  0.000000000,  0.000000000, 0.000000000,
                  0.7539,  0.647455411, 0.001586048,  0.111506795,  0.000000000, 0.000000000,
                  0.0140, -0.284224694, 0.389641492, -0.000446377,  0.875899328, 0.000000000,
                  0.2256,  0.219238949, 0.119142573,  0.530763623, -0.051525609, 0.776195245 };

                performTest(correlation_matrix1d, expected_Cholesky1d);
            }

            void choleski_decomp_tests::testCholeskyDecompositionCase4()
            {
                auto correlation_matrix1d =
                { 1.0000,  0.0098, -0.3539,  0.6517,  0.0000,  0.2202,
                  0.0098,  1.0000,  0.0430,  0.6971, -0.3118,  0.2303,
                 -0.3539,  0.0430,  1.0000, -0.2143,  0.3982,  0.1238,
                  0.6517,  0.6971, -0.2143,  1.0000, -0.1577,  0.3772,
                  0.0000, -0.3118,  0.3982, -0.1577,  1.0000, -0.0125,
                  0.2202,  0.2303,  0.1238,  0.3772, -0.0125,  1.0000 };

                auto expected_Cholesky1d =
                { 1.0000,  0.000000000,  0.000000000, 0.000000000,  0.000000000, 0.000000000,
                  0.0098,  0.999951979,  0.000000000, 0.000000000,  0.000000000, 0.000000000,
                 -0.3539,  0.046470452,  0.934128089, 0.000000000,  0.000000000, 0.000000000,
                  0.6517,  0.690746510, -0.016874209, 0.312844417,  0.000000000, 0.000000000,
                  0.0000, -0.311814974,  0.441791856, 0.208218493,  0.815007017, 0.000000000,
                  0.2202,  0.228152996,  0.204604068, 0.254287120, -0.103923242, 0.884383043 };

                performTest(correlation_matrix1d, expected_Cholesky1d);
            }

            void choleski_decomp_tests::testCholeskyDecompositionCase5()
            {
                auto correlation_matrix1d =
                { 1.0, 0.8, 0.7, 0.7, 0.0, 0.8,
                  0.8, 1.0, 0.8, 0.8, 0.0, 0.8,
                  0.7, 0.8, 1.0, 0.7, 0.0, 0.7,
                  0.7, 0.8, 0.7, 1.0, 0.0, 0.7,
                  0.0, 0.0, 0.0, 0.0, 1.0, 0.3,
                  0.8, 0.8, 0.7, 0.7, 0.3, 1.0 };

                auto expected_Cholesky1d =
                { 1.0, 0.000000000, 0.000000000, 0.000000000, 0.0, 0.000000000,
                  0.8, 0.600000000, 0.000000000, 0.000000000, 0.0, 0.000000000,
                  0.7, 0.400000000, 0.591607978, 0.000000000, 0.0, 0.000000000,
                  0.7, 0.400000000, 0.084515425, 0.585540044, 0.0, 0.000000000,
                  0.0, 0.000000000, 0.000000000, 0.000000000, 1.0, 0.000000000,
                  0.8, 0.266666667, 0.056343617, 0.048795004, 0.3, 0.439696865 };

                performTest(correlation_matrix1d, expected_Cholesky1d);
            }
        }
    }
}
