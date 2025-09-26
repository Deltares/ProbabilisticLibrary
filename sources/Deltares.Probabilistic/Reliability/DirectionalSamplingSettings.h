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

#include "../Model/RandomSettings.h"
#include "DesignPointBuilder.h"
#include "DirectionReliabilitySettings.h"
#include "StochastSettingsSet.h"

namespace Deltares
{
    namespace Reliability
    {
        /**
         * \brief Settings for directional sampling
         */
        class DirectionalSamplingSettings
        {
        public:
            /**
             * \brief The minimum number of directions to be examined
             */
            int MinimumDirections = 1000;

            /**
             * \brief The maximum number of directions to be examined
             */
            int MaximumDirections = 10000;

            /**
             * \brief The directional sampling algorithm stops when the calculated variation coefficient is less than this value
             */
            double VariationCoefficient = 0.05;

            /**
             * \brief Method type how the design point (alpha values) is calculated
             */
            DesignPointMethod designPointMethod = DesignPointMethod::CenterOfGravity;

            /**
             * \brief Settings for calculating one direction
             */
            std::shared_ptr<DirectionReliabilitySettings> DirectionSettings = std::make_shared<DirectionReliabilitySettings>();

            /**
             * \brief Settings for performing model runs
             */
            std::shared_ptr<Models::RunSettings> runSettings = std::make_shared<Models::RunSettings>();

            /**
             * \brief Settings for generating random values
             */
            std::shared_ptr<Models::RandomSettings> randomSettings = std::make_shared<Models::RandomSettings>();

            /**
             * \brief Settings for individual stochastic variables
             */
            std::shared_ptr<StochastSettingsSet> StochastSet = std::make_shared<StochastSettingsSet>();

            /**
             * \brief Reports whether the settings have valid values
             * \param report Report in which the validity is reported
             */
            void validate(Logging::ValidationReport& report) const
            {
                Logging::ValidationSupport::checkMinimum(report, 1, MinimumDirections, "minimum directions");
                Logging::ValidationSupport::checkMinimum(report, MinimumDirections, MaximumDirections, "maximum directions");
                Logging::ValidationSupport::checkMinimum(report, 0, VariationCoefficient, "variation coefficient");

                runSettings->validate(report);
            }
        };
    }
}


