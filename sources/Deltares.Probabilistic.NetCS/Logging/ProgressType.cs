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
