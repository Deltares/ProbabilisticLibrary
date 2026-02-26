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
using Deltares.Probabilistic.Utils;

namespace Deltares.Probabilistic.Statistics;

public class ProbabilityValue
{
    private int id = 0;

    public ProbabilityValue()
    {
        this.id = Interface.Create("probability_value");
    }

    internal ProbabilityValue(int id)
    {
        this.id = id;
    }

    ~ProbabilityValue()
    {
        Interface.Destroy(id);
    }

    internal int GetId()
    {
        return id;
    }

    public double ReliabilityIndex
    {
        get { return Interface.GetValue(id, "reliability_index"); }
        set { Interface.SetValue(id, "reliability_index", value); }
    }

    public double ProbabilityOfFailure
    {
        get { return Interface.GetValue(id, "probability_of_failure"); }
        set { Interface.SetValue(id, "probability_of_failure", value); }
    }

    public double ProbabilityOfNonFailure
    {
        get { return Interface.GetValue(id, "probability_of_non_failure"); }
        set { Interface.SetValue(id, "probability_of_non_failure", value); }
    }

    public double ReturnPeriod
    {
        get { return Interface.GetValue(id, "return_period"); }
        set { Interface.SetValue(id, "return_period", value); }
    }
}