using System;
using Deltares.Probabilistic.Utils;

namespace Deltares.Probabilistic.Model;

public class ModelParameter : IDisposable
{
    private int id = 0;

    public ModelParameter()
    {
        this.id = Interface.Create("model_parameter");
    }

    internal ModelParameter(int id)
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

    public string Name
    {
        get { return Interface.GetStringValue(id, "name"); }
        set { Interface.SetStringValue(id, "name", value); }
    }

    public int Index
    {
        get { return Interface.GetIntValue(id, "index"); }
        set { Interface.SetIntValue(id, "index", value); }
    }

    public double DefaultValue
    {
        get { return Interface.GetValue(id, "default_value"); }
        set { Interface.SetValue(id, "default_value", value); }
    }

    public bool IsArray
    {
        get { return Interface.GetBoolValue(id, "is_array"); }
        set { Interface.SetBoolValue(id, "is_array", value); }
    }

    public int ArraySize
    {
        get { return Interface.GetIntValue(id, "array_size"); }
        set { Interface.SetIntValue(id, "array_size", value); }
    }
}