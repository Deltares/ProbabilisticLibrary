#include <cmath>
#include <numbers>
#include "gtest/gtest.h"
#include "testMatrixInverse.h"
#include "../../Deltares.Probabilistic/Math/matrix.h"

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
                matinv_singular();
            }

            void matinv_tests::matinv_test1() const
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

            void matinv_tests::matinv_singular() const
            {
                auto m1 = Matrix(2, 2);
                m1(0, 0) = 3.0;
                m1(0, 1) = 12.0;
                m1(1, 0) = 2.0;
                m1(1, 1) = 8.0;
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

        }
    }
}
