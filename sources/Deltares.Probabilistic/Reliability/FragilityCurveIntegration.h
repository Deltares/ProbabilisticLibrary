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

#include "FragilityCurveIntegrationSettings.h"
#include "ReliabilityMethod.h"

namespace Deltares
{
    namespace Reliability
    {
        class FragilityCurveIntegration : public ReliabilityMethod
        {
        public:
            std::shared_ptr<FragilityCurveIntegrationSettings> Settings = std::make_shared<FragilityCurveIntegrationSettings>();

            std::shared_ptr<DesignPoint> getDesignPoint(std::shared_ptr<Statistics::Stochast> parameter, std::shared_ptr<Statistics::Stochast> fragilityCurve, std::shared_ptr<Statistics::Stochast> fragilityCurveNormalized = nullptr);
            std::shared_ptr<DesignPoint> getDesignPoint(std::shared_ptr<Models::ModelRunner> modelRunner) override;
        private:
            class UStep
            {
            public:
                UStep(double u, double weight)
                {
                    this->U = u;
                    this->Weight = weight;
                };
                double U = 0.0;
                double Weight = 0.0;
            };
            std::vector<std::shared_ptr<UStep>> getSteps(std::shared_ptr<Statistics::Stochast> stochast, double stepSize);
        };
    }
}

