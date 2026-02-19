using System;

namespace Deltares.Probabilistic.Statistics;

public enum ConstantParameterType { Deviation, VariationCoefficient };

internal static class ConstantParameterTypeConverter
{
    public static string ConvertToString(ConstantParameterType method)
    {
        return method switch
        {
            ConstantParameterType.Deviation => "deviation",
            ConstantParameterType.VariationCoefficient => "variation_coefficient",
            _ => throw new ArgumentOutOfRangeException(nameof(method), method, null)
        };
    }

    public static ConstantParameterType ConvertFromString(string value)
    {
        return value switch
        {
            "deviation" => ConstantParameterType.Deviation,
            "variation_coefficient" => ConstantParameterType.VariationCoefficient,
            _ => throw new ArgumentException($"Unknown gradient method: '{value}'", nameof(value))
        };
    }
}
