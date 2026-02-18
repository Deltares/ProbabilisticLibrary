using Deltares.Probabilistic.Reliability;
using System;

namespace Deltares.Probabilistic.Sensitivity;

public enum SensitivityMethod
{
    SingleVariation,
    Sobol
}

public static class SensitivityMethodConverter
{
    public static string ConvertToString(SensitivityMethod method)
    {
        return method switch
        {
            SensitivityMethod.SingleVariation => "single_variation",
            SensitivityMethod.Sobol => "sobol",
            _ => throw new ArgumentOutOfRangeException(nameof(method), method, null)
        };
    }

    public static SensitivityMethod ConvertFromString(string value)
    {
        return value switch
        {
            "single_variation" => SensitivityMethod.SingleVariation,
            "sobol" => SensitivityMethod.Sobol,
            _ => throw new ArgumentException($"Unknown sensitivity method: '{value}'", nameof(value))
        };
    }
}
