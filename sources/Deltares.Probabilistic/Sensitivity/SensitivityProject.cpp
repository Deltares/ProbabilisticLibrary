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
#include "SensitivityProject.h"

namespace Deltares
{
    namespace Sensitivity
    {
        void SensitivityProject::run()
        {
            this->sensitivityMethod = this->settings->GetSensitivityMethod();
            this->runSettings = this->settings->RunSettings;

            this->stochast = this->getStochast();
        }

        std::shared_ptr<Statistics::Stochast> SensitivityProject::getStochast()
        {
            std::shared_ptr<Models::UConverter> uConverter = std::make_shared<Models::UConverter>(this->stochasts, this->correlationMatrix);
            const std::shared_ptr<Models::ModelRunner> modelRunner = std::make_shared<Models::ModelRunner>(this->model, uConverter, this->progressIndicator);
            modelRunner->Settings = this->runSettings;
            modelRunner->initializeForRun();

            this->stochast = this->sensitivityMethod->getStochast(modelRunner);

            return this->stochast;
        }

        bool SensitivityProject::isValid() const
        {
            return  this->model != nullptr && 
                    this->runSettings != nullptr && this->runSettings->isValid();
        }
    }
}

