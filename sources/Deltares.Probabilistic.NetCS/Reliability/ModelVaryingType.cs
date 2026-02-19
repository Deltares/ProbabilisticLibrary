using System;

namespace Deltares.Probabilistic.Reliability;

public enum ModelVaryingType {Monotone, Varying}

internal static class ModelVaryingTypeConverter
{
    public static string ConvertToString(ModelVaryingType method)
    {
        return method switch
        {
            ModelVaryingType.Monotone => "monotone",
            ModelVaryingType.Varying => "varying",
            _ => throw new ArgumentOutOfRangeException(nameof(method), method, null)
        };
    }

    public static ModelVaryingType ConvertFromString(string value)
    {
        return value switch
        {
            "monotone" => ModelVaryingType.Monotone,
            "varying" => ModelVaryingType.Varying,
            _ => throw new ArgumentException($"Unknown model varying type: '{value}'", nameof(value))
        };
    }
}
