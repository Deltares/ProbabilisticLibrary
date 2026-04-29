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

namespace Deltares.Probabilistic.Reliability;

public enum ExcludingCombineType {WeightedSum, HohenbichlerExcluding }

internal static class ExcludingCombineTypeConverter
{
    public static string ConvertToString(ExcludingCombineType method)
    {
        return method switch
        {
            ExcludingCombineType.WeightedSum => "weighted_sum",
            ExcludingCombineType.HohenbichlerExcluding => "hohenbichler_excluding",
            _ => throw new ArgumentOutOfRangeException(nameof(method), method, null)
        };
    }

    public static ExcludingCombineType ConvertFromString(string value)
    {
        return value switch
        {
            "weighted_sum" => ExcludingCombineType.WeightedSum,
            "hohenbichler_excluding" => ExcludingCombineType.HohenbichlerExcluding,
            _ => throw new ArgumentException($"Unknown excluding combine type: '{value}'", nameof(value))
        };
    }
}
