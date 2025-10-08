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
#include "DesignPointBuilder.h"

namespace Deltares
{
    namespace Reliability
    {
        /**
         * \brief Settings for numerical integration
         */
        class NumericalIntegrationSettings
        {
        public:
            /**
             * \brief Method type how the design point (alpha values) is calculated
             */
            DesignPointMethod designPointMethod = DesignPointMethod::CenterOfGravity;

            /**
             * \brief Settings per stochastic variable, contains (among others) the center value and multiplication factor used to shift samples in the importance sampling algorithm
             */
            std::shared_ptr<StochastSettingsSet> StochastSet = std::make_shared<StochastSettingsSet>();

            /**
             * \brief Settings for performing model runs
             */
            std::shared_ptr<Models::RunSettings> runSettings = std::make_shared<Models::RunSettings>();

            /**
             * \brief Reports whether the settings have valid values
             * \param report Report in which the validity is reported
             */
            void validate(Logging::ValidationReport& report) const
            {
                validateStochastSet(report);
                runSettings->validate(report);
            }

        private:
            /**
             * \brief Indicates whether all stochast settings have valid values
             * \return True if all valid, false otherwise
             */
            void validateStochastSet(Logging::ValidationReport& report) const
            {
                for (size_t i = 0; i < this->StochastSet->getVaryingStochastCount(); i++)
                {
                    const std::shared_ptr<StochastSettings> stochastSettings = this->StochastSet->VaryingStochastSettings[i];

                    Logging::ValidationSupport::checkMinimumInt(report, 1, stochastSettings->Intervals, "intervals");
                    Logging::ValidationSupport::checkMinimum(report, -Statistics::StandardNormal::UMax, stochastSettings->MinValue, "min value");
                    Logging::ValidationSupport::checkMinimum(report, stochastSettings->MinValue, stochastSettings->MaxValue, "max value");
                    Logging::ValidationSupport::checkMaximum(report, Statistics::StandardNormal::UMax, stochastSettings->MaxValue, "max value");
                }
            }
        };
    }
}

