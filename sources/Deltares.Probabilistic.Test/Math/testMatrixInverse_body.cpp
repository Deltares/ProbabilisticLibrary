// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of the Probabilistic Library.
//
// The Probabilistic Library is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// All names, logos, and references to "Deltares" are registered trademarks of
// Stichting Deltares and remain full property of Stichting Deltares at all times.
// All rights reserved.
//
#include <cmath>
#include <numbers>
#include "gtest/gtest.h"
#include "testMatrixInverse.h"
#include "../Utils/testutils.h"

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
                qr_tests();
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

            Matrix matinv_tests::get16x16Matrix() const
            {
                auto matrix = 
                    {
                         1.0, 0.21076, 0.70884, 0.73605, 0.78807, 0.63547, 0.16428, 0.23592, 0.65908, 0.22649, 0.56082, 0.18181, 0.46315, 0.85656, 0.53365, 0.51922,
                         0.21076, 1.0, 0.12841, 0.11929, 0.17762, 0.14725, 0.048919, 0.98775, 0.37244, 0.069508, 0.18167, 0.95003, 0.16674, 0.16974, 0.56781, 0.17373 ,
                         0.70884, 0.12841, 1.0, 0.73743, 0.72403, 0.92173, 0.29188, 0.12102, 0.69862, 0.33278, 0.62551, 0.17439, 0.65134, 0.62532, 0.60051, 0.38451 ,
                         0.73605, 0.11929, 0.73743, 1.0, 0.83932, 0.60833, 0.31827, 0.13532, 0.52655, 0.36584, 0.52862, 0.13355, 0.36535, 0.60835, 0.42408, 0.44136 ,
                         0.78807, 0.17762, 0.72403, 0.83932, 1.0, 0.5932, 0.21426, 0.179, 0.52232, 0.28852, 0.48393, 0.17737, 0.35808, 0.67623, 0.42194, 0.4356 ,
                         0.63547, 0.14725, 0.92173, 0.60833, 0.5932, 1.0, 0.26503, 0.15158, 0.76413, 0.27666, 0.72624, 0.20237, 0.88438, 0.67998, 0.61061, 0.51864 ,
                         0.16428, 0.048919, 0.29188, 0.31827, 0.21426, 0.26503, 1.0, 0.036463, 0.25759, 0.98521, 0.13184, 0.054544, 0.2296, 0.1059, 0.16791, 0.21016 ,
                         0.23592, 0.98775, 0.12102, 0.13532, 0.179, 0.15158, 0.036463, 1.0, 0.38629, 0.055959, 0.21529, 0.9366, 0.18085, 0.21809, 0.56624, 0.21682 ,
                         0.65908, 0.37244, 0.69862, 0.52655, 0.52232, 0.76413, 0.25759, 0.38629, 1.0, 0.26941, 0.89338, 0.47424, 0.74206, 0.64367, 0.90008, 0.63933 ,
                         0.22649, 0.069508, 0.33278, 0.36584, 0.28852, 0.27666, 0.98521, 0.055959, 0.26941, 1.0, 0.14021, 0.055787, 0.20527, 0.16463, 0.17834, 0.20875 ,
                         0.56082, 0.18167, 0.62551, 0.52862, 0.48393, 0.72624, 0.13184, 0.21529, 0.89338, 0.14021, 1.0, 0.27655, 0.74718, 0.67953, 0.73246, 0.73609 ,
                         0.18181, 0.95003, 0.17439, 0.13355, 0.17737, 0.20237, 0.054544, 0.9366, 0.47424, 0.055787, 0.27655, 1.0, 0.23915, 0.15532, 0.69917, 0.23277,
                         0.46315, 0.16674, 0.65134, 0.36535, 0.35808, 0.88438, 0.2296, 0.18085, 0.74206, 0.20527, 0.74718, 0.23915, 1.0, 0.6374, 0.54583, 0.65442 ,
                         0.85656, 0.16974, 0.62532, 0.60835, 0.67623, 0.67998, 0.1059, 0.21809, 0.64367, 0.16463, 0.67953, 0.15532, 0.6374, 1.0, 0.47759, 0.71153 ,
                         0.53365, 0.56781, 0.60051, 0.42408, 0.42194, 0.61061, 0.16791, 0.56624, 0.90008, 0.17834, 0.73246, 0.69917, 0.54583, 0.47759, 1.0, 0.44484 ,
                         0.51922, 0.17373, 0.38451, 0.44136, 0.4356, 0.51864, 0.21016, 0.21682, 0.63933, 0.20875, 0.73609, 0.23277, 0.65442, 0.71153, 0.44484, 1.0 
                    };
                auto m1 = testutils::convert1dmatrix(matrix);
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

                auto m4 = get16x16Matrix();
                auto m4check = m4.IsPositiveDefinite();
                ASSERT_TRUE(m4check);
            }

            void matinv_tests::qr_tests()
            {
                auto m1 = Matrix(3, 3);

                m1(0, 0) = 12.0;
                m1(0, 1) = 6.0;
                m1(0, 2) = -4.0;
                m1(1, 0) = -51.0;
                m1(1, 1) = 167.0;
                m1(1, 2) = 24.0;
                m1(2, 0) = 4.0;
                m1(2, 1) = -68.0;
                m1(2, 2) = -41.0;

                std::vector<Matrix> qr = m1.QR(m1);

                Matrix q = qr[0];

                EXPECT_NEAR(q(0, 0), 0.857143, 0.001);
            }
        }
    }
}
