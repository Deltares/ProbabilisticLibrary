// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of the Probabilistic Library.
//
// The Probabilistic Library is free software: you can redistribute it and/or modify
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
#include <cmath>

#include "../Model/Sample.h"

namespace Deltares
{
    namespace Reliability
    {
        class ConvergenceReport
        {
        public:
            bool IsConverged = false;
            double Convergence = nan("");
            int FailedSamples = -1;
            double FailFraction = nan("");
            double RelaxationFactor = nan("");
            double VarianceFactor = nan("");
            double MaxWeight = nan("");
            double FailWeight = nan("");
            double ZMargin = 0;
            int TotalDirections = -1;
            int TotalIterations = -1;
            int TotalModelRuns = 0;

            std::shared_ptr<Models::Sample> NearestSample = nullptr;

            double getSmallestFraction()
            {
                return std::min(FailFraction, 1 - FailFraction);
            }
        };
    }
}

