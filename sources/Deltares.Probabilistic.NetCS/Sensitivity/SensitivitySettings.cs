using System;
using System.Collections.Generic;
using Deltares.Probabilistic.Model;
using Deltares.Probabilistic.Reliability;
using Deltares.Probabilistic.Utils;

namespace Deltares.Probabilistic.Sensitivity;

public class SensitivitySettings : IDisposable
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

    public void Dispose()
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