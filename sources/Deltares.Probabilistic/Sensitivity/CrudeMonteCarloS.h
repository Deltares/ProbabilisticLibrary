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
#include "CrudeMonteCarloSettingsS.h"
#include "SensitivityMethod.h"
#include "SensitivityResult.h"

namespace Deltares
{
    namespace Sensitivity
    {
        /**
         * \brief Calculates the sensitivity using the Crude Monte Carlo algorithm
         */
        class CrudeMonteCarloS : public SensitivityMethod
        {
        public:
            /**
             * \brief Settings for this algorithm
             */
            std::shared_ptr<CrudeMonteCarloSettingsS> Settings = std::make_shared<CrudeMonteCarloSettingsS>();

            /**
             * \brief Gets the sensitivity
             * \param modelRunner The model for which the sensitivity is calculated
             * \return The sensitivity in the form of a stochastic variable
             */
            std::shared_ptr<Sensitivity::SensitivityResult> getSensitivityStochast(std::shared_ptr<Models::ModelRunner> modelRunner) override;
        };
    }
}

