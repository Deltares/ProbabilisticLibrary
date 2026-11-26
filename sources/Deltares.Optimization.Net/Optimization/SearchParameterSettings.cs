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
using Deltares.Probabilistic.Model;
using Deltares.Probabilistic.Utils;

namespace Deltares.Probabilistic.Optimization;

public class SearchParameterSettings
{
    private int id = 0;
    private ModelParameter parameter = null;

    public SearchParameterSettings()
    {
        this.id = Interface.Create("search_parameter_settings");
    }

    internal SearchParameterSettings(int id)
    {
        this.id = id;
    }

    ~SearchParameterSettings()
    {
        Interface.Destroy(id);
    }

    internal int GetId()
    {
        return id;
    }

    public ModelParameter Parameter
    {
        get
        {
            if (parameter == null)
            {
                int parameterId = Interface.GetIdValue(id, "parameter");
                if (parameterId > 0)
                {
                    parameter = ObjectFactory.GetObject<ModelParameter>(parameterId);
                }
            }

            return parameter;
        }
        set
        {
            Interface.SetIntValue(id, "parameter", value.GetId());
            parameter = value;
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

    public int NumberOfValues
    {
        get { return Interface.GetIntValue(id, "number_of_values"); }
        set { Interface.SetIntValue(id, "number_of_values", value); }
    }

    public int NumberOfRefinements
    {
        get { return Interface.GetIntValue(id, "number_of_refinements"); }
        set { Interface.SetIntValue(id, "number_of_refinements", value); }
    }

    public bool IsMoveAllowed
    {
        get { return Interface.GetBoolValue(id, "is_move_allowed"); }
        set { Interface.SetBoolValue(id, "is_move_allowed", value); }
    }

    public double GradientStepSize
    {
        get { return Interface.GetValue(id, "gradient_step_size"); }
        set { Interface.SetValue(id, "gradient_step_size", value); }
    }

    /// <summary>
    /// Get the absolute value based on a ratio and the low and high boundaries
    /// </summary>
    /// <param name="ratio"></param>
    /// <returns></returns>
    public double GetAbsoluteValue(double ratio)
    {
        double diff = MaxValue - MinValue;
        return MinValue + ratio * diff;
    }

    /// <summary>
    /// Get the relative value based on an absolute value and the low and high boundaries
    /// </summary>
    /// <param name="value"></param>
    /// <returns></returns>
    public double GetRelativeValue(double value)
    {
        return (value - MinValue) / (MaxValue - MinValue);
    }

    /// <summary>
    /// Gets the start value
    /// </summary>
    /// <returns></returns>
    public double GetStartValue()
    {
        if (double.IsNaN(StartValue))
        {
            return (MinValue + MaxValue) / 2;
        }
        else
        {
            return StartValue;
        }
    }
}
