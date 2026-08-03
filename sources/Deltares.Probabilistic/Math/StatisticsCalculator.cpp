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
#include "StatisticsCalculator.h"

#include "../Utils/ProbabilisticLibraryException.h"

namespace Deltares::Numeric
{
    void StatisticsCalculator::addValue(double value, double weight)
    {
        if (weight < 0)
        {
            throw Reliability::ProbabilisticLibraryException("Negative weight not supported");
        }

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
     * \brief Gets the sample standard deviation (division by n-1)
     * \return Sample standard deviation
     */
    double StatisticsCalculator::getSampleStandardDeviation() const
    {
        return sqrt(sumSquaredDeviations / (sumWeights - sumSquaredWeights / sumWeights));
    }

    /**
     * \brief Gets the standard deviation (division by n)
     * \return Standard deviation
     */
    double StatisticsCalculator::getStandardDeviation() const
    {
        return sqrt(sumSquaredDeviations / sumWeights);
    }

}

