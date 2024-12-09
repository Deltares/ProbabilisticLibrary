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
#include <vector>
#include "../Math/vector1D.h"
#include "../Math/basic_math.h"

namespace Deltares
{
    namespace Reliability
    {
        class sphereTasks
        {
        public:
            sphereTasks() : maxSteps(5), maxAngle(M_PI / 2.0) {};
            sphereTasks(const size_t mxSteps, const bool allQuadrants) : maxSteps(mxSteps), maxAngle(allQuadrants ? 2.0 * M_PI : M_PI / 2.0) {};
            std::vector<Numeric::vector1D> examineSurfaceForTasks(const Numeric::vector1D& uRay);
        private:
            void examineSurfaceForTasks(const size_t idx, const Numeric::vector1D& uRay, const std::vector<int> uRayIpoint);
            std::vector<Numeric::vector1D> uTasks;
            const size_t maxSteps;
            const double maxAngle;
        };
    }
}
