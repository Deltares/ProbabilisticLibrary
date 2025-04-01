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
#include "SensitivityProject.h"

namespace Deltares
{
    namespace Sensitivity
    {
        void SensitivityProject::run()
        {
            this->modelRuns = 0;

            this->sensitivityResult = nullptr;
            this->sensitivityResults.clear();

            this->settings->RandomSettings->setFixed(true);

            this->sensitivityMethod = this->settings->GetSensitivityMethod();
            this->runSettings = this->settings->RunSettings;

            if (this->parameter.empty())
            {
                for (std::shared_ptr<Models::ModelInputParameter>& modelParameter : this->model->outputParameters)
                {
                    this->parameterSelector->parameter = modelParameter->name;
                    if (modelParameter->isArray)
                    {
                        for (int index = 0; index < modelParameter->arraySize; index++)
                        {
                            this->parameterSelector->arrayIndex = index;

                            auto result = std::shared_ptr<Sensitivity::SensitivityResult>(this->getSensitivityResult());
                            result->stochast->name += "[" + std::to_string(index) + "]";
                            this->sensitivityResults.push_back(result);
                        }
                    }
                    else
                    {
                        auto result = std::shared_ptr<Sensitivity::SensitivityResult>(this->getSensitivityResult());
                        this->sensitivityResults.push_back(result);
                    }
                }
            }
            else
            {
                this->parameterSelector->parameter = this->parameter;
                this->parameterSelector->arrayIndex = this->arrayIndex;

                auto result = std::shared_ptr<Sensitivity::SensitivityResult>(this->getSensitivityResult());
                this->sensitivityResults.push_back(result);
            }

            if (!this->sensitivityResults.empty())
            {
                this->sensitivityResult = this->sensitivityResults[0];
            }

            // reset the index
            this->model->Index = 0;

            this->settings->RandomSettings->setFixed(false);

            this->outputCorrelationMatrix = nullptr;
            if (this->settings->CalculateCorrelations)
            {
                this->outputCorrelationMatrix = this->sensitivityMethod->getCorrelationMatrix();
            }
        }

        Sensitivity::SensitivityResult* SensitivityProject::getSensitivityResult()
        {
            this->model->zValueConverter = this->parameterSelector;

            std::shared_ptr<Models::UConverter> uConverter = std::make_shared<Models::UConverter>(this->stochasts, this->correlationMatrix);
            const std::shared_ptr<Models::ModelRunner> modelRunner = std::make_shared<Models::ModelRunner>(this->model, uConverter, this->progressIndicator);
            modelRunner->Settings = this->runSettings;
            modelRunner->initializeForRun();

            SensitivityResult* result = this->sensitivityMethod->getSensitivityStochast(modelRunner);
            result->stochast->name = this->parameterSelector->parameter;

            this->modelRuns += this->model->getModelRuns();

            return result;
        }

        bool SensitivityProject::isValid() const
        {
            return this->model != nullptr && this->runSettings != nullptr && this->runSettings->isValid();
        }
    }
}

