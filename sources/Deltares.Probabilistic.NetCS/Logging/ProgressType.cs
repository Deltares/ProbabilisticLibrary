using System;

namespace Deltares.Probabilistic.Logging;

public enum ProgressType { Global, Detailed };

internal static class ProgressTypeConverter
{
    public static string ConvertToString(ProgressType type)
    {
        return type switch
        {
            ProgressType.Global => "global",
            ProgressType.Detailed => "detailed",
            _ => throw new ArgumentOutOfRangeException(nameof(type), type, null)
        };
    }

    public static ProgressType ConvertFromString(string value)
    {
        return value.ToLowerInvariant() switch
        {
            "global" => ProgressType.Global,
            "detailed" => ProgressType.Detailed,
            _ => throw new ArgumentException($"Unknown progress type: {value}")
        };
    }
}
