// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of the Probabilistic Library.
//
// The Probabilistic Library is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// All names, logos, and references to "Deltares" are registered trademarks of
// Stichting Deltares and remain full property of Stichting Deltares at all times.
// All rights reserved.
//
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
