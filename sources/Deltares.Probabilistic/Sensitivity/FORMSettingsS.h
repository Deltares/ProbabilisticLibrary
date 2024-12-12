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
#include "../Model/GradientSettings.h"
#include "../Statistics/StandardNormal.h"
#include <memory>

namespace Deltares
{
    namespace Sensitivity
    {
        class FORMSettingsS
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
             * \brief Minimum u value from where to start calculation
             */
            double Minimum = -Statistics::StandardNormal::UMax;

            /**
             * \brief Maximum u value where to stop calculation
             */
            double Maximum = Statistics::StandardNormal::UMax;

            /**
             * \brief Step size
             */
            double StepSize = 0.5;

            /**
              * \brief Settings for calculating the gradient at a stochast point
              */
            std::shared_ptr<Models::GradientSettings> GradientSettings = std::make_shared<Models::GradientSettings>();

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
                return this->GradientSettings->isValid();
            }
        };
    }
}

