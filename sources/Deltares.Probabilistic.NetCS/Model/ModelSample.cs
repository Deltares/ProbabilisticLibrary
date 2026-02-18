using System;
using Deltares.Probabilistic.Utils;

namespace Deltares.Probabilistic.Model;

public class ModelSample : IDisposable
{
    private int id = 0;
    private double[] values = null;
    private double[] outputValues = null;

    public ModelSample()
    {
        this.id = Interface.Create("model_project");
    }

    internal ModelSample(int id)
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

    public double[] Values
    {
        get { return values; }
    }

    public double[] OutputValues
    {
        get { return outputValues; }
    }

    public int Iteration
    {
        get { return Interface.GetIntValue(id, "iteration"); }
        set { Interface.SetIntValue(id, "iteration", value); }
    }

    public double Beta
    {
        get { return Interface.GetValue(id, "beta"); }
        set { Interface.SetValue(id, "beta", value); }
    }

    public double Z
    {
        get { return Interface.GetValue(id, "z"); }
        set { Interface.SetValue(id, "z", value); }
    }

    public double Weight
    {
        get { return Interface.GetValue(id, "weight"); }
        set { Interface.SetValue(id, "weight", value); }
    }

    public bool AllowProxy
    {
        get { return Interface.GetBoolValue(id, "allow_proxy"); }
        set { Interface.SetBoolValue(id, "allow_proxy", value); }
    }

    public bool UsedProxy
    {
        get { return Interface.GetBoolValue(id, "used_proxy"); }
        set { Interface.SetBoolValue(id, "used_proxy", value); }
    }

    public bool IsRestartRequired
    {
        get { return Interface.GetBoolValue(id, "is_restart_required"); }
        set { Interface.SetBoolValue(id, "is_restart_required", value); }
    }

    public int Tag
    {
        get { return Interface.GetIntValue(id, "tag"); }
        set { Interface.SetIntValue(id, "tag", value); }
    }
}