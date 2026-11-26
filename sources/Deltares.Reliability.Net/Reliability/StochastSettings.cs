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

namespace Deltares.Probabilistic.Reliability;

public class StochastSettings
{
    private int id = 0;
    private Stochast stochast = null;

    public StochastSettings()
    {
        this.id = Interface.Create("stochast_settings");
    }

    internal StochastSettings(int id)
    {
        this.id = id;
    }

    ~StochastSettings()
    {
        Interface.Destroy(id);
    }

    internal int GetId()
    {
        return id;
    }

    public Stochast Stochast
    {
        get
        {
            if (stochast == null)
            {
                int stochastId = Interface.GetIdValue(id, "variable");
                if (stochastId > 0)
                {
                    stochast = ObjectFactory.GetObject<Stochast>(stochastId);
                }
            }

            return stochast;
        }
        set
        {
            Interface.SetIntValue(id, "variable", value.GetId());
            stochast = value;
        }
    }

    public double MinValue
    {
        get { return Interface.GetValue(id, "min_value"); }
        set { Interface.SetValue(id, "min_value", value); }
    }

    public double MaxValue
    {
        get { return Interface.GetValue(id, "max_value"); }
        set { Interface.SetValue(id, "max_value", value); }
    }

    public double StartValue
    {
        get { return Interface.GetValue(id, "start_value"); }
        set { Interface.SetValue(id, "start_value", value); }
    }

    public double VarianceFactor
    {
        get { return Interface.GetValue(id, "variance_factor"); }
        set { Interface.SetValue(id, "variance_factor", value); }
    }

    public bool IsInitializationAllowed
    {
        get { return Interface.GetBoolValue(id, "is_initialization_allowed"); }
        set { Interface.SetBoolValue(id, "is_initialization_allowed", value); }
    }

    public bool IsVarianceAllowed
    {
        get { return Interface.GetBoolValue(id, "is_variance_allowed"); }
        set { Interface.SetBoolValue(id, "is_variance_allowed", value); }
    }

    public int Intervals
    {
        get { return Interface.GetIntValue(id, "intervals"); }
        set { Interface.SetIntValue(id, "intervals", value); }
    }
}

public interface IHasStochastSetting
{
    StochastSettings GetStochastSetting(Stochast stochast);
}
