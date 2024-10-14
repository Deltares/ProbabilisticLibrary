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
#pragma once

#include "CobylaReliabilitySettings.h"
#include "ReliabilityMethod.h"
#include "../Optimization/CobylaOptimization.h"

namespace Deltares
{
    namespace Reliability
    {
        class wrappedOptimizationModel : public Optimization::optimizationModel
        {
        public:
            wrappedOptimizationModel(std::shared_ptr<Models::ModelRunner> model_runner) : modelRunner(model_runner) {}
            double GetZValue(const std::shared_ptr<Models::Sample> sample) const override;
            double GetConstraintValue(const std::shared_ptr<Models::Sample> sample) const override;
            unsigned GetNumberOfConstraints() const { return 1; };
        private:
            std::shared_ptr<Models::ModelRunner> modelRunner;
        };

        class CobylaReliability : public ReliabilityMethod
        {
        public:
            std::shared_ptr<CobylaReliabilitySettings> Settings = std::make_shared<CobylaReliabilitySettings>();
            std::shared_ptr<DesignPoint> getDesignPoint(std::shared_ptr<Models::ModelRunner> modelRunner) override;
        };
    }
}


