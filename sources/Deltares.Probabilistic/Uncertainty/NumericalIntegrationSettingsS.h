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
#include "../Reliability/StochastSettingsSet.h"
#include <memory>

namespace Deltares::Uncertainty
{
    class NumericalIntegrationSettingsS
    {
    public:
        /**
         * \brief Indicates whether correlations should be calculated
         */
        bool CalculateCorrelations = false;

        /**
         * \brief Indicates whether correlations with input values should be calculated
         */
        bool CalculateInputCorrelations = false;

        /**
         * \brief Settings for individual stochastic variables, such as the start value
         */
        std::shared_ptr<Reliability::StochastSettingsSet> StochastSet = std::make_shared<Reliability::StochastSettingsSet>();

        /**
         * \brief Quantiles which should be calculated
         */
        std::vector<std::shared_ptr<Statistics::ProbabilityValue>> RequestedQuantiles;

        /**
         * \brief Settings for performing model runs
         */
        std::shared_ptr<Models::RunSettings> RunSettings = std::make_shared<Models::RunSettings>();

        /**
         * \brief Indicates whether the settings have valid values
         * \return Indication
         */
        bool isValid()
        {
            return this->RunSettings->isValid();
        }
    };
}