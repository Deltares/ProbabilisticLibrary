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
#include <vector>

#include "SensitivityMethod.h"
#include "SettingsS.h"
#include "../Statistics/Stochast.h"
#include "../Statistics/CorrelationMatrix.h"

namespace Deltares
{
    namespace Sensitivity
    {
        /**
         * \brief Combines a model, stochastic variables and calculation settings, can perform a calculation and holds results
         */
        class SensitivityProject
        {
        public:
            /**
             * \brief Collection of all stochastic variables
             */
            std::vector<std::shared_ptr<Statistics::Stochast>> stochasts;

            /**
             * \brief Defines correlations between stochastic variables
             */
            std::shared_ptr<Statistics::CorrelationMatrix> correlationMatrix = nullptr;

            /**
             * \brief Method which performs a reliability calculation
             */
            std::shared_ptr<Sensitivity::SensitivityMethod> sensitivityMethod = nullptr;

            /**
             * \brief Deterministic model which calculates a z-value based on input values
             */
            std::shared_ptr<Models::ZModel> model = nullptr;

            /**
             * \brief Calculation settings
             */
            std::shared_ptr<Sensitivity::SettingsS> settings = std::make_shared<Sensitivity::SettingsS>();

            /**
             * \brief Settings for performing a calculation
             * \remark Settings of the reliability calculation are held in the settings of the reliability method
             */
            std::shared_ptr<Models::RunSettings> runSettings = std::make_shared<Models::RunSettings>();

            /**
             * \brief Callback for progress during the calculation
             */
            std::shared_ptr<Models::ProgressIndicator> progressIndicator = nullptr;

            /**
             * \brief Results of the reliability calculation
             */
            std::shared_ptr<Statistics::Stochast> stochast = nullptr;

            /**
             * \brief Indicates whether the input is valid
             * \return Indication
             */
            bool isValid() const;

            /**
             * \brief Performs the reliability calculation
             * \return Design point
             */
            std::shared_ptr<Statistics::Stochast> getStochast();

            /**
             * \brief Runs the reliability calculation
             */
            void run();
        };
    }
}

