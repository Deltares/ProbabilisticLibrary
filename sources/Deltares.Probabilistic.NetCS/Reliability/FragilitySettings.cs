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

public class FragilitySettings
{
    private int id = 0;

    public FragilitySettings()
    {
        this.id = Interface.Create("fragility_settings");
    }

    internal FragilitySettings(int id)
    {
        this.id = id;
    }

    ~FragilitySettings()
    {
        Interface.Destroy(id);
    }

    internal int GetId()
    {
        return id;
    }

    public double StepSize
    {
        get { return Interface.GetValue(id, "step_size"); }
        set { Interface.SetValue(id, "step_size", value); }
    }

    public DesignPointMethod DesignPointMethod
    {
        get { return DesignPointMethodConverter.ConvertFromString(Interface.GetStringValue(id, "design_point_method")); }
        set { Interface.SetStringValue(id, "design_point_method", DesignPointMethodConverter.ConvertToString(value)); }
    }
}