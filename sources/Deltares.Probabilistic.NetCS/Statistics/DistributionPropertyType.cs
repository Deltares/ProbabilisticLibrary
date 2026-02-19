using System;

namespace Deltares.Probabilistic.Statistics;

public enum DistributionPropertyType { Location, Scale, Minimum, Maximum, Shift, ShiftB, Shape, ShapeB, Observations };

internal static class DistributionPropertyConverter
{
    public static string ConvertToString(DistributionPropertyType propertyType)
    {
        switch (propertyType)
        {
            case DistributionPropertyType.Location: return "location";
            case DistributionPropertyType.Scale: return "scale";
            case DistributionPropertyType.Minimum: return "minimum";
            case DistributionPropertyType.Maximum: return "maximum";
            case DistributionPropertyType.Shift: return "shift";
            case DistributionPropertyType.ShiftB: return "shift_b";
            case DistributionPropertyType.Shape: return "shape";
            case DistributionPropertyType.ShapeB: return "shape_b";
            case DistributionPropertyType.Observations: return "observations";

            default:
                throw new ArgumentOutOfRangeException(nameof(propertyType), propertyType,
                    "Unknown distribution property type");
        }
    }

    public static DistributionPropertyType ConvertFromString(string value)
    {
        switch (value.ToLowerInvariant())
        {
            case "location": return DistributionPropertyType.Location;
            case "scale": return DistributionPropertyType.Scale;
            case "minimum": return DistributionPropertyType.Minimum;
            case "maximum": return DistributionPropertyType.Maximum;
            case "shift": return DistributionPropertyType.Shift;
            case "shift_b": return DistributionPropertyType.ShiftB;
            case "shape": return DistributionPropertyType.Shape;
            case "shape_b": return DistributionPropertyType.ShapeB;
            case "observations": return DistributionPropertyType.Observations;

            default:
                throw new ArgumentException($"Unknown distribution property type string: '{value}'", nameof(value));
        }
    }
}
