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
#include "CrudeMonteCarloSettings.h"
#include "ReliabilityMethod.h"

namespace Deltares
{
    namespace Reliability
    {
        class CrudeMonteCarlo : public ReliabilityMethod
        {
        public:
            std::shared_ptr<CrudeMonteCarloSettings> Settings = std::make_shared<CrudeMonteCarloSettings>();
            std::shared_ptr<DesignPoint> getDesignPoint(std::shared_ptr<Models::ModelRunner> modelRunner) override;
        private:
            std::shared_ptr<DesignPoint> getReducedDesignPoint(std::shared_ptr<Models::ModelRunner> modelRunner, std::shared_ptr<SampleProvider> sampleProvider, double zRemainder, double qRange);
            bool checkConvergence(std::shared_ptr<Models::ModelRunner> modelRunner, double pf, int samples, int nmaal);
            double getConvergence(double pf, int samples);
            void applyLimits(std::shared_ptr<Models::Sample> sample);
        };
    }
}

