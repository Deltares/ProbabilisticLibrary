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

public class HistogramValue
{
    private int id = 0;

    public HistogramValue()
    {
        this.id = Interface.Create("histogram_value");
    }

    internal HistogramValue(int id)
    {
        this.id = id;
    }

    public HistogramValue(double lowerBound, double upperBound, double amount) : this()
    {
        LowerBound = lowerBound;
        UpperBound = upperBound;
        Amount = amount;
    }

    ~HistogramValue()
    {
        Interface.Destroy(id);
    }

    internal int GetId()
    {
        return id;
    }

    public double LowerBound
    {
        get { return Interface.GetValue(id, "lower_bound"); }
        set { Interface.SetValue(id, "lower_bound", value); }
    }

    public double UpperBound
    {
        get { return Interface.GetValue(id, "upper_bound"); }
        set { Interface.SetValue(id, "upper_bound", value); }
    }

    public double Amount
    {
        get { return Interface.GetValue(id, "amount"); }
        set { Interface.SetValue(id, "amount", value); }
    }

}