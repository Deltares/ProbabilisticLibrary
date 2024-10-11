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

#include "StartPointCalculatorSettings.h"
#include "StochastSettingsSet.h"
#include "../Model/RunSettings.h"

namespace Deltares
{
    namespace Reliability
    {
        /**
         * \brief Settings for Cobyla Reliability algorithm
         */
        class CobylaReliabilitySettings
        {
        public:

            /**
             * \brief Maximum number of guessed design points in one FORM loop
             */
            int MaximumIterations = 50;

            /**
             * \brief The FORM loop stops when the next guessed design point is less than this value away from the current guessed design point
             */
            double EpsilonBeta = 0.01;

            /**
             * \brief Settings for performing model runs
             */
            std::shared_ptr<Models::RunSettings> RunSettings = std::make_shared<Models::RunSettings>();

            /**
             * \brief Settings for calculating the initial guessed design point
             */
            std::shared_ptr<StartPointCalculatorSettings> StartPointSettings = std::make_shared<StartPointCalculatorSettings>();

            /**
             * \brief Settings for individual stochastic variable, such as the start value
             */
            std::shared_ptr<StochastSettingsSet> StochastSet = std::make_shared<StochastSettingsSet>();

            /**
             * \brief Indicates whether these settings have valid values
             * \return Indication
             */
            bool isValid()
            {
                return
                    MaximumIterations >= 1 &&
                    EpsilonBeta >= 0.0001 &&
                    StartPointSettings->isValid() &&
                    RunSettings->isValid();
            }
        };
    }
}

