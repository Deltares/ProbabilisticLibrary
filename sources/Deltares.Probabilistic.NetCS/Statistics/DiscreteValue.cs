using System;
using Deltares.Probabilistic.Utils;

namespace Deltares.Probabilistic.Statistics;

public class DiscreteValue : IDisposable
{
    private int id = 0;

    public DiscreteValue()
    {
        this.id = Interface.Create("discrete_value");
    }

    public DiscreteValue(double x, double amount) : this()
    {
        this.X = x;
        this.Amount = amount;
    }

    internal DiscreteValue(int id)
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

    public double X
    {
        get { return Interface.GetValue(id, "x"); }
        set { Interface.SetValue(id, "x", value); }
    }

    public double Amount
    {
        get { return Interface.GetValue(id, "amount"); }
        set { Interface.SetValue(id, "amount", value); }
    }
}