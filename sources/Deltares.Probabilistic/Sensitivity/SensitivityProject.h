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
#include <vector>

#include "../Model/ModelProject.h"
#include "ParameterSelector.h"
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
        class SensitivityProject : public Models::ModelProject
        {
        public:
            /**
             * \brief Method which performs a reliability calculation
             */
            std::shared_ptr<Sensitivity::SensitivityMethod> sensitivityMethod = nullptr;

            /**
             * \brief Output parameter for which the sensitivity is calculated, blank for all parameters
             */
            std::string parameter = "";

            /**
             * \brief Array index of the output parameter if the parameter is an array
             */
            int arrayIndex = 0;

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
             * \brief Results of the sensitivity calculation
             */
            std::shared_ptr<Sensitivity::SensitivityResult> sensitivityResult = nullptr;

            /**
             * \brief Results of the sensitivity calculation
             */
            std::vector<std::shared_ptr<Sensitivity::SensitivityResult>> sensitivityResults;

            /**
             * \brief Resulting correlation matrix
             */
            std::shared_ptr<Statistics::CorrelationMatrix> outputCorrelationMatrix = nullptr;

            /**
             * \brief Indicates whether the input is valid
             * \return Indication
             */
            bool isValid() override;

            /**
             * \brief Performs the sensitivity calculation
             * \return Sensitivity result
             */
            Sensitivity::SensitivityResult getSensitivityResult();

            /**
             * \brief Runs the reliability calculation
             */
            void run();

        private:
            std::shared_ptr<ParameterSelector> parameterSelector = std::make_shared<ParameterSelector>();
        };
    }
}

