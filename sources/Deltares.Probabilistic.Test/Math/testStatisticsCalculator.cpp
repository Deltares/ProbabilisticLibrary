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
#include <complex>
#include <gtest/gtest.h>
#include "testStatisticsCalculator.h"
#include "../../Deltares.Probabilistic/Math/NumericSupport.h"
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
        std::vector<double> values = { 1.2, 4.5, 8.2, 6.2, 4.5 };

        double sum = 0;
        for (size_t i = 0; i < values.size(); i++)
        {
            sum += values[i];
        }

        double expected_mean = sum / values.size();

        double sum_diffs = 0;
        for (size_t i = 0; i < values.size(); i++)
        {
            double diff = values[i] - expected_mean;
            sum_diffs += diff * diff;
        }

        double expected_std_dev = std::sqrt(sum_diffs / values.size());
        double expected_sample_std_dev = std::sqrt(sum_diffs / (values.size() - 1));

        // test

        constexpr double margin = 0.001;
        auto calc = Numeric::StatisticsCalculator();

        for (size_t i = 0; i < values.size(); i++)
        {
            calc.addValue(values[i]);
        }

        ASSERT_NEAR(calc.getMean(), expected_mean, margin);
        ASSERT_NEAR(calc.getSampleStandardDeviation(), expected_sample_std_dev, margin);
        ASSERT_NEAR(calc.getStandardDeviation(), expected_std_dev, margin);
    }

    void testStatisticsCalculator::statisticsCalculatorWeightedTest()
    {
        std::vector<double> values = {1.2, 4.5, 8.2, 6.2, 4.5};
        std::vector<double> weights = { 0.5, 0.8, 1.3, 0.1, 1.7 };

        double sum = 0;
        double sum_weights = 0;
        for (size_t i = 0; i < values.size(); i++)
        {
            sum += values[i] * weights[i];
            sum_weights += weights[i];
        }

        double expected_mean = sum / sum_weights;

        double sum_diffs = 0;
        double sum_squared_weights = 0;
        for (size_t i = 0; i < values.size(); i++)
        {
            double diff = values[i] - expected_mean;
            sum_diffs += diff * diff * weights[i];
            sum_squared_weights += weights[i] * weights[i];
        }

        double expected_std_dev = std::sqrt(sum_diffs / sum_weights);
        double expected_sample_std_dev = std::sqrt(sum_diffs / (sum_weights - sum_squared_weights / sum_weights));

        // test

        constexpr double margin = 0.001;
        auto calc = Numeric::StatisticsCalculator();

        for (size_t i = 0; i < values.size(); i++)
        {
            calc.addValue(values[i], weights[i]);
        }

        ASSERT_NEAR(calc.getMean(), expected_mean, margin);
        ASSERT_NEAR(calc.getSampleStandardDeviation(), expected_sample_std_dev, margin);
        ASSERT_NEAR(calc.getStandardDeviation(), expected_std_dev, margin);
    }
}

