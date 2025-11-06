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

#include <functional>
#include "Distribution.h"
#include "../StochastProperties.h"
#include "../../Math/WeightedValue.h"

namespace Deltares::Statistics
{
    /**
     * \brief Supporting class for distributions
     */
    class DistributionSupport
    {
    public:
        /**
         * \brief Adapts the stochast properties by specifying an x-value for a certain u-value
         * \param distribution Distribution to be used
         * \param stochast Stochast properties
         * \param x The specified x-value
         * \param u The u-value to which the x-value belongs
         * \param constantType Indicates whether the deviation or variation should be kept constant
         */
        static void setXAtUByIteration(Distribution& distribution, StochastProperties& stochast, double x, double u, ConstantParameterType constantType);

        /**
         * \brief Gets the mean of a stochast by iteration
         * \param distribution Distribution to be used
         * \param stochast Stochast properties
         * \remarks To be used when analytical solution is not possible
         */
        static double getMeanByIteration(Distribution& distribution, StochastProperties& stochast);

        /**
         * \brief Gets the deviation of a stochast by iteration
         * \param distribution Distribution to be used
         * \param stochast Stochast properties
         * \remarks To be used when analytical solution is not possible
         */
        static double getDeviationByIteration(Distribution& distribution, StochastProperties& stochast);

        /**
         * \brief Gets the x-value at a certain u-value by iteration
         * \param distribution Distribution to be used
         * \param stochast Stochast properties
         * \param u The requested u-value
         * \remarks To be used when analytical solution is not possible
         */
        static double getXFromUByIteration(Distribution& distribution, StochastProperties& stochast, double u);

        /**
         * \brief Converts a list of values and weights to weighted values
         * \param values Values
         * \param weights Weights
         * \returns Weighted values
         */
        static std::vector<Numeric::WeightedValue> GetWeightedValues(const std::vector<double>& values, const std::vector<double>& weights);

        /**
         * \brief Gets a list which reflects as much as possible a list with weights
         * \param values List of values
         * \param weights List of weights
         * \return List reflecting list with weights
         */
        static std::vector<double> getExpandedValues(const std::vector<double>& values, const std::vector<double>& weights);

    private:
        static std::vector<double> getValuesForIteration(Distribution& distribution, StochastProperties& stochast);
    };
}

