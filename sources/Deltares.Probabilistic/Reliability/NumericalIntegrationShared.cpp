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

#include "NumericalIntegrationShared.h"
#include "../Statistics/StandardNormal.h"

using namespace Deltares::Statistics;

namespace Deltares::Reliability
{
    std::vector<double> NumericalIntegrationShared:: buildUpList(const StochastSettings& varying_stochast_settings)
    {
        constexpr double u_delta = 0.01;

        // Initialize parameters for stochastic parameter u.
        const int nr_intervals = varying_stochast_settings.Intervals; // number of intervals in u-space as integer
        const double u_min = varying_stochast_settings.MinValue; // lower bound for u
        const double u_max = varying_stochast_settings.MaxValue; // upper bound for u
        const double range_u = u_max - u_min;

        // Build up list of u values to be computed
        std::vector u_values = { -StandardNormal::UMax };

        for (int i = 0; i <= nr_intervals; i++)
        {
            const double u_value = u_min + i * range_u / nr_intervals;
            if (u_value > -StandardNormal::UMax + u_delta && u_value < StandardNormal::UMax - u_delta)
            {
                u_values.push_back(u_value);
            }
        }
        u_values.push_back(StandardNormal::UMax);
        return u_values;
    }
}

