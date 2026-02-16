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
#include <gtest/gtest.h>
#include "testMatrixMultiplication.h"
#include "../../Deltares.Probabilistic/Math/matrix.h"

namespace Deltares
{
    namespace Probabilistic
    {
        namespace Test
        {
            void matmul_tests::all_matmul_tests() const
            {
                matmul_test1();
                matmul_test2();
                matvec_test();
            }

            void matmul_tests::matmul_test1() const
            {
                auto m1 = Numeric::Matrix(3, 3);
                auto m2 = Numeric::Matrix(3, 2);
                auto m3 = Numeric::Matrix(3, 2);
                m1(0, 0) = 0.2;
                m1(0, 1) = 0.8;
                m1(0, 2) = 1.1;
                m1(1, 0) = 0.0;
                m1(1, 1) = 0.2;
                m1(1, 2) = 0.5;
                m1(2, 0) = 1.8;
                m1(2, 1) = 1.6;
                m1(2, 2) = 1.4;
                m2(0, 0) = 50.0;
                m2(0, 1) = 60.0;
                m2(1, 0) = 83.0;
                m2(1, 1) = 55.0;
                m2(2, 0) = 32.0;
                m2(2, 1) = 45.0;
                m3(0, 0) = 111.6;
                m3(0, 1) = 105.5;
                m3(1, 0) = 32.6;
                m3(1, 1) = 33.5;
                m3(2, 0) = 267.6;
                m3(2, 1) = 259.0;
                auto m4 = m1.matmul(m2);
                for (size_t i = 0; i < 3; i++)
                {
                    for (size_t j = 0; j < 2; j++)
                    {
                        EXPECT_NEAR(m3(i, j), m4(i, j), margin);
                    }
                }
            }

            void matmul_tests::matmul_test2() const
            {
                auto m1 = Numeric::Matrix(3, 3);
                auto m2 = Numeric::Matrix(5, 5);
                int success = 1;
                try
                {
                    auto m3 = m1.matmul(m2);
                    success = 0;
                }
                catch (const std::exception& e)
                {
                    auto msg = e.what();
                    EXPECT_EQ(msg[0], 'd'); // error message starts with "dimension..."
                }
                EXPECT_EQ(success, 1);
            }

            void matmul_tests::matvec_test() const
            {
                auto m = Numeric::Matrix(2, 2); // identity matrix
                m(0, 0) = 1.0;
                m(1, 1) = 1.0;
                m(0, 1) = 0.0;
                m(1, 0) = 0.0;
                auto v = Numeric::vector1D({ std::numbers::pi, std::numbers::e });
                auto r = m.matvec(v); // r = v as m is the identity matrix
                for (size_t i = 0; i < v.size(); i++)
                {
                    EXPECT_NEAR(r(i), v(i), margin);
                }
            }
        }
    }
}
