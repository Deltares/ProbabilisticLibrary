using System;

namespace Deltares.Probabilistic.Reliability;

public enum GradientMethod
{
    OneDirection,
    TwoDirections
}

internal static class GradientMethodConverter
{
    public static string ConvertToString(GradientMethod method)
    {
        return method switch
        {
            GradientMethod.OneDirection => "single",
            GradientMethod.TwoDirections => "double",
            _ => throw new ArgumentOutOfRangeException(nameof(method), method, null)
        };
    }

    public static GradientMethod ConvertFromString(string value)
    {
        return value switch
        {
            "single" => GradientMethod.OneDirection,
            "double" => GradientMethod.TwoDirections,
            _ => throw new ArgumentException($"Unknown gradient method: '{value}'", nameof(value))
        };
    }
}
