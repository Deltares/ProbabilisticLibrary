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
            std::shared_ptr<Sample> getStartPoint(Models::ModelRunner& modelRunner) const;
        private:
            std::shared_ptr<Sample> getNoneStartPoint() const;
            std::shared_ptr<Sample> getRayStartPoint(Models::ModelRunner& modelRunner) const;
            std::shared_ptr<Sample> getSensitivityStartPoint(Models::ModelRunner& modelRunner) const;
            std::shared_ptr<Sample> getSphereStartPoint(Models::ModelRunner& modelRunner) const;
            std::shared_ptr<Sample> getDirectionStartPoint(Models::ModelRunner& modelRunner, Sample& startPoint) const;
            void correctDefaultValues(Sample& startPoint) const;
            static void setBestSample(std::shared_ptr<Sample> & bestSample, const std::shared_ptr<Sample> sample);
            static std::shared_ptr<Sample> refineSpherePoint(Sample& u, Sample& previous);
            static std::vector<double> getGradient(Models::ModelRunner& modelRunner, const std::shared_ptr<Sample>& sample);
        };
    }
}

