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
