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

public class ProbabilityLimitStateFunction : LimitStateFunction
{
    private int id = 0;
    private FragilityCurve fragilityCurve = null;

    public ProbabilityLimitStateFunction() : base(-1)
    {
        this.id = Interface.Create("probability_limit_state_function");
        base.SetId(id);
    }

    internal ProbabilityLimitStateFunction(int id) : base(id)
    {
        this.id = id;
    }

    ~ProbabilityLimitStateFunction()
    {
        Interface.Destroy(id);
    }

    public FragilityCurve FragilityCurve
    {
        get
        {
            if (fragilityCurve == null)
            {
                int fragilityCurveId = Interface.GetIdValue(id, "fragility_curve");
                fragilityCurve = new FragilityCurve(fragilityCurveId);
            }

            return fragilityCurve;
        }
        set
        {
            fragilityCurve = value;
            Interface.SetIntValue(id, "fragility_curve", value?.GetId() ?? 0);
        }
    }
}