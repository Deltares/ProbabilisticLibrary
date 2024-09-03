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
#include "DistributionType.h"
#include "../../Deltares.Probabilistic/Statistics/DistributionType.h"
#include "../../Deltares.Probabilistic/Statistics/DistributionPropertyType.h"

namespace Deltares
{
    namespace Statistics
    {
        namespace Wrappers
        {
            Statistics::DistributionType DistributionTypeConverter::getNativeDistributionType(Wrappers::DistributionType distributionType)
            {
                switch (distributionType)
                {
                case Wrappers::DistributionType::Deterministic: return Statistics::DistributionType::Deterministic;
                case Wrappers::DistributionType::Normal: return Statistics::DistributionType::Normal;
                case Wrappers::DistributionType::LogNormal: return Statistics::DistributionType::LogNormal;
                case Wrappers::DistributionType::StudentT: return Statistics::DistributionType::StudentT;
                case Wrappers::DistributionType::Uniform: return Statistics::DistributionType::Uniform;
                case Wrappers::DistributionType::Exponential: return Statistics::DistributionType::Exponential;
                case Wrappers::DistributionType::Triangular: return Statistics::DistributionType::Triangular;
                case Wrappers::DistributionType::Trapezoidal: return Statistics::DistributionType::Trapezoidal;
                case Wrappers::DistributionType::Gumbel: return Statistics::DistributionType::Gumbel;
                case Wrappers::DistributionType::Weibull: return Statistics::DistributionType::Weibull;
                case Wrappers::DistributionType::ConditionalWeibull: return Statistics::DistributionType::ConditionalWeibull;
                case Wrappers::DistributionType::Frechet: return Statistics::DistributionType::Frechet;
                case Wrappers::DistributionType::GeneralizedExtremeValue: return Statistics::DistributionType::GeneralizedExtremeValue;
                case Wrappers::DistributionType::Rayleigh: return Statistics::DistributionType::Rayleigh;
                case Wrappers::DistributionType::RayleighN: return Statistics::DistributionType::RayleighN;
                case Wrappers::DistributionType::Pareto: return Statistics::DistributionType::Pareto;
                case Wrappers::DistributionType::GeneralizedPareto: return Statistics::DistributionType::GeneralizedPareto;
                case Wrappers::DistributionType::Beta: return Statistics::DistributionType::Beta;
                case Wrappers::DistributionType::Gamma: return Statistics::DistributionType::Gamma;
                case Wrappers::DistributionType::Bernoulli: return Statistics::DistributionType::Bernoulli;
                case Wrappers::DistributionType::Poisson: return Statistics::DistributionType::Poisson;
                case Wrappers::DistributionType::Table: return Statistics::DistributionType::Table;
                case Wrappers::DistributionType::CDFCurve: return Statistics::DistributionType::CDFCurve;
                case Wrappers::DistributionType::Discrete: return Statistics::DistributionType::Discrete;
                case Wrappers::DistributionType::Qualitative: return Statistics::DistributionType::Qualitative;
                case Wrappers::DistributionType::Composite: return Statistics::DistributionType::Composite;
                case Wrappers::DistributionType::StandardNormal: return Statistics::DistributionType::StandardNormal;
                default: throw gcnew System::NotSupportedException(distributionType.ToString());
                }
            }

            DistributionType DistributionTypeConverter::getManagedDistributionType(Statistics::DistributionType distributionType)
            {
                switch (distributionType)
                {
                case Statistics::DistributionType::Deterministic: return Wrappers::DistributionType::Deterministic;
                case Statistics::DistributionType::Normal: return Wrappers::DistributionType::Normal;
                case Statistics::DistributionType::LogNormal: return Wrappers::DistributionType::LogNormal;
                case Statistics::DistributionType::StudentT: return Wrappers::DistributionType::StudentT;
                case Statistics::DistributionType::Uniform: return Wrappers::DistributionType::Uniform;
                case Statistics::DistributionType::Exponential: return Wrappers::DistributionType::Exponential;
                case Statistics::DistributionType::Triangular: return Wrappers::DistributionType::Triangular;
                case Statistics::DistributionType::Trapezoidal: return Wrappers::DistributionType::Trapezoidal;
                case Statistics::DistributionType::Gumbel: return Wrappers::DistributionType::Gumbel;
                case Statistics::DistributionType::Weibull: return Wrappers::DistributionType::Weibull;
                case Statistics::DistributionType::ConditionalWeibull: return Wrappers::DistributionType::ConditionalWeibull;
                case Statistics::DistributionType::Frechet: return Wrappers::DistributionType::Frechet;
                case Statistics::DistributionType::GeneralizedExtremeValue: return Wrappers::DistributionType::GeneralizedExtremeValue;
                case Statistics::DistributionType::Rayleigh: return Wrappers::DistributionType::Rayleigh;
                case Statistics::DistributionType::RayleighN: return Wrappers::DistributionType::RayleighN;
                case Statistics::DistributionType::Pareto: return Wrappers::DistributionType::Pareto;
                case Statistics::DistributionType::GeneralizedPareto: return Wrappers::DistributionType::GeneralizedPareto;
                case Statistics::DistributionType::Beta: return Wrappers::DistributionType::Beta;
                case Statistics::DistributionType::Gamma: return Wrappers::DistributionType::Gamma;
                case Statistics::DistributionType::Bernoulli: return Wrappers::DistributionType::Bernoulli;
                case Statistics::DistributionType::Poisson: return Wrappers::DistributionType::Poisson;
                case Statistics::DistributionType::Table: return Wrappers::DistributionType::Table;
                case Statistics::DistributionType::CDFCurve: return Wrappers::DistributionType::CDFCurve;
                case Statistics::DistributionType::Discrete: return Wrappers::DistributionType::Discrete;
                case Statistics::DistributionType::Qualitative: return Wrappers::DistributionType::Qualitative;
                case Statistics::DistributionType::Composite: return Wrappers::DistributionType::Composite;
                case Statistics::DistributionType::StandardNormal: return Wrappers::DistributionType::StandardNormal;
                default: throw gcnew System::NotSupportedException("distribution type");
                }
            }

            Statistics::DistributionPropertyType DistributionTypeConverter::getNativeDistributionPropertyType(Wrappers::DistributionPropertyType distributionPropertyType)
            {
                switch (distributionPropertyType)
                {
                case Wrappers::DistributionPropertyType::Location: return Statistics::DistributionPropertyType::Location;
                case Wrappers::DistributionPropertyType::Scale: return Statistics::DistributionPropertyType::Scale;
                case Wrappers::DistributionPropertyType::Minimum: return Statistics::DistributionPropertyType::Minimum;
                case Wrappers::DistributionPropertyType::Maximum: return Statistics::DistributionPropertyType::Maximum;
                case Wrappers::DistributionPropertyType::Shift: return Statistics::DistributionPropertyType::Shift;
                case Wrappers::DistributionPropertyType::ShiftB: return Statistics::DistributionPropertyType::ShiftB;
                case Wrappers::DistributionPropertyType::Shape: return Statistics::DistributionPropertyType::Shape;
                case Wrappers::DistributionPropertyType::ShapeB: return Statistics::DistributionPropertyType::ShapeB;
                case Wrappers::DistributionPropertyType::Observations: return Statistics::DistributionPropertyType::Observations;
                default: throw gcnew System::NotSupportedException(distributionPropertyType.ToString());
                }
            }
        }
    }
}


