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
using Deltares.Probabilistic.Logging;
using Deltares.Probabilistic.Utils;

namespace Deltares.Probabilistic.Sensitivity;

public class SensitivitySettings
{
    private int id = 0;

    public SensitivitySettings()
    {
        this.id = Interface.Create("sensitivity_settings");
    }

    internal SensitivitySettings(int id)
    {
        this.id = id;
    }

    ~SensitivitySettings()
    {
        Interface.Destroy(id);
    }

    internal int GetId()
    {
        return id;
    }

    public SensitivityMethod SensitivityMethod
    {
        get { return SensitivityMethodConverter.ConvertFromString(Interface.GetStringValue(id, "sensitivity_method")); }
        set { Interface.SetStringValue(id, "sensitivity_method", SensitivityMethodConverter.ConvertToString(value)); }
    }

    public int MaxParallelProcesses
    {
        get { return Interface.GetIntValue(id, "max_parallel_processes"); }
        set { Interface.SetIntValue(id, "max_parallel_processes", value); }
    }

    public bool SaveRealizations
    {
        get { return Interface.GetBoolValue(id, "save_realizations"); }
        set { Interface.SetBoolValue(id, "save_realizations", value); }
    }

    public bool SaveMessages
    {
        get { return Interface.GetBoolValue(id, "save_messages"); }
        set { Interface.SetBoolValue(id, "save_messages", value); }
    }

    public bool ReuseCalculations
    {
        get { return Interface.GetBoolValue(id, "reuse_calculations"); }
        set { Interface.SetBoolValue(id, "reuse_calculations", value); }
    }

    public MessageType LowestMessageType
    {
        get { return MessageTypeConverter.ConvertFromString(Interface.GetStringValue(id, "lowest_message_type")); }
        set { Interface.SetStringValue(id, "lowest_message_type", MessageTypeConverter.ConvertToString(value)); }
    }

    public int MaxChunkSize
    {
        get { return Interface.GetIntValue(id, "max_chunk_size"); }
        set { Interface.SetIntValue(id, "max_chunk_size", value); }
    }

    public double LowValue
    {
        get { return Interface.GetValue(id, "low_value"); }
        set { Interface.SetValue(id, "low_value", value); }
    }

    public double HighValue
    {
        get { return Interface.GetValue(id, "high_value"); }
        set { Interface.SetValue(id, "high_value", value); }
    }

    public int Iterations
    {
        get { return Interface.GetIntValue(id, "iterations"); }
        set { Interface.SetIntValue(id, "iterations", value); }
    }
}