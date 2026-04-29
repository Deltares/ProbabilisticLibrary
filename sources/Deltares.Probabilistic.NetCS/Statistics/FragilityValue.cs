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
using Deltares.Probabilistic.Reliability;
using Deltares.Probabilistic.Utils;

namespace Deltares.Probabilistic.Statistics;

public class FragilityValue
{
    private int id = 0;
    private DesignPoint designPoint = null;

    public FragilityValue()
    {
        this.id = Interface.Create("fragility_value");
    }

    internal FragilityValue(int id)
    {
        this.id = id;
    }

    ~FragilityValue()
    {
        Interface.Destroy(id);
    }

    internal int GetId()
    {
        return id;
    }

    public double X
    {
        get { return Interface.GetValue(id, "x"); }
        set { Interface.SetValue(id, "x", value); }
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

    public DesignPoint DesignPoint
    {
        get
        {
            if (designPoint == null)
            {
                int designPointId = Interface.GetIdValue(id, "design_point");
                designPoint = ObjectFactory.GetObject<DesignPoint>(designPointId);
            }

            return designPoint;
        }
        set
        {
            Interface.SetIntValue(id, "design_point", value.GetId());
            designPoint = value;
        }
    }
}