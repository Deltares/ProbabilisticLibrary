using System;
using Deltares.Probabilistic.Utils;

namespace Deltares.Probabilistic.Statistics;

public class HistogramValue : IDisposable
{
    private int id = 0;

    public HistogramValue()
    {
        this.id = Interface.Create("histogram_value");
    }

    internal HistogramValue(int id)
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

    public double LowerBound
    {
        get { return Interface.GetValue(id, "lower_bound"); }
        set { Interface.SetValue(id, "lower_bound", value); }
    }

    public double UpperBound
    {
        get { return Interface.GetValue(id, "upper_bound"); }
        set { Interface.SetValue(id, "upper_bound", value); }
    }

    public double Amount
    {
        get { return Interface.GetValue(id, "amount"); }
        set { Interface.SetValue(id, "amount", value); }
    }

}