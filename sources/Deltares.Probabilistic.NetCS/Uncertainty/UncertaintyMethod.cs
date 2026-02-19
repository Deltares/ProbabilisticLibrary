using System;

namespace Deltares.Probabilistic.Uncertainty;

public enum UncertaintyMethod
{
    CrudeMonteCarlo, ImportanceSampling, DirectionalSampling, NumericalIntegration, FORM, FOSM
}

internal static class UncertaintyMethodConverter
{
    public static string ConvertToString(UncertaintyMethod method)
    {
        return method switch
        {
            UncertaintyMethod.NumericalIntegration => "numerical_integration",
            UncertaintyMethod.CrudeMonteCarlo => "crude_monte_carlo",
            UncertaintyMethod.ImportanceSampling => "importance_sampling",
            UncertaintyMethod.DirectionalSampling => "directional_sampling",
            UncertaintyMethod.FORM => "form",
            UncertaintyMethod.FOSM => "fosm",
            _ => throw new ArgumentOutOfRangeException(nameof(method), method, null)
        };
    }

    public static UncertaintyMethod ConvertFromString(string value)
    {
        return value switch
        {
            "numerical_integration" => UncertaintyMethod.NumericalIntegration,
            "crude_monte_carlo" => UncertaintyMethod.CrudeMonteCarlo,
            "importance_sampling" => UncertaintyMethod.ImportanceSampling,
            "directional_sampling" => UncertaintyMethod.DirectionalSampling,
            "form" => UncertaintyMethod.FORM,
            "fosm" => UncertaintyMethod.FOSM,
            _ => throw new ArgumentException($"Unknown uncertainty method: '{value}'", nameof(value))
        };
    }
}
