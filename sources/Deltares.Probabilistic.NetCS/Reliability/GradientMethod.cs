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
            GradientMethod.OneDirection => "one_direction",
            GradientMethod.TwoDirections => "two_directions",
            _ => throw new ArgumentOutOfRangeException(nameof(method), method, null)
        };
    }

    public static GradientMethod ConvertFromString(string value)
    {
        return value switch
        {
            "one_direction" => GradientMethod.OneDirection,
            "two_directions" => GradientMethod.TwoDirections,
            _ => throw new ArgumentException($"Unknown gradient method: '{value}'", nameof(value))
        };
    }
}
