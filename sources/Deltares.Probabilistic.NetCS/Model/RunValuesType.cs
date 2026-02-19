using System;

namespace Deltares.Probabilistic.Model;

public enum RunValuesType
{
    MedianValues,
    MeanValues,
    DesignValues
}

internal static class RunValuesTypeConverter
{
    public static string ConvertToString(RunValuesType type)
    {
        return type switch
        {
            RunValuesType.MedianValues => "median_values",
            RunValuesType.MeanValues => "mean_values",
            RunValuesType.DesignValues => "design_values",
            _ => throw new ArgumentOutOfRangeException(nameof(type), type, null)
        };
    }

    public static RunValuesType ConvertFromString(string value)
    {
        return value.ToLowerInvariant() switch
        {
            "median_values" => RunValuesType.MedianValues,
            "mean_values" => RunValuesType.MeanValues,
            "design_values" => RunValuesType.DesignValues,
            _ => throw new ArgumentException($"Unknown run values type: {value}")
        };
    }
}
