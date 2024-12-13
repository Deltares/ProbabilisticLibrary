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
#include "NumericalIntegrationSettingsS.h"
#include "SensitivityMethod.h"

namespace Deltares
{
    namespace Sensitivity
    {
        /**
         * \brief Calculates the sensitivity using the numerical integration algorithm
         */
        class NumericalIntegrationS : public SensitivityMethod
        {
        public:
            /**
             * \brief Settings for this algorithm
             */
            std::shared_ptr<NumericalIntegrationSettingsS> Settings = std::make_shared<NumericalIntegrationSettingsS>();

            /**
             * \brief Gets the sensitivity
             * \param modelRunner The model for which the sensitivity is calculated
             * \return The sensitivity in the form of a stochastic variable
             */
            std::shared_ptr<Statistics::Stochast> getSensitivityStochast(std::shared_ptr<Models::ModelRunner> modelRunner) override;
        private:
            std::vector<std::shared_ptr<Numeric::WeightedValue>> collectSamples(std::shared_ptr<Models::ModelRunner> modelRunner, int stochastIndex, std::shared_ptr<Models::Sample> parentSample, double density, int nSamples, bool registerSamplesForCorrelation);
        };
    }
}

