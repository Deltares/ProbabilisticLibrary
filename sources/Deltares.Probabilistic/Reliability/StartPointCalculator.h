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
#include "StartPointCalculatorSettings.h"
#include "../Model/Sample.h"
#include "../Model/ModelRunner.h"

namespace Deltares
{
    namespace Reliability {

        class StartPointCalculator
        {
        public:
            std::shared_ptr<StartPointCalculatorSettings> Settings = std::make_shared<StartPointCalculatorSettings>();
            std::shared_ptr<Sample> getStartPoint(std::shared_ptr<Models::ModelRunner> modelRunner);
        private:
            std::shared_ptr<Sample> getNoneStartPoint();
            std::shared_ptr<Sample> getRayStartPoint(std::shared_ptr<Models::ModelRunner> modelRunner);
            std::shared_ptr<Sample> getSensitivityStartPoint(std::shared_ptr<Models::ModelRunner> modelRunner);
            std::shared_ptr<Sample> getSphereStartPoint(std::shared_ptr<Models::ModelRunner> modelRunner);
            std::shared_ptr<Sample> getDirectionStartPoint(std::shared_ptr<Models::ModelRunner> modelRunner, std::shared_ptr<Sample> startPoint);
            void correctDefaultValues(std::shared_ptr<Sample> startPoint);
            void setBestSample(std::shared_ptr<Sample> & bestSample, const std::shared_ptr<Sample> sample);
            std::shared_ptr<Sample> refineSpherePoint(const std::shared_ptr<Sample> u, const std::shared_ptr<Sample> previous);
            std::vector<double> getGradient(std::shared_ptr<Models::ModelRunner> modelRunner, std::shared_ptr<Sample> sample);
        };
    }
}

