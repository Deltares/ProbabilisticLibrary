using System;

namespace Deltares.Probabilistic.Reliability;

public enum StartPointMethod
{
    FixedValue,
    RaySearch,
    SensitivitySearch,
    SphereSearch
}

public static class StartPointMethodConverter
{
    public static string ConvertToString(StartPointMethod method)
    {
        return method switch
        {
            StartPointMethod.FixedValue => "fixed_value",
            StartPointMethod.RaySearch => "ray_search",
            StartPointMethod.SensitivitySearch => "sensitivity_search",
            StartPointMethod.SphereSearch => "sphere_search",
            _ => throw new ArgumentOutOfRangeException(nameof(method), method, null)
        };
    }

    public static StartPointMethod ConvertFromString(string value)
    {
        return value switch
        {
            "fixed_value" => StartPointMethod.FixedValue,
            "ray_search" => StartPointMethod.RaySearch,
            "sensitivity_search" => StartPointMethod.SensitivitySearch,
            "sphere_search" => StartPointMethod.SphereSearch,
            _ => throw new ArgumentException($"Unknown start point method: '{value}'", nameof(value))
        };
    }
}
