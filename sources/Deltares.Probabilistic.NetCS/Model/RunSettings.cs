using System.Collections.Generic;
using Deltares.Probabilistic.Model;
using Deltares.Probabilistic.Reliability;
using Deltares.Probabilistic.Utils;

public class RunSettings
{
    private int id = 0;
    private List<StochastSettings> stochastSettings = null;

    public RunSettings()
    {
        this.id = Interface.Create("run_settings");
    }

    internal RunSettings(int id)
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