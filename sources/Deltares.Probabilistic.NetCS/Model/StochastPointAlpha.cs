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
using Deltares.Probabilistic.Statistics;
using Deltares.Probabilistic.Utils;

namespace Deltares.Probabilistic.Model;

public class StochastPointAlpha
{
    private int id = 0;
    private Stochast stochast = null;
    private IStochastProvider stochastProvider = null;

    public StochastPointAlpha()
    {
        this.id = Interface.Create("alpha");
    }

    internal StochastPointAlpha(int id, IStochastProvider stochastProvider)
    {
        this.id = id;
        this.stochastProvider = stochastProvider;
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
                stochast = stochastProvider?.GetStochast(stochastId) ?? new Stochast(stochastId);
                stochastProvider = null; // not needed any more
            }

            return stochast;
        }
        set
        {
            Interface.SetIntValue(id, "variable", value.GetId());
            stochast = value;
        }
    }

    public int Index
    {
        get { return Interface.GetIntValue(id, "index"); }
        set { Interface.SetIntValue(id, "index", value); }
    }

    public string Identifier
    {
        get { return Interface.GetStringValue(id, "identifier"); }
        set { Interface.SetStringValue(id, "identifier", value); }
    }

    public double Alpha
    {
        get { return Interface.GetValue(id, "alpha"); }
        set { Interface.SetValue(id, "alpha", value); }
    }

    public double AlphaCorrelated
    {
        get { return Interface.GetValue(id, "alpha_correlated"); }
        set { Interface.SetValue(id, "alpha_correlated", value); }
    }

    public double InfluenceFactor
    {
        get { return Interface.GetValue(id, "influence_factor"); }
        set { Interface.SetValue(id, "influence_factor", value); }
    }

    public double X
    {
        get { return Interface.GetValue(id, "x"); }
        set { Interface.SetValue(id, "x", value); }
    }

    public double U
    {
        get { return Interface.GetValue(id, "u"); }
        set { Interface.SetValue(id, "u", value); }
    }
}