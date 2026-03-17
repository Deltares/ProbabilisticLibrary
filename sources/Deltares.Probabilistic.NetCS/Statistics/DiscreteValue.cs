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

namespace Deltares.Probabilistic.Statistics;

public class DiscreteValue
{
    private int id = 0;

    public DiscreteValue()
    {
        this.id = Interface.Create("discrete_value");
    }

    public DiscreteValue(double x, double amount) : this()
    {
        this.X = x;
        this.Amount = amount;
    }

    internal DiscreteValue(int id)
    {
        this.id = id;
    }

    ~DiscreteValue()
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

    public double Amount
    {
        get { return Interface.GetValue(id, "amount"); }
        set { Interface.SetValue(id, "amount", value); }
    }

    public double NormalizedAmount
    {
        get { return Interface.GetValue(id, "normalized_amount"); }
    }

    public double CumulativeAmount
    {
        get { return Interface.GetValue(id, "cumulative_amount"); }
    }
}