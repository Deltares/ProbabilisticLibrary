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

#include "../../Deltares.Probabilistic/Statistics/DistributionType.h"
#include "../../Deltares.Probabilistic/Statistics/DistributionPropertyType.h"

namespace Deltares
{
    namespace Statistics
    {
        namespace Wrappers
        {
            public enum class DistributionType { Deterministic, Normal, LogNormal, Uniform, Triangular, Trapezoidal, Exponential, Gamma, Beta, Frechet, Weibull, ConditionalWeibull, Gumbel, GeneralizedExtremeValue, StudentT, Rayleigh, RayleighN, Pareto, GeneralizedPareto, Table, CDFCurve, Discrete, Bernoulli, Poisson, Composite, StandardNormal, Qualitative };
            public enum class DistributionPropertyType { Location, Scale, Minimum, Maximum, Shift, ShiftB, Shape, ShapeB, Observations };

            public ref class DistributionTypeConverter
            {
            public:
                static Statistics::DistributionType getNativeDistributionType(DistributionType distributionType);
                static DistributionType getManagedDistributionType(Statistics::DistributionType distributionType);
                static Statistics::DistributionPropertyType getNativeDistributionPropertyType(Wrappers::DistributionPropertyType distributionPropertyType);
            };
        }
    }
}


