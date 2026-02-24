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

namespace Deltares.Probabilistic.Reliability;

public enum ReliabilityMethod
{
    NumericalIntegration,
    NumericalBisection,
    CrudeMonteCarlo,
    ImportanceSampling,
    AdaptiveImportanceSampling,
    DirectionalSampling,
    DirectionReliability,
    SubsetSimulation,
    LatinHypercube,
    Cobyla,
    FORM,
    FORMThenDirectionalSampling,
    DirectionalSamplingThenFORM
}

internal static class ReliabilityMethodConverter
{
    public static string ConvertToString(ReliabilityMethod method)
    {
        return method switch
        {
            ReliabilityMethod.NumericalIntegration => "numerical_integration",
            ReliabilityMethod.NumericalBisection => "numerical_bisection",
            ReliabilityMethod.CrudeMonteCarlo => "crude_monte_carlo",
            ReliabilityMethod.ImportanceSampling => "importance_sampling",
            ReliabilityMethod.AdaptiveImportanceSampling => "adaptive_importance_sampling",
            ReliabilityMethod.DirectionalSampling => "directional_sampling",
            ReliabilityMethod.DirectionReliability => "direction_reliability",
            ReliabilityMethod.SubsetSimulation => "subset_simulation",
            ReliabilityMethod.LatinHypercube => "latin_hypercube",
            ReliabilityMethod.Cobyla => "cobyla_reliability",
            ReliabilityMethod.FORM => "form",
            ReliabilityMethod.FORMThenDirectionalSampling => "form_then_directional_sampling",
            ReliabilityMethod.DirectionalSamplingThenFORM => "directional_sampling_then_form",
            _ => throw new ArgumentOutOfRangeException(nameof(method), method, null)
        };
    }

    public static ReliabilityMethod ConvertFromString(string value)
    {
        return value switch
        {
            "numerical_integration" => ReliabilityMethod.NumericalIntegration,
            "numerical_bisection" => ReliabilityMethod.NumericalBisection,
            "crude_monte_carlo" => ReliabilityMethod.CrudeMonteCarlo,
            "importance_sampling" => ReliabilityMethod.ImportanceSampling,
            "adaptive_importance_sampling" => ReliabilityMethod.AdaptiveImportanceSampling,
            "directional_sampling" => ReliabilityMethod.DirectionalSampling,
            "direction_reliability" => ReliabilityMethod.DirectionReliability,
            "subset_simulation" => ReliabilityMethod.SubsetSimulation,
            "latin_hypercube" => ReliabilityMethod.LatinHypercube,
            "cobyla_reliability" => ReliabilityMethod.Cobyla,
            "form" => ReliabilityMethod.FORM,
            "form_then_directional_sampling" => ReliabilityMethod.FORMThenDirectionalSampling,
            "directional_sampling_then_form" => ReliabilityMethod.DirectionalSamplingThenFORM,
            _ => throw new ArgumentException($"Unknown reliability method: '{value}'", nameof(value))
        };
    }
}
