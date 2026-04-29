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

public enum SampleMethod
{
    MarkovChain,
    AdaptiveConditional
};

public static class SampleMethodConverter
{
    public static string ConvertToString(SampleMethod method)
    {
        return method switch
        {
            SampleMethod.MarkovChain => "markov_chain",
            SampleMethod.AdaptiveConditional => "adaptive_conditional",
            _ => throw new ArgumentOutOfRangeException(nameof(method), method, null)
        };
    }

    public static SampleMethod ConvertFromString(string value)
    {
        return value switch
        {
            "markov_chain" => SampleMethod.MarkovChain,
            "adaptive_conditional" => SampleMethod.AdaptiveConditional,
            _ => throw new ArgumentException($"Unknown sample method: '{value}'", nameof(value))
        };
    }
}
