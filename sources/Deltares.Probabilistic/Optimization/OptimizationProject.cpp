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
#include "OptimizationProject.h"

namespace Deltares::Optimization
{
    void OptimizationProject::run()
    {
        this->modelRuns = 0;
        this->optimizationMethod = this->settings->GetOptimizationMethod();
        this->runSettings = this->settings->RunSettings;

        this->result = this->getOptimizedSample();
    }

    void OptimizationProject::stop()
    {
        this->optimizationMethod->Stop();
    }

    std::shared_ptr<OptimizationResult> OptimizationProject::getOptimizedSample()
    {
        this->result = std::make_shared<OptimizationResult>(optimizationMethod->getOptimizedSample(zModel));

        if (this->result != nullptr)
        {
            this->modelRuns += this->result->modelRuns;
        }

        return this->result;
    }

    void OptimizationProject::validate(Logging::ValidationReport& report)
    {
        ModelProject::validate(report);
        settings->validate(report);
    }
}

