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

#include <vector>
#include "ReliabilityMethod.h"
#include "NumericalBisectionSettings.h"
#include "IntegrationGrid.h"

namespace Deltares
{
    namespace Reliability
    {
        class NumericalBisection : public ReliabilityMethod
        {
        public:
            std::shared_ptr<NumericalBisectionSettings> Settings = std::make_shared<NumericalBisectionSettings>();
            std::shared_ptr<DesignPoint> getDesignPoint(std::shared_ptr<Models::ModelRunner> modelRunner) override;
            bool isValid() override
            {
                return Settings->isValid();
            }

        private:
            std::vector<double> getStartPoint(const int nStochasts) const;
            static size_t getChunkSize(const int nStochasts);
            void splitUnknownCells(std::shared_ptr<Models::ModelRunner> model, int step, std::vector<std::shared_ptr<IntegrationCell>>& unknownCells) const;
            void derivePoints(IntegrationDomain& domain);
            static void updateProbabilities(IntegrationDomain& domain, double& probUnknown, double& probExcluded, double& probFail);
            bool isReady(double diff, int step) const;
            std::shared_ptr<Models::Sample> getMostProbableFailingPoint(double beta, IntegrationDomain& domain) const;
        };
    }
}

