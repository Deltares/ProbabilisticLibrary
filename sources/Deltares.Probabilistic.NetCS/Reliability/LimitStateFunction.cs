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

public class LimitStateFunction
{
    private int id = 0;

    public LimitStateFunction()
    {
        this.id = Interface.Create("limit_state_function");
    }

    internal LimitStateFunction(int id)
    {
        this.id = id;
    }

    ~LimitStateFunction()
    {
        Interface.Destroy(id);
    }

    internal int GetId()
    {
        return id;
    }

    internal void SetId(int id)
    {
        this.id = id;
    }

    public double CriticalValue
    {
        get { return Interface.GetValue(id, "critical_value"); }
        set { Interface.SetValue(id, "critical_value", value); }
    }

    public bool UseCompareParameter
    {
        get { return Interface.GetBoolValue(id, "use_compare_parameter"); }
        set { Interface.SetBoolValue(id, "use_compare_parameter", value); }
    }

    public string Parameter
    {
        get { return Interface.GetStringValue(id, "parameter"); }
        set { Interface.SetStringValue(id, "parameter", value); }
    }

    public string CompareParameter
    {
        get { return Interface.GetStringValue(id, "compare_parameter"); }
        set { Interface.SetStringValue(id, "compare_parameter", value); }
    }

    public bool Normalize
    {
        get { return Interface.GetBoolValue(id, "normalize"); }
        set { Interface.SetBoolValue(id, "normalize", value); }
    }

    public CompareType CompareType
    {
        get { return CompareTypeConverter.ConvertFromString(Interface.GetStringValue(id, "compare_type")); }
        set { Interface.SetStringValue(id, "compare_type", CompareTypeConverter.ConvertToString(value)); }
    }
}