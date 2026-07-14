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
#pragma once

#include <cmath>

namespace Deltares::Numeric
{
    /**
     * \brief Calculates mean and standard deviation without storing all values
     */
    class StatisticsCalculator
    {
    public:
        StatisticsCalculator() = default;

        /**
         * \brief Adds a value
         * \param value The value
         * \param weight optional weight
         */
        void addValue(double value, double weight = 1.0)
        {
            if (weight == 0.0)
            {
                return;
            }

            sumWeights += weight;
            sumSquaredWeights += weight * weight;

            // Welford algorithm

            if (initial)
            {
                mean = value;
                sumSquaredDeviations = 0.0;
                initial = false;
            }
            else
            {
                double delta = value - mean;
                mean += delta * weight / sumWeights;
                sumSquaredDeviations += weight * delta * (value - mean);
            }
        }

        /**
         * \brief Gets the mean value
         * \return Mean value
         */
        double getMean() const
        {
            return mean;
        }

        /**
         * \brief Gets the sample standard deviation (division by n-1)
         * \return Sample standard deviation
         */
        double getSampleStandardDeviation() const
        {
            return sqrt(sumSquaredDeviations / (sumWeights - sumSquaredWeights / sumWeights));
        }

        /**
         * \brief Gets the standard deviation (division by n)
         * \return Standard deviation
         */
        double getStandardDeviation() const
        {
            return sqrt(sumSquaredDeviations / sumWeights);
        }

        /**
         * \brief Gets sum of weight (equal to number of entries if no weights specified)
         * \return sum of weights
         */
        double getCount() const
        {
            return sumWeights;
        }

    private:
        bool initial = true;
        double mean = std::nan("");
        double sumWeights = 0;
        double sumSquaredWeights = 0;
        double sumSquaredDeviations = std::nan("");
    };

}

