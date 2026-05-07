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

#include "DiscreteValue.h"
#include "HistogramValue.h"
#include "FragilityValue.h"
#include "../Utils/probLibException.h"
#include "DistributionPropertyType.h"

#include <vector>
#include <memory>

#include "ContributingStochast.h"

namespace Deltares::Statistics
{
    /**
     * \brief Defines the stochastic parameters of a stochastic variable
     * \remark Mean and standard deviation are not stochastic parameters, but are derived based on these stochastic parameters and a distribution type
     */
    class StochastProperties
    {

    public:
        /**
         * \brief Location, for normal distribution equal to the mean
         */
        double Location = 0;

        /**
         * \brief Scale, for normal distribution equal to the standard deviation
         */
        double Scale = 0;

        /**
         * \brief Minimum allowed value
         */
        double Minimum = - std::numeric_limits<double>::infinity();

        /**
         * \brief Maximum allowed value
         */
        double Maximum = std::numeric_limits<double>::infinity();

        /**
         * \brief Value by which the stochast is shifted
         */
        double Shift = 0;

        /**
         * \brief Secondary value by which the stochast is shifted
         */
        double ShiftB = 0;

        /**
         * \brief Shape, defines the shape of the distribution
         */
        double Shape = 1;

        /**
         * \brief Secondary shape, defines the shape of the distribution
         */
        double ShapeB = 1;

        /**
         * \brief Number of observations 
         */
        int Observations = 2;

        /**
         * \brief Collection of discrete values
         */
        std::vector<std::shared_ptr<DiscreteValue>> DiscreteValues;

        /**
         * \brief Collection of bins in a histogram distribution
         */
        std::vector<std::shared_ptr<HistogramValue>> HistogramValues;

        /**
         * \brief Collection of CDF values
         */
        std::vector<std::shared_ptr<FragilityValue>> FragilityValues;

        /**
         * \brief Collection of contributing stochasts
         */
        std::vector<std::shared_ptr<ContributingStochast>> ContributingStochasts;

        /**
         * \brief General method to apply a value to one of the stochastic parameters
         * \param property Stochastic parameter type
         * \param value Value to be applied
         */
        void applyValue(DistributionPropertyType property, double value);

        /**
         * \brief Makes a copy of the stochastic properties
         * \remark Discrete values, histogram values and CDF values are omitted
         * \return Copy
         */
        std::shared_ptr<StochastProperties> clone() const;

        void copyFrom(const std::shared_ptr<StochastProperties>& source);

        /**
         * \brief Sorts the fragility values on the x value
         */
        void sortFragilityValuesOnX();

        /**
         * \brief Sorts the fragility values on the reliability
         */
        void sortFragilityValuesOnReliability();

        /**
         * \brief Indicates whether InitializeForRun should be invoked before performing other methods
         * \return Indication
         */
        bool dirty = false;

        /**
         * \brief Indicates whether InitializeForRun should be invoked before performing other methods
         * \return Indication
         */
        void setDirty();
    };
}
