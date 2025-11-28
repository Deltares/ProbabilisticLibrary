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
#include "ReliabilityProject.h"

#include <iostream>

namespace Deltares
{
    namespace Reliability
    {
        void ReliabilityProject::run()
        {
            this->modelRuns = 0;
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

        std::shared_ptr<Reliability::DesignPoint> ReliabilityProject::getDesignPoint()
        {
            this->model->zValueConverter = this->limitStateFunction;

            std::shared_ptr<UConverter> uConverter = std::make_shared<UConverter>(this->stochasts, this->correlation);
            const std::shared_ptr<ModelRunner> modelRunner = std::make_shared<ModelRunner>(this->model, uConverter, this->progressIndicator);
            modelRunner->Settings = this->runSettings;
            modelRunner->initializeForRun();

            this->designPoint = this->reliabilityMethod->getDesignPoint(modelRunner);

            if (this->designPoint != nullptr)
            {
                this->modelRuns += this->designPoint->getTotalModelRuns();
            }

            return this->designPoint;
        }

        std::shared_ptr<Statistics::Stochast> ReliabilityProject::getFragilityCurve()
        {
            this->fragilityCurve = std::make_shared<Statistics::Stochast>();
            fragilityCurve->setDistributionType(Statistics::CDFCurve);

            std::shared_ptr<UConverter> uConverter = std::make_shared<UConverter>(this->stochasts, this->correlation);
            const std::shared_ptr<ModelRunner> modelRunner = std::make_shared<ModelRunner>(this->model, uConverter, this->progressIndicator);
            modelRunner->Settings = this->runSettings;
            modelRunner->initializeForRun();

            this->designPoint = this->reliabilityMethod->getDesignPoint(modelRunner);

            if (this->designPoint != nullptr)
            {
                this->modelRuns += this->designPoint->getTotalModelRuns();
            }

            return this->fragilityCurve;
        }

        void ReliabilityProject::validate(Logging::ValidationReport& report)
        {
            ModelProject::validate(report);
            settings->validate(report);
        }
    }
}

