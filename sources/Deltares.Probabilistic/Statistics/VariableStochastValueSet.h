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
#include <vector>

#include "DistributionType.h"
#include "VariableStochastValue.h"
#include "Distributions/Distribution.h"

namespace Deltares
{
    namespace Statistics
    {
        /**
          * \brief Produces an interpolated stochast based on a number of conditional values
          */
        class VariableStochastValuesSet
        {
        public:
            /**
              * \brief Collection of conditional values
              */
            std::vector<std::shared_ptr<VariableStochastValue>> StochastValues;

            void getMergedStochast(std::shared_ptr<StochastProperties> defaultStochast,
                                   std::shared_ptr<Distribution> distribution,
                                   std::shared_ptr<VariableStochastValue> value,
                                   std::shared_ptr<StochastProperties>& source);
            /**
              * \brief Prepares this class for running
              */
            void initializeForRun(std::shared_ptr<StochastProperties> defaultStochast, DistributionType distributionType, bool truncated, bool inverted);

            /**
              * \brief Gets the interpolated stochast at a given x-value
              * \param x The value at which the interpolated stochast is generated
              * \return Interpolated stochast
              */
            std::shared_ptr<StochastProperties> getInterpolatedStochast(double x);

            /**
              * \brief Indicates whether an interpolated stochast can lead to different x values
              * \param distributionType distribution type
              * \param defaultStochast default stochast
              * \return Indication
              */
            bool isVarying(DistributionType distributionType, std::shared_ptr<StochastProperties> defaultStochast = nullptr);

            /**
              * \brief Indicates whether the stochast value set is valid
              * \return Indication
              */
            bool isValid(DistributionType distributionType, bool truncated, bool inverted);

            /**
              * \brief Makes a deep copy of a source
              * \param source Source
              * \return Copy
              */
            void copyFrom(std::shared_ptr<VariableStochastValuesSet> source);
        private:
            std::vector<double> xValues;
            std::vector<double> locations;
            std::vector<double> scales;
            std::vector<double> minimums;
            std::vector<double> maximums;
            std::vector<double> shapes;
            std::vector<double> shapesB;
            std::vector<double> shifts;
            std::vector<double> shiftsB;
            std::vector<double> observations;
        };
    }
}

