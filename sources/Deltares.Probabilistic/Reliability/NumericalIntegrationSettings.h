// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of the Probabilistic Library.
//
// The Probabilistic Library is free software: you can redistribute it and/or modify
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
             * \brief Indicates whether these settings have valid values
             * \return Indication
             */
            bool isValid() const
            {
                return
                    this->isStochastSetValid() &&
                    runSettings->isValid();
            }

        private:
            /**
             * \brief Indicates whether all stochast settings have valid values
             * \return True if all valid, false otherwise
             */
            bool isStochastSetValid() const
            {
                for (size_t i = 0; i < this->StochastSet->getVaryingStochastCount(); i++)
                {
                    const std::shared_ptr<StochastSettings> stochastSettings = this->StochastSet->VaryingStochastSettings[i];
                    const bool valid =
                        stochastSettings->MinValue < stochastSettings->MaxValue &&
                        stochastSettings->Intervals >= 1 &&
                        stochastSettings->MinValue >= -Statistics::StandardNormal::UMax && stochastSettings->MinValue <= Statistics::StandardNormal::UMax &&
                        stochastSettings->MaxValue >= -Statistics::StandardNormal::UMax && stochastSettings->MaxValue <= Statistics::StandardNormal::UMax;

                    if (!valid)
                    {
                        return false;
                    }
                }

                return true;
            }
        };
    }
}

