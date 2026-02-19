using System;

namespace Deltares.Probabilistic.Reliability;

public enum CombineMethod {ImportanceSampling, DirectionaSampling, Hohenbichler, HohenbichlerFORM}

internal static class CombineMethodConverter
{
    public static string ConvertToString(CombineMethod method)
    {
        return method switch
        {
            CombineMethod.ImportanceSampling => "importance_sampling",
            CombineMethod.DirectionaSampling => "directional_sampling",
            CombineMethod.Hohenbichler => "hohenbichler",
            CombineMethod.HohenbichlerFORM => "Hohenbichler_form",
            _ => throw new ArgumentOutOfRangeException(nameof(method), method, null)
        };
    }

    public static CombineMethod ConvertFromString(string value)
    {
        return value switch
        {
            "importance_sampling" => CombineMethod.ImportanceSampling,
            "directional_sampling" => CombineMethod.DirectionaSampling,
            "hohenbichler" => CombineMethod.Hohenbichler,
            "hohenbichler_form" => CombineMethod.HohenbichlerFORM,
            _ => throw new ArgumentException($"Unknown combine method: '{value}'", nameof(value))
        };
    }
}
