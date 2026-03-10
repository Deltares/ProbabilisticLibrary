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

public class Scenario
{
    private int id = 0;
    private Stochast parameter = null;

    public Scenario()
    {
        this.id = Interface.Create("scenario");
    }

    internal Scenario(int id)
    {
        this.id = id;
    }

    ~Scenario()
    {
        Interface.Destroy(id);
    }

    internal int GetId()
    {
        return id;
    }

    public string Name
    {
        get { return Interface.GetStringValue(id, "name"); }
        set { Interface.SetStringValue(id, "name", value); }
    }

    public double Probability
    {
        get { return Interface.GetValue(id, "probability"); }
        set { Interface.SetValue(id, "probability", value); }
    }

    public double PhysicalValue
    {
        get { return Interface.GetValue(id, "physical_value"); }
        set { Interface.SetValue(id, "physical_value", value); }
    }

    public Stochast Parameter
    {
        get
        {
            if (parameter == null)
            {
                int parameterId = Interface.GetIdValue(id, "parameter");
                if (parameterId > 0)
                {
                    this.parameter = ObjectFactory.GetObject<Stochast>(parameterId);
                }
            }
            return parameter;
        }
        set
        {
            parameter = value;
            Interface.SetValue(id, "parameter", value.GetId());
        }
    }
}