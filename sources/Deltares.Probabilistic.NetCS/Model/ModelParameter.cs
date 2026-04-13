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

namespace Deltares.Probabilistic.Model;

public class ModelParameter
{
    private int id = 0;
    private string name = string.Empty;
    private bool nameAssigned = false;

    public ModelParameter()
    {
        this.id = Interface.Create("model_parameter");

        if (id == 63)
        {
            int k = 1;
        }
    }

    internal ModelParameter(int id)
    {
        this.id = id;

        if (id == 63)
        {
            int k = 1;
        }

    }

    ~ModelParameter()
    {
        Interface.Destroy(id);
    }

    public int GetId()
    {
        return id;
    }

    public string Name
    {
        get
        {
            if (!nameAssigned)
            {
                name = Interface.GetStringValue(id, "name");
                nameAssigned = true;
            }

            return name;
        }
        set
        {
            Interface.SetStringValue(id, "name", value);
            name = value;
            nameAssigned = true;
        }
    }

    public int Index
    {
        get { return Interface.GetIntValue(id, "index"); }
        set { Interface.SetIntValue(id, "index", value); }
    }

    public double DefaultValue
    {
        get { return Interface.GetValue(id, "default_value"); }
        set { Interface.SetValue(id, "default_value", value); }
    }

    public bool IsArray
    {
        get { return Interface.GetBoolValue(id, "is_array"); }
        set { Interface.SetBoolValue(id, "is_array", value); }
    }

    public int ArraySize
    {
        get { return Interface.GetIntValue(id, "array_size"); }
        set { Interface.SetIntValue(id, "array_size", value); }
    }
}