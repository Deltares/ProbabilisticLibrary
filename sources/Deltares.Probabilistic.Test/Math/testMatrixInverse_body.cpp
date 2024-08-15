#include <cmath>
#include <numbers>
#include "gtest/gtest.h"
#include "testMatrixInverse.h"

using namespace Deltares::Numeric;

namespace Deltares
{
    namespace Probabilistic
    {
        namespace Test
        {
            void matinv_tests::all_matinv_tests() const
            {
                matinv_test1();
                matinv_singular_test();
                positive_definite_tests();
            }

            Matrix matinv_tests::get3x3posDefiniteMatrix() const
            {
                auto m1 = Matrix(3, 3);
                m1(0, 0) = 0.2;
                m1(0, 1) = 0.8;
                m1(0, 2) = 1.1;
                m1(1, 0) = 0.0;
                m1(1, 1) = 0.2;
                m1(1, 2) = 0.5;
                m1(2, 0) = 1.8;
                m1(2, 1) = 1.6;
                m1(2, 2) = 1.4;
                return m1;
            }

            void matinv_tests::matinv_test1() const
            {
                auto m1 = get3x3posDefiniteMatrix();
                auto m2 = m1.Inverse();
                auto m3 = m2.matmul(m1);
                for (size_t i = 0; i < 3; i++)
                {
                    for (size_t j = 0; j < 3; j++)
                    {
                        if (i == j)
                        {
                            EXPECT_NEAR(m3(i, j), 1.0, margin);
                        }
                        else
                        {
                            EXPECT_TRUE(fabs(m3(i, j)) < margin);
                        }
                    }
                }
            }

            Matrix matinv_tests::get2x2singularMatrix() const
            {
                auto m1 = Matrix(2, 2);
                m1(0, 0) = 3.0;
                m1(0, 1) = 12.0;
                m1(1, 0) = 2.0;
                m1(1, 1) = 8.0;
                return m1;
            }

            void matinv_tests::matinv_singular_test() const
            {
                auto m1 = get2x2singularMatrix();
                try
                {
                    auto m2 = m1.Inverse();
                }
                catch (const std::exception& e)
                {
                    auto msg = e.what();
                    const char* expected = "can not inverse matrix.";
                    EXPECT_STREQ(msg, expected);
                }

                // check that original values are not changed
                EXPECT_EQ(m1(0, 0), 3.0);
                EXPECT_EQ(m1(0, 1), 12.0);
                EXPECT_EQ(m1(1, 0), 2.0);
                EXPECT_EQ(m1(1, 1), 8.0);
            }

            Matrix matinv_tests::get2x2symmetrixMatrix() const
            {
                auto m1 = Matrix(2, 2);
                m1(0, 0) = 1.0;
                m1(0, 1) = 0.5;
                m1(1, 0) = 0.5;
                m1(1, 1) = 1.0;
                return m1;
            }

            void matinv_tests::positive_definite_tests() const
            {
                auto m1 = get3x3posDefiniteMatrix();
                auto m1check = m1.IsPositiveDefinite();
                ASSERT_TRUE(m1check);

                auto m2 = get2x2singularMatrix();
                auto m2check = m2.IsPositiveDefinite();
                ASSERT_FALSE(m2check);

                auto m3 = get2x2symmetrixMatrix();
                auto m3check = m3.IsPositiveDefinite();
                ASSERT_TRUE(m3check);
            }
        }
    }
}
