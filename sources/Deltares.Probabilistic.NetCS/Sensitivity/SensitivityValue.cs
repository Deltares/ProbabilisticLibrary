using System;
using Deltares.Probabilistic.Utils;

namespace Deltares.Probabilistic.Sensitivity;

public class SensitivityValue : IDisposable
{
    private int id = 0;

    public SensitivityValue()
    {
        this.id = Interface.Create("sensitivity_value");
    }

    internal SensitivityValue(int id)
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

    public double Variable
    {
        get { return Interface.GetValue(id, "variable"); }
        set { Interface.SetValue(id, "variable", value); }
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