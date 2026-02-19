using System;
using Deltares.Probabilistic.Utils;

namespace Deltares.Probabilistic.Reliability;

public class CombineSettings : IDisposable
{
    private int id = 0;

    public CombineSettings()
    {
        this.id = Interface.Create("combine_settings");
    }

    internal CombineSettings(int id)
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

    public CombineType CombineType
    {
        get { return CombineTypeConverter.ConvertFromString(Interface.GetStringValue(id, "combine_type")); }
        set { Interface.SetStringValue(id, "combine_type", CombineTypeConverter.ConvertToString(value)); }
    }

    public CombineMethod CombineMethod
    {
        get { return CombineMethodConverter.ConvertFromString(Interface.GetStringValue(id, "combine_type")); }
        set { Interface.SetStringValue(id, "combine_type", CombineMethodConverter.ConvertToString(value)); }
    }
}