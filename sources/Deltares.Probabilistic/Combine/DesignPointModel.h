// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of Streams.
//
// Streams is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// All names, logos, and references to "Deltares" are registered trademarks of
// Stichting Deltares and remain full property of Stichting Deltares at all times.
// All rights reserved.
//
#pragma once

#include <map>

#include "../Reliability/DesignPoint.h"
#include "../Model/ModelSample.h"

namespace Deltares
{
    namespace Reliability
    {
        /**
         * \brief Represents a linearized model defined by a design point
         */
        class DesignPointModel
        {
        public:
            /**
             * \brief If ignored, the calculation result will be nan when it would be positive (before inversion)
             */
            bool ignored = false;

            /**
             * \brief If inverted, the calculation result is inverted
             */
            bool inverted = false;

            /**
             * \brief design point which is linearized
             */
            std::shared_ptr<DesignPoint> designPoint;

            /**
             * \brief Calculates a value which is positive when the sample would not fail according to the design point, otherwise negative
             * \param sample Sample providing values for the calculation and in which the calculation result is stored
             * \remark The sample must be defined in u-space
             */
            void calculate(std::shared_ptr<ModelSample> sample);

            /**
             * \brief Gets the length in the direction of a sample where the calculation result would be 0
             * \param sample Sample indicating direction
             * \return Length
             */
            double getDirectionBeta(std::shared_ptr<ModelSample> sample);

            /**
             * \brief Assigns a number of stochasts
             * \param normalizedStochasts The stochasts which correspond to the values in a sample to be calculated
             * \param stochastsMap Mapping of normalized stochasts to stochasts in the design point
             * \param designPointsMap Mapping of normalized stochasts to the design point they originate from
             */
            void setParameters(std::vector<std::shared_ptr<Statistics::Stochast>>& normalizedStochasts, std::map<std::shared_ptr<Statistics::Stochast>, std::shared_ptr<Statistics::Stochast>> stochastsMap, std::map<std::shared_ptr<Statistics::Stochast>, std::shared_ptr<Reliability::DesignPoint>> designPointsMap);

            /**
             * \brief Indicates whether the design point has a non-zero alpha value for a stochast
             * \param stochast Stochast
             * \return Indication
             */
            bool isVarying(std::shared_ptr<Statistics::Stochast> stochast);
        private:
            std::vector<double> alphas;
        };
    }
}

