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
    SubsetSimulation,
    LatinHypercube,
    Cobyla,
    FORM,
    FORMThenDirectionalSampling,
    DirectionalSamplingThenFORM
}

public static class ReliabilityMethodConverter
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
            ReliabilityMethod.SubsetSimulation => "subset_simulation",
            ReliabilityMethod.LatinHypercube => "latin_hypercube",
            ReliabilityMethod.Cobyla => "cobyla",
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
            "subset_simulation" => ReliabilityMethod.SubsetSimulation,
            "latin_hypercube" => ReliabilityMethod.LatinHypercube,
            "cobyla" => ReliabilityMethod.Cobyla,
            "form" => ReliabilityMethod.FORM,
            "form_then_directional_sampling" => ReliabilityMethod.FORMThenDirectionalSampling,
            "directional_sampling_then_form" => ReliabilityMethod.DirectionalSamplingThenFORM,
            _ => throw new ArgumentException($"Unknown reliability method: '{value}'", nameof(value))
        };
    }
}
