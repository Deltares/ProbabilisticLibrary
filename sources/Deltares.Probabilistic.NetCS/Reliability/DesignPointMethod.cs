using System;

namespace Deltares.Probabilistic.Reliability;

public enum DesignPointMethod
{
    CentreOfGravity,
    CentreOfAngles,
    NearestToMean
}

public static class DesignPointMethodConverter
{
    public static string ConvertToString(DesignPointMethod method)
    {
        return method switch
        {
            DesignPointMethod.CentreOfGravity => "centre_of_gravity",
            DesignPointMethod.CentreOfAngles => "centre_of_angles",
            DesignPointMethod.NearestToMean => "nearest_to_mean",
            _ => throw new ArgumentOutOfRangeException(nameof(method), method, null)
        };
    }

    public static DesignPointMethod ConvertFromString(string value)
    {
        return value switch
        {
            "centre_of_gravity" => DesignPointMethod.CentreOfGravity,
            "centre_of_angles" => DesignPointMethod.CentreOfAngles,
            "nearest_to_mean" => DesignPointMethod.NearestToMean,
            _ => throw new ArgumentException($"Unknown design point method: '{value}'", nameof(value))
        };
    }
}
