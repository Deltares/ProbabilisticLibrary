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
using Deltares.Probabilistic.Utils;

namespace Deltares.Probabilistic.Reliability;

public class ReliabilityResult
{
    public ReliabilityResult()
    {
    }

    internal ReliabilityResult(int id)
    {
        this.Index = Interface.GetIntValue(id, "index");
        this.ReliabilityIndex = Interface.GetValue(id, "reliability_index");
        this.Convergence = Interface.GetValue(id, "convergence");
        this.Variation = Interface.GetValue(id, "variation");
        this.Contribution = Interface.GetValue(id, "contribution");
        this.Samples = Interface.GetIntValue(id, "samples");

        Interface.Destroy(id);
    }

    public int Index { get; set; }
    public double ReliabilityIndex { get; set; }
    public double Convergence { get; set; }
    public double Variation { get; set; }
    public double Contribution { get; set; }
    public int Samples { get; set; }
    public double Difference { get; set; }
    public double LowReliability { get; set; }
    public double HighReliability { get; set; }
}