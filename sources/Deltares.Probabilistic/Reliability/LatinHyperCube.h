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

#include "ReliabilityMethod.h"
#include "LatinHyperCubeSettings.h"

namespace Deltares
{
    namespace Reliability
    {
        struct IndexedItem
        {
            int Value = 0;
            double Sequence = 0.0;
        };

        class LatinHyperCube: public ReliabilityMethod
        {
        public:
            std::shared_ptr<LatinHyperCubeSettings> Settings = std::make_shared<LatinHyperCubeSettings>();
            std::shared_ptr<DesignPoint> getDesignPoint(std::shared_ptr<Models::ModelRunner> modelRunner) override;
        private:
            std::shared_ptr<DesignPoint> getReducedDesignPoint(std::shared_ptr<Models::ModelRunner>& modelRunner, std::shared_ptr<Models::SampleProvider>& sampleProvider, double qRange);
            double ReportConvergence(std::shared_ptr<Models::ModelRunner>& modelRunner, double pf, int samples, int nMaal) const;
            std::vector<std::shared_ptr<Sample>> CreateAllSamples(int nStochasts, const std::shared_ptr<Models::SampleProvider>& sampleProvider) const;
        };
    }
}
