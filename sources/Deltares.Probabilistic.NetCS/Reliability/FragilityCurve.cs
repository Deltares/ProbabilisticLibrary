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
using Deltares.Probabilistic.Model;
using Deltares.Probabilistic.Statistics;
using Deltares.Probabilistic.Utils;
using System.Collections.Generic;

namespace Deltares.Probabilistic.Reliability;

public class FragilityCurve : Stochast
{
    private int id = 0;

    public FragilityCurve() : base(-1)
    {
        id = Interface.Create("fragility_curve");
        base.SetId(id);
    }

    internal FragilityCurve(int id) : base(id)
    {
        this.id = id;
    }

    ~FragilityCurve()
    {
        Interface.Destroy(id);
    }

    internal int GetId()
    {
        return id;
    }

    public bool Fixed
    {
        get { return Interface.GetBoolValue(id, "fixed"); }
        set { Interface.SetBoolValue(id, "fixed", value); }
    }

    public double FixedValue
    {
        get { return Interface.GetValue(id, "fixed_value"); }
        set { Interface.SetValue(id, "fixed_value", value); }
    }

    public virtual StochastPoint GetRealization(double x)
    {
        Interface.SetValue(id, "design_point_x", x);
        int designPointId = Interface.GetIdValue(id, "design_point_x");

        return new DesignPoint(designPointId);
    }
}