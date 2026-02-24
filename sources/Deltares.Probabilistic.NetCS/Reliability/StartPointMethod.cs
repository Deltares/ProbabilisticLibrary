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

public enum StartPointMethod
{
    FixedValue,
    RaySearch,
    SensitivitySearch,
    SphereSearch
}

public static class StartPointMethodConverter
{
    public static string ConvertToString(StartPointMethod method)
    {
        return method switch
        {
            StartPointMethod.FixedValue => "fixed_value",
            StartPointMethod.RaySearch => "ray_search",
            StartPointMethod.SensitivitySearch => "sensitivity_search",
            StartPointMethod.SphereSearch => "sphere_search",
            _ => throw new ArgumentOutOfRangeException(nameof(method), method, null)
        };
    }

    public static StartPointMethod ConvertFromString(string value)
    {
        return value switch
        {
            "fixed_value" => StartPointMethod.FixedValue,
            "ray_search" => StartPointMethod.RaySearch,
            "sensitivity_search" => StartPointMethod.SensitivitySearch,
            "sphere_search" => StartPointMethod.SphereSearch,
            _ => throw new ArgumentException($"Unknown start point method: '{value}'", nameof(value))
        };
    }
}
