// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of Streams.
//
// Streams is free software: you can redistribute it and/or modify
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
#include "SensitivityProject.h"

namespace Deltares
{
    namespace Sensitivity
    {
        void SensitivityProject::run()
        {
            this->sensitivityStochast = nullptr;
            this->sensitivityStochasts.clear();

            this->sensitivityMethod = this->settings->GetSensitivityMethod();
            this->runSettings = this->settings->RunSettings;

            for (std::shared_ptr<Models::ModelParameter> modelParameter : this->model->outputParameters)
            {
                this->model->Index = modelParameter->index;

                std::shared_ptr<Statistics::Stochast> stochast = this->getStochast();
                this->sensitivityStochasts.push_back(stochast);
            }

            if (this->sensitivityStochasts.size() > 0)
            {
                this->sensitivityStochast = this->sensitivityStochasts[0];
            }

            // reset the index
            this->model->Index = 0;

            this->outputCorrelationMatrix = nullptr;
            if (this->settings->CalculateCorrelations)
            {
                this->outputCorrelationMatrix = this->sensitivityMethod->getCorrelationMatrix();
            }
        }

        std::shared_ptr<Statistics::Stochast> SensitivityProject::getStochast()
        {
            if (this->initializer != nullptr)
            {
                initializer();
            }

            std::shared_ptr<Models::UConverter> uConverter = std::make_shared<Models::UConverter>(this->stochasts, this->correlationMatrix);
            const std::shared_ptr<Models::ModelRunner> modelRunner = std::make_shared<Models::ModelRunner>(this->model, uConverter, this->progressIndicator);
            modelRunner->Settings = this->runSettings;
            modelRunner->initializeForRun();

            std::shared_ptr<Statistics::Stochast> sensitivityStochast = this->sensitivityMethod->getStochast(modelRunner);
            sensitivityStochast->name = this->model->getIndexedName();

            return sensitivityStochast;
        }

        bool SensitivityProject::isValid() const
        {
            return  this->model != nullptr &&
                this->runSettings != nullptr && this->runSettings->isValid();
        }
    }
}

