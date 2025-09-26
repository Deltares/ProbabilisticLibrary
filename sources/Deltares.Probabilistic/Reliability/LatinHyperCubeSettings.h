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

namespace Deltares
{
    namespace Reliability
    {
        class LatinHyperCubeSettings
        {
        public:
            /**
             * \brief The minimum number of samples to be examined
             */
            int MinimumSamples = 1000;

            /**
             * \brief Method type how the design point (alpha values) is calculated
             */
            DesignPointMethod designPointMethod = DesignPointMethod::CenterOfGravity;

            /**
             * \brief settings for the random number generator
             */
            std::shared_ptr<Deltares::Models::RandomSettings> randomSettings = std::make_shared<Deltares::Models::RandomSettings>();

            /**
             * \brief Settings for individual stochastic variables, such as the start value
             */
            std::shared_ptr<StochastSettingsSet> StochastSet = std::make_shared<StochastSettingsSet>();

            /**
             * \brief Settings for performing model runs
             */
            std::shared_ptr<Models::RunSettings> RunSettings = std::make_shared<Models::RunSettings>();

            /**
             * \brief Reports whether the settings have valid values
             * \param report Report in which the validity is reported
             */
            void validate(Logging::ValidationReport& report) const
            {
                Logging::ValidationSupport::checkMinimum(report, 1, MinimumSamples, "minimum samples");
                runSettings->validate(report);
            }

            /**
             * \brief Settings for performing model runs
             */
            std::shared_ptr<Models::RunSettings> runSettings = std::make_shared<Models::RunSettings>();
        };
    }
}
