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
#include "SphereTasks.h"
#include <cmath>

namespace Deltares
{
    namespace Reliability
    {
        std::vector<Numeric::vector1D> sphereTasks::examineSurfaceForTasks(const Numeric::vector1D& uRay)
        {
            uTasks.clear();
            auto uRayIpoint = std::vector<int>();
            for (size_t i = 0; i < uRay.size(); i++)
            {
                if (uRay(i) != 0.0)
                {
                    uRayIpoint.push_back(i);
                }
            }
            examineSurfaceForTasks(0, uRay, uRayIpoint);
            return uTasks;
        }

        // Method for the search of all possible u
        void sphereTasks::examineSurfaceForTasks(const size_t idx, const Numeric::vector1D& uRay, const std::vector<int> uRayIpoint)
        {
            // idx     : index in u array
            // uRay    : direction in which u search takes places

            const size_t nrActiveStochasts = uRayIpoint.size();
            const size_t nrStochasts = uRay.size();
            if (idx + 1 < nrActiveStochasts)
            {
                size_t jMax = (uRay(uRayIpoint[idx]) == 0.0 ? 1 : maxSteps);
                auto u = Numeric::vector1D(nrStochasts);

                double dangle = maxAngle / (double)(maxSteps - 1);
                for (size_t j = 1; j <= jMax; j++)
                {
                    double angle = dangle * double(j - 1);
                    for (size_t k = 0; k <= idx; k++)
                    {
                        u(uRayIpoint[k]) = uRay(uRayIpoint[k]) * sin(angle);
                    }
                    u(uRayIpoint[idx + 1]) = uRay(uRayIpoint[idx + 1]) * cos(angle);
                    for (size_t k = idx + 2; k < nrActiveStochasts; k++)
                    {
                        u(uRayIpoint[k]) = uRay(uRayIpoint[k]);
                    }

                    examineSurfaceForTasks(idx + 1, u, uRayIpoint);
                }
            }
            else
            {
                uTasks.push_back(uRay);
            }
        }
    }
}
