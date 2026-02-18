using System;
using Deltares.Probabilistic.Utils;

namespace Deltares.Probabilistic.Statistics;

public class ProbabilityValue : IDisposable
{
    private int id = 0;

    public ProbabilityValue()
    {
        this.id = Interface.Create("probability_value");
    }

    internal ProbabilityValue(int id)
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

    public double ReliabilityIndex
    {
        get { return Interface.GetValue(id, "reliability_index"); }
        set { Interface.SetValue(id, "reliability_index", value); }
    }

    public double ProbabilityOfFailure
    {
        get { return Interface.GetValue(id, "probability_of_failure"); }
        set { Interface.SetValue(id, "probability_of_failure", value); }
    }

    public double ProbabilityOfNonFailure
    {
        get { return Interface.GetValue(id, "probability_of_non_failure"); }
        set { Interface.SetValue(id, "probability_of_non_failure", value); }
    }

    public double ReturnPeriod
    {
        get { return Interface.GetValue(id, "return_period"); }
        set { Interface.SetValue(id, "return_period", value); }
    }
}