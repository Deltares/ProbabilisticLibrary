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
#include "testStatisticsCalculator.h"
#include "../../Deltares.Probabilistic/Math/StatisticsCalculator.h"

namespace Deltares::Probabilistic::Test
{
    void testStatisticsCalculator::allStatisticsCalculatorTests()
    {
        statisticsCalculatorTest();
        statisticsCalculatorWeightedTest();
    }

    void testStatisticsCalculator::statisticsCalculatorTest()
    {
        constexpr double margin = 0.001;
        auto calc = Numeric::StatisticsCalculator();

        calc.addValue(1.2);
        calc.addValue(4.5);
        calc.addValue(8.2);
        calc.addValue(6.2);
        calc.addValue(4.5);

        ASSERT_NEAR(calc.getMean(), 4.92, margin);
        ASSERT_NEAR(calc.getSampleStandardDeviation(), 2.578, margin);
        ASSERT_NEAR(calc.getStandardDeviation(), 2.306, margin);
    }

    void testStatisticsCalculator::statisticsCalculatorWeightedTest()
    {
        constexpr double margin = 0.001;
        auto calc = Numeric::StatisticsCalculator();

        calc.addValue(1.2, 0.5);
        calc.addValue(4.5, 0.8);
        calc.addValue(8.2, 1.3);
        calc.addValue(6.2, 0.1);
        calc.addValue(4.5, 1.7);

        ASSERT_NEAR(calc.getMean(), 5.2568, margin);
        ASSERT_NEAR(calc.getSampleStandardDeviation(), 2.580, margin);
        ASSERT_NEAR(calc.getStandardDeviation(), 2.186, margin);
    }
}

