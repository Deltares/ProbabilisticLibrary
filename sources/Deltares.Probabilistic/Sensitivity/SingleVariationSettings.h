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

#include "../Model/RunSettings.h"
#include <memory>

namespace Deltares::Sensitivity
{
    /**
     * \brief Settings for the single variation sensitivity method
     */
    class SingleVariationSettings
    {
    public:
        /**
         * \brief Low value fraction
         */
        double LowValue = 0.05;

        /**
         * \brief High value fraction
         */
        double HighValue = 0.95;

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
            Logging::ValidationSupport::checkMinimumNonInclusive(report, 0, LowValue, "low value");
            Logging::ValidationSupport::checkMaximumNonInclusive(report, 1, LowValue, "low value");
            Logging::ValidationSupport::checkMinimumNonInclusive(report, 0, HighValue, "high value");
            Logging::ValidationSupport::checkMaximumNonInclusive(report, 1, HighValue, "high value");

            RunSettings->validate(report);
        }
    };
}

