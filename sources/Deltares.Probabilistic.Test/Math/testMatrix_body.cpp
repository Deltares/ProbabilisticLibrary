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
#include <numbers>

#include "testMatrix.h"
#include "gtest/gtest.h"
#include "../Utils/testutils.h"

using namespace Deltares::Numeric;

namespace Deltares
{
    namespace Probabilistic
    {
        namespace Test
        {
            void matrix_tests::qr_tests()
            {
                auto matrix_values =
                {
                    12.000,  -51.000,    4.000,
                     6.000,  167.000, -68.000,
                    -4.000,   24.000, -41.000,
                    -1.000,    1.000,   0.000,
                     2.000,    0.000,   3.000
                };

                Matrix m1 = testutils::convert1dmatrix(matrix_values, 5, 3, false);

                QR qr = m1.qr_decompose();

                auto expected_q_values =
                {
                    0.846, -0.391,  0.343,  0.082,  0.078,
                    0.423,  0.904, -0.029,  0.026,  0.045,
                   -0.282,  0.170,  0.933, -0.047, -0.137,
                   -0.071,  0.014, -0.001,  0.980, -0.184,
                    0.141, -0.017, -0.106, -0.171, -0.969
                };

                Matrix expected_q = testutils::convert1dmatrix(expected_q_values, 5, 5, false);

                for (size_t i = 0; i < m1.getRowCount(); i++)
                {
                    for (size_t j = 0; j < m1.getColumnCount(); j++)
                    {
                        EXPECT_NEAR(expected_q(i, j), qr.q(i, j), 0.001);
                    }
                }

                auto expected_r_values =
                {
                   14.177,  20.667, -13.402,
                   -0.000, 175.043, -70.080,
                    0.000,   0.000, -35.202,
                   -0.000,  -0.000, -0.000,
                    0.000,   0.000, -0.000
                };

                Matrix expected_r = testutils::convert1dmatrix(expected_r_values, 5, 3, false);

                for (size_t i = 0; i < m1.getRowCount(); i++)
                {
                    for (size_t j = 0; j < m1.getColumnCount(); j++)
                    {
                        EXPECT_NEAR(expected_r(i, j), qr.r(i, j), 0.001);
                    }
                }


                Matrix qr_product = qr.q.matmul(qr.r);

                for (size_t i = 0; i < m1.getRowCount(); i++)
                {
                    for (size_t j = 0; j < m1.getColumnCount(); j++)
                    {
                        EXPECT_NEAR(m1(i, j), qr_product(i, j), 0.001);
                    }
                }
            }
        }
    }
}
