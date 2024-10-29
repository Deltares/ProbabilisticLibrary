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

#include <vector>
#include "GradientSettings.h"
#include "ModelRunner.h"

namespace Deltares
{
    namespace Models
    {
        /**
         * \brief Calculates the gradient of a model at a given location
         */
        class GradientCalculator
        {
        public:
            GradientCalculator() = default;

            /**
             * \brief Settings of the gradient calculation
             */
            std::shared_ptr<GradientSettings> Settings = std::make_shared<GradientSettings>();

            /**
             * \brief Calculates the gradient
             * \param modelRunner The model for which the gradient is calculated
             * \param sample The location at which the gradient is calculated
             * \return Gradient, vector with size of sample size
             * \remarks The z-value of the given sample is calculated too and stored in sample->Z
             */
            std::vector<double> getGradient(std::shared_ptr<Models::ModelRunner> modelRunner, std::shared_ptr<Sample> sample);
        };
    }
}



