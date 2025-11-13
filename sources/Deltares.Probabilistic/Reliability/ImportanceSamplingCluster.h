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
#include <memory>

#include "DesignPointBuilder.h"
#include "../Model/Sample.h"

namespace Deltares
{
    namespace Reliability
    {
        /// <summary>
        /// Holds administration and results per importance sampling cluster
        /// </summary>
        class ImportanceSamplingCluster
        {
        public:
            std::shared_ptr<Models::Sample> Center = nullptr;
            DesignPointBuilder designPointBuilder = DesignPointBuilder();

            double ProbFailure = 0.0;
            int TotalCount = 0;
            double TotalWeight = 0.0;
            int FailCount = 0;
            double FailWeight = 0.0;
            double FailFraction = 0.0;
            double MaxFailWeight = 0.0;

            void initialize(int nStochasts, double z0Fac, bool z0Ignore, DesignPointMethod method, std::shared_ptr<StochastSettingsSet> stochastSet);
            void addSample(std::shared_ptr<Deltares::Models::Sample> sample);

            std::shared_ptr<Models::Sample> NearestSample = nullptr; // result sample which is nearest to the limit state
        private:
            double z0Fac = 0;
            bool z0Ignore = false;

            double getProbabilityOfFailure(bool useCount);
            double getProbability(double z0Fac, double failWeight, double totalWeight);
        };
    }
}
