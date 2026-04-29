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
using System;

namespace Deltares.Probabilistic.Statistics;

public enum DistributionType { Deterministic, Normal, LogNormal, Uniform, Triangular, Trapezoidal, Exponential, Gamma, Beta, Frechet, Weibull, ConditionalWeibull, Gumbel, GeneralizedExtremeValue, StudentT, Rayleigh, RayleighN, Pareto, GeneralizedPareto, Histogram, CDFCurve, Discrete, Bernoulli, Poisson, Composite, StandardNormal, Qualitative };

internal static class DistributionTypeConverter
{
    public static string ConvertToString(DistributionType distributionType)
    {
        switch (distributionType)
        {
            case DistributionType.Deterministic: return "deterministic";
            case DistributionType.Normal: return "normal";
            case DistributionType.LogNormal: return "log_normal";
            case DistributionType.Uniform: return "uniform";
            case DistributionType.Triangular: return "triangular";
            case DistributionType.Trapezoidal: return "trapezoidal";
            case DistributionType.Exponential: return "exponential";
            case DistributionType.Gamma: return "gamma";
            case DistributionType.Beta: return "beta";
            case DistributionType.Frechet: return "frechet";
            case DistributionType.Weibull: return "weibull";
            case DistributionType.ConditionalWeibull: return "conditional_weibull";
            case DistributionType.Gumbel: return "gumbel";
            case DistributionType.GeneralizedExtremeValue: return "generalized_extreme_value";
            case DistributionType.StudentT: return "student_t";
            case DistributionType.Rayleigh: return "rayleigh";
            case DistributionType.RayleighN: return "rayleigh_n";
            case DistributionType.Pareto: return "pareto";
            case DistributionType.GeneralizedPareto: return "generalized_pareto";
            case DistributionType.Histogram: return "histogram";
            case DistributionType.CDFCurve: return "cdf_curve";
            case DistributionType.Discrete: return "discrete";
            case DistributionType.Bernoulli: return "bernoulli";
            case DistributionType.Poisson: return "poisson";
            case DistributionType.Composite: return "composite";
            case DistributionType.StandardNormal: return "standard_normal";
            case DistributionType.Qualitative: return "qualitative";

            default:
                throw new ArgumentOutOfRangeException(nameof(distributionType), distributionType, "Unknown distribution type");
        }
    }

    public static DistributionType ConvertFromString(string value)
    {
        switch (value.ToLowerInvariant())
        {
            case "deterministic": return DistributionType.Deterministic;
            case "normal": return DistributionType.Normal;
            case "log_normal": return DistributionType.LogNormal;
            case "uniform": return DistributionType.Uniform;
            case "triangular": return DistributionType.Triangular;
            case "trapezoidal": return DistributionType.Trapezoidal;
            case "exponential": return DistributionType.Exponential;
            case "gamma": return DistributionType.Gamma;
            case "beta": return DistributionType.Beta;
            case "frechet": return DistributionType.Frechet;
            case "weibull": return DistributionType.Weibull;
            case "conditional_weibull": return DistributionType.ConditionalWeibull;
            case "gumbel": return DistributionType.Gumbel;
            case "generalized_extreme_value": return DistributionType.GeneralizedExtremeValue;
            case "student_t": return DistributionType.StudentT;
            case "rayleigh": return DistributionType.Rayleigh;
            case "rayleigh_n": return DistributionType.RayleighN;
            case "pareto": return DistributionType.Pareto;
            case "generalized_pareto": return DistributionType.GeneralizedPareto;
            case "histogram": return DistributionType.Histogram;
            case "cdf_curve": return DistributionType.CDFCurve;
            case "discrete": return DistributionType.Discrete;
            case "bernoulli": return DistributionType.Bernoulli;
            case "poisson": return DistributionType.Poisson;
            case "composite": return DistributionType.Composite;
            case "standard_normal": return DistributionType.StandardNormal;
            case "qualitative": return DistributionType.Qualitative;

            default:
                throw new ArgumentException($"Unknown distribution type string: '{value}'", nameof(value));
        }
    }
}
