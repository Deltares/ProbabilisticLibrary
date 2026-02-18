using System;
using Deltares.Probabilistic.Statistics;
using Deltares.Probabilistic.Utils;

namespace Deltares.Probabilistic.Reliability;

public class StochastSettings : IDisposable
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

    public void Dispose()
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
                    stochast = new Stochast(stochastId);
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