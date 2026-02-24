using System;
using Deltares.Probabilistic.Model;
using Deltares.Probabilistic.Statistics;
using Deltares.Probabilistic.Utils;

namespace Deltares.Probabilistic.Sensitivity;

public class SensitivityValue : IDisposable
{
    private int id = 0;
    private Stochast stochast = null;
    private IStochastProvider stochastProvider = null;

    public SensitivityValue()
    {
        this.id = Interface.Create("sensitivity_value");
    }

    internal SensitivityValue(int id, IStochastProvider stochastProvider)
    {
        this.id = id;
        this.stochastProvider = stochastProvider;
    }

    public void Dispose()
    {
        Interface.Destroy(id);
    }

    internal int GetId()
    {
        return id;
    }

    public Stochast Parameter
    {
        get
        {
            if (stochast == null)
            {
                int stochastId = Interface.GetIdValue(id, "variable");
                stochast = stochastProvider?.GetStochast(stochastId) ?? new Stochast(stochastId);
                stochastProvider = null; // not needed any more
            }

            return stochast;
        }
        set
        {
            Interface.SetIntValue(id, "variable", value.GetId());
            stochast = value;
        }
    }

    public double Low
    {
        get { return Interface.GetValue(id, "low"); }
        set { Interface.SetValue(id, "low", value); }
    }

    public double Medium
    {
        get { return Interface.GetValue(id, "medium"); }
        set { Interface.SetValue(id, "medium", value); }
    }

    public double High
    {
        get { return Interface.GetValue(id, "high"); }
        set { Interface.SetValue(id, "high", value); }
    }

    public double FirstOrderIndex
    {
        get { return Interface.GetValue(id, "first_order_index"); }
        set { Interface.SetValue(id, "first_order_index", value); }
    }

    public double TotalIndex
    {
        get { return Interface.GetValue(id, "total_index"); }
        set { Interface.SetValue(id, "total_index", value); }
    }
}