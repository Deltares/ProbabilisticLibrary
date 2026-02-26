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
using Deltares.Probabilistic.Model;
using Deltares.Probabilistic.Statistics;
using Deltares.Probabilistic.Utils;

namespace Deltares.Probabilistic.Sensitivity;

public class SensitivityValue : IDisposable
{
    private int id = 0;
    private Stochast stochast = null;

    public SensitivityValue()
    {
        this.id = Interface.Create("sensitivity_value");
    }

    internal SensitivityValue(int id)
    {
        this.id = id;
    }

    public void Dispose()
    {
        Interface.Destroy(id);
    }

    internal int GetId()
    {
        return id;
    }

    public Stochast Parameter
    {
        get
        {
            if (stochast == null)
            {
                int stochastId = Interface.GetIdValue(id, "variable");
                stochast = ObjectFactory.GetObject<Stochast>(stochastId);
            }

            return stochast;
        }
        set
        {
            Interface.SetIntValue(id, "variable", value.GetId());
            stochast = value;
        }
    }

    public double Low
    {
        get { return Interface.GetValue(id, "low"); }
        set { Interface.SetValue(id, "low", value); }
    }

    public double Medium
    {
        get { return Interface.GetValue(id, "medium"); }
        set { Interface.SetValue(id, "medium", value); }
    }

    public double High
    {
        get { return Interface.GetValue(id, "high"); }
        set { Interface.SetValue(id, "high", value); }
    }

    public double FirstOrderIndex
    {
        get { return Interface.GetValue(id, "first_order_index"); }
        set { Interface.SetValue(id, "first_order_index", value); }
    }

    public double TotalIndex
    {
        get { return Interface.GetValue(id, "total_index"); }
        set { Interface.SetValue(id, "total_index", value); }
    }
}