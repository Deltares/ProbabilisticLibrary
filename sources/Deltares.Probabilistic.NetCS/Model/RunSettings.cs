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

public class RunSettings
{
    private int id = 0;

    public RunSettings()
    {
        this.id = Interface.Create("run_project_settings");
    }

    internal RunSettings(int id)
    {
        this.id = id;
    }

    ~RunSettings()
    {
        Interface.Destroy(id);
    }

    internal int GetId()
    {
        return id;
    }

    public RunValuesType RunValuesType
    {
        get { return RunValuesTypeConverter.ConvertFromString(Interface.GetStringValue(id, "run_values_type")); }
        set { Interface.SetStringValue(id, "run_values_type", RunValuesTypeConverter.ConvertToString(value)); }
    }

    public bool ReuseCalculations
    {
        get { return Interface.GetBoolValue(id, "reuse_calculations"); }
        set { Interface.SetBoolValue(id, "reuse_calculations", value); }
    }
}