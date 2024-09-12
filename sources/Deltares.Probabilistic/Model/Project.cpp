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
#include "Project.h"

namespace Deltares
{
    namespace Models
    {
        void Project::run()
        {
            if (this->initializer != nullptr)
            {
                this->initializer();
            }

            this->reliabilityMethod = this->settings->GetReliabilityMethod();
            this->runSettings = this->settings->RunSettings;

            if (this->settings->ReliabilityResult == Reliability::ResultDesignPoint)
            {
                this->designPoint = this->getDesignPoint();
            }
            else if (this->settings->ReliabilityResult == Reliability::ResultFragilityCurve)
            {
                this->fragilityCurve = this->getFragilityCurve();
            }
        }

        std::shared_ptr<Reliability::DesignPoint> Project::getDesignPoint()
        {
            std::shared_ptr<UConverter> uConverter = std::make_shared<UConverter>(this->stochasts, this->correlationMatrix);
            const std::shared_ptr<ModelRunner> modelRunner = std::make_shared<ModelRunner>(this->model, uConverter, this->progressIndicator);
            modelRunner->Settings = this->runSettings;
            modelRunner->initializeForRun();

            this->designPoint = this->reliabilityMethod->getDesignPoint(modelRunner);

            return this->designPoint;
        }

        std::shared_ptr<Statistics::Stochast> Project::getFragilityCurve()
        {
            this->fragilityCurve = std::make_shared<Statistics::Stochast>();
            fragilityCurve->setDistributionType(Statistics::CDFCurve);

            std::shared_ptr<UConverter> uConverter = std::make_shared<UConverter>(this->stochasts, this->correlationMatrix);
            const std::shared_ptr<ModelRunner> modelRunner = std::make_shared<ModelRunner>(this->model, uConverter, this->progressIndicator);
            modelRunner->Settings = this->runSettings;
            modelRunner->initializeForRun();

            this->designPoint = this->reliabilityMethod->getDesignPoint(modelRunner);

            return this->fragilityCurve;
        }

        bool Project::isValid() const
        {
            return  this->model != nullptr && 
                    this->runSettings != nullptr && this->runSettings->isValid();
        }
    }
}

