using System;

namespace Deltares.Probabilistic.Reliability;

public enum CombineType {Series, Parallel}

internal static class CombineTypeConverter
{
    public static string ConvertToString(CombineType method)
    {
        return method switch
        {
            CombineType.Series => "series",
            CombineType.Parallel => "parallel",
            _ => throw new ArgumentOutOfRangeException(nameof(method), method, null)
        };
    }

    public static CombineType ConvertFromString(string value)
    {
        return value switch
        {
            "series" => CombineType.Series,
            "parallel" => CombineType.Parallel,
            _ => throw new ArgumentException($"Unknown combine type: '{value}'", nameof(value))
        };
    }
}
