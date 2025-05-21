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
#include "UncertaintyProject.h"

namespace Deltares::Sensitivity
{
    void UncertaintyProject::run()
    {
        modelRuns = 0;

        sensitivityResult = nullptr;
        sensitivityResults.clear();

        settings->RandomSettings->setFixed(true);

        sensitivityMethod = settings->GetSensitivityMethod();
        runSettings = settings->RunSettings;

        if (parameter.empty())
        {
            for (const auto& modelParameter : model->outputParameters)
            {
                parameterSelector->parameter = modelParameter->name;
                if (modelParameter->isArray)
                {
                    for (int index = 0; index < modelParameter->arraySize; index++)
                    {
                        parameterSelector->arrayIndex = index;

                        auto result = std::make_shared<UncertaintyResult>(getSensitivityResult());
                        result->stochast->name += "[" + std::to_string(index) + "]";
                        sensitivityResults.push_back(result);
                    }
                }
                else
                {
                    auto result = std::make_shared<UncertaintyResult>(getSensitivityResult());
                    sensitivityResults.push_back(result);
                }
            }
        }
        else
        {
            parameterSelector->parameter = parameter;
            parameterSelector->arrayIndex = arrayIndex;

            auto result = std::make_shared<UncertaintyResult>(getSensitivityResult());
            sensitivityResults.push_back(result);
        }

        if (!sensitivityResults.empty())
        {
            sensitivityResult = sensitivityResults[0];
        }

        // reset the index
        model->Index = 0;

        settings->RandomSettings->setFixed(false);

        outputCorrelationMatrix = nullptr;
        if (settings->CalculateCorrelations)
        {
            outputCorrelationMatrix = sensitivityMethod->getCorrelationMatrix();
        }
    }

    UncertaintyResult UncertaintyProject::getSensitivityResult()
    {
        model->zValueConverter = parameterSelector;

        std::shared_ptr<Models::UConverter> uConverter = std::make_shared<Models::UConverter>(stochasts, correlationMatrix);
        const std::shared_ptr<Models::ModelRunner> modelRunner = std::make_shared<Models::ModelRunner>(model, uConverter, progressIndicator);
        modelRunner->Settings = runSettings;
        modelRunner->initializeForRun();

        auto result = sensitivityMethod->getSensitivityStochast(modelRunner);
        result.stochast->name = parameterSelector->parameter;

        modelRuns += model->getModelRuns();

        return result;
    }

    bool UncertaintyProject::isValid()
    {
        return ModelProject::isValid() &&
            runSettings != nullptr && runSettings->isValid() &&
            settings->isValid();
    }
}

