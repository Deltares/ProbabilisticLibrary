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
#include "../Model/ParameterSelector.h"
#include "SensitivityMethod.h"
#include "SensitivitySettings.h"

namespace Deltares::Sensitivity
{
    /**
     * \brief Combines a model, stochastic variables and calculation settings, can perform a calculation and holds results
     */
    class SensitivityProject : public Models::ModelProject
    {
    public:
        /**
         * \brief Method which performs a uncertainty calculation
         */
        std::shared_ptr<SensitivityMethod> sensitivityMethod = nullptr;

        /**
         * \brief Output parameter for which the uncertainty is calculated, blank for all parameters
         */
        std::string parameter = "";

        /**
         * \brief Array index of the output parameter if the parameter is an array
         */
        int arrayIndex = 0;

        /**
         * \brief Calculation settings
         */
        std::shared_ptr<SensitivitySettings> settings = std::make_shared<SensitivitySettings>();

        /**
         * \brief Settings for performing a calculation
         * \remark Settings of the uncertainty calculation are held in the settings of the uncertainty method
         */
        std::shared_ptr<Models::RunSettings> runSettings = std::make_shared<Models::RunSettings>();

        /**
         * \brief Callback for progress during the calculation
         */
        std::shared_ptr<Models::ProgressIndicator> progressIndicator = nullptr;

        /**
         * \brief Results of the sensitivity calculation
         */
        std::shared_ptr<SensitivityResult> sensitivityResult = nullptr;

        /**
         * \brief Results of the sensitivity calculation
         */
        std::vector<std::shared_ptr<SensitivityResult>> sensitivityResults;

        /**
         * \brief Reports whether these settings have valid values
         * \param report Report in which the validity is reported
         */
        void validate(Logging::ValidationReport& report) override;

        /**
         * \brief Performs the sensitivity calculation
         * \return Sensitivity result
         */
        SensitivityResult getSensitivityResult();

        /**
         * \brief Runs the reliability calculation
         */
        void run();
    private:
        std::shared_ptr<Models::ParameterSelector> parameterSelector = std::make_shared<Models::ParameterSelector>();
    };
}


