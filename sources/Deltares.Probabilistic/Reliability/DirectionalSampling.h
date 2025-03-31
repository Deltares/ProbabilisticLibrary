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

#include "DirectionalSamplingSettings.h"
#include "DirectionReliability.h"
#include "PrecomputeValues.h"
#include "ReliabilityMethod.h"

namespace Deltares
{
    namespace Reliability
    {
        class DirectionalSampling : public ReliabilityMethod
        {
        private:
            static double getConvergence(const double pf, const double sumWeights, const double sumWeights2, const int nDirections);
            std::vector<double> getDirectionBetas(std::shared_ptr<Models::ModelRunner> modelRunner, std::vector<std::shared_ptr<Sample>> samples, double z0, double threshold);
            std::unordered_map<int, double> previousResults;
            static std::vector<PrecomputeValues> precompute(const std::shared_ptr<Models::ModelRunner>& modelRunner,
                const std::vector<std::shared_ptr<Sample>>& samples, double z0,
                const DirectionReliabilityForDirectionalSampling& directionReliability, std::vector<bool>& mask);

        public:
            std::shared_ptr<DirectionalSamplingSettings> Settings = std::make_shared<DirectionalSamplingSettings>();
            std::shared_ptr<DesignPoint> getDesignPoint(std::shared_ptr<Models::ModelRunner> modelRunner) override;
        };
    }
}

