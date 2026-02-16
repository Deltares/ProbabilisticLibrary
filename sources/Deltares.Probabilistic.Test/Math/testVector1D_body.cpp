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
#include <gtest/gtest.h>
#include <cmath>
#include "../../Deltares.Probabilistic/Math/vector1D.h"
#include "testVector1D.h"

namespace Deltares
{
    namespace Probabilistic
    {
        namespace Test
        {

            void vector1D_tests::allVector1Dtests() const
            {
                vector_sum_test();
                vector_normalize_test();
                minmax_test();
            }

            void vector1D_tests::vector_sum_test() const
            {
                auto v1 = Numeric::vector1D(2);
                auto v2 = Numeric::vector1D(2);
                v1.assign(2.0);
                v2.assign(3.0);
                auto lv1 = v1.sumOfSquares();
                auto lv2 = v2.sumOfSquares();
                auto inner = v2.sumOfInner(v1);
                EXPECT_DOUBLE_EQ(lv1, 8.0);
                EXPECT_DOUBLE_EQ(lv2, 18.0);
                EXPECT_DOUBLE_EQ(inner, 12.0);
            }

            void vector1D_tests::vector_normalize_test() const
            {
                auto v = Numeric::vector1D({ 0.1, 0.2, 0.3 });
                v.normalize();
                EXPECT_NEAR(v(0), 0.267261, 2e-6);
                EXPECT_NEAR(v(1), 0.534522, 2e-6);
                EXPECT_NEAR(v(2), 0.801784, 2e-6);
                auto s = v.sumOfSquares();
                EXPECT_NEAR(s, 1.0, 1e-10);
            }

            void vector1D_tests::minmax_test() const
            {
                auto v = Numeric::vector1D({ 0.1, 0.2, 0.3 });
                double max = v.maxval();
                double min = v.minval();
                EXPECT_DOUBLE_EQ(v(0), min);
                EXPECT_DOUBLE_EQ(v(2), max);

                auto v2 = Numeric::vector1D();
                double y = v2.maxval();
                EXPECT_TRUE(std::isnan(y));
            }
        }
    }
}
