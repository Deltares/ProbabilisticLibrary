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
#include "TestNumericSupport.h"
#include "../../Deltares.Probabilistic/Math/NumericSupport.h"
#include "../../Deltares.Probabilistic/Utils/ProbabilisticLibraryException.h"

namespace Deltares::Probabilistic::Test
{
    using namespace Deltares::Numeric;

    void TestNumericSupport::testLinearInterpolate()
    {
        constexpr double margin = 1e-9;
        auto x = std::vector{ 0.0, 1.0 };
        auto y = std::vector{ 3.0, 4.0 };
        auto result = NumericSupport::interpolate(0.5, x, y);
        EXPECT_NEAR(result, 3.5, margin);

        result = NumericSupport::interpolate(2.0, x, y, true);
        EXPECT_NEAR(result, 5.0, margin);

        result = NumericSupport::interpolate(2.0, x, y);
        EXPECT_NEAR(result, 4.0, margin);
    }

    void TestNumericSupport::testLogLinearInterpolate()
    {
        using enum InterpolationType;
        constexpr double margin = 1e-9;
        auto x = std::vector{ 1.0, 4.0 };
        auto y = std::vector{ 3.0, 4.0 };
        auto result = NumericSupport::interpolate(2.0, x, y, false, Logarithmic);
        EXPECT_NEAR(result, 3.5, margin);

        result = NumericSupport::interpolate(8.0, x, y, true, Logarithmic);
        EXPECT_NEAR(result, 4.5, margin);

        result = NumericSupport::interpolate(8.0, x, y, false, Logarithmic);
        EXPECT_NEAR(result, 4.0, margin);
    }

    void TestNumericSupport::testHarmonicInterpolate()
    {
        using enum InterpolationType;
        constexpr double margin = 1e-9;
        auto x = std::vector{ 1.0, 4.0 };
        auto y = std::vector{ 3.0, 4.0 };
        auto result = NumericSupport::interpolate(2.0, x, y, false, Harmonic);
        EXPECT_NEAR(result, 3.0 + 2.0/3.0, margin);

        result = NumericSupport::interpolate(8.0, x, y, true, Harmonic);
        EXPECT_NEAR(result, 4.0 + 1.0/6.0, margin);

        result = NumericSupport::interpolate(8.0, x, y, false, Harmonic);
        EXPECT_NEAR(result, 4.0, margin);
    }

    void TestNumericSupport::testMinMaxFunctions()
    {
        auto numbers = std::vector { -2.0, 2.0, -3.0, 3.0, 1.0, -1.0, 0.0 };

        auto maxval = NumericSupport::getMaximum(numbers);
        EXPECT_EQ(maxval, 3.0);

        auto minval = NumericSupport::getMinimum(numbers);
        EXPECT_EQ(minval, -3.0);

        auto locmin = NumericSupport::getLocationMinimum(numbers);
        EXPECT_EQ(locmin, 2);

        auto locmax = NumericSupport::getLocationMaximum(numbers);
        EXPECT_EQ(locmax, 3);

        for (auto& x : numbers) { x = std::abs(x); }
        minval = NumericSupport::getMinimum(numbers);
        EXPECT_EQ(minval, 0.0);

        locmax = NumericSupport::getLocationMinimum(numbers);
        EXPECT_EQ(locmax, 6);
    }

    void TestNumericSupport::testGetFraction()
    {
        auto q = NumericSupport::getFraction(3.4, 4.3);
        EXPECT_NEAR(q, 3.4 * 4.3, 1e-9);
    }

    void TestNumericSupport::testLimit()
    {
        auto y = NumericSupport::limit(0.5, 1.0, 2.0);
        EXPECT_EQ(y, 1.0) << "expect value equal to lower limit";

        y = NumericSupport::limit(2.5, 1.0, 2.0);
        EXPECT_EQ(y, 2.0) << "expect value equal to upper limit";

        y = NumericSupport::limit(2.5, 2.0, 1.0);
        EXPECT_EQ(y, 2.0) << "expect value equal to maximum of the boundary values";
    }

    void TestNumericSupport::testAreEqual()
    {
        constexpr double x1 = 1e30;
        constexpr double x2 = 1e30 + 1e17; // relative difference is 1e-13; below threshold
        bool equal = NumericSupport::areEqual(x1, x2);
        EXPECT_TRUE(equal);

        constexpr double x3 = 1e30;
        constexpr double x4 = 1e30 + 1e19; // relative difference is 1e-11; above threshold
        equal = NumericSupport::areEqual(x3, x4);
        EXPECT_FALSE(equal);

        constexpr double x5 = 1.0;
        constexpr double x6 = 1.0 + 1e-13; // relative difference is 1e-13; below threshold
        equal = NumericSupport::areEqual(x5, x6);
        EXPECT_TRUE(equal);

        constexpr double x7 = 1.0;
        constexpr double x8 = 1.0 + 1e-11; // relative difference is 1e-11; above threshold
        equal = NumericSupport::areEqual(x7, x8);
        EXPECT_FALSE(equal);
    }
}

