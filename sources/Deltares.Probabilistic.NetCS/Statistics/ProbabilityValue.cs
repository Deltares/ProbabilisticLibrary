using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
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

public class CorrelationMatrix : IDisposable
{
    private int id = 0;

    public CorrelationMatrix()
    {
        this.id = Interface.Create("correlation_matrix");
    }

    internal CorrelationMatrix(int id)
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

    public void Initialize(IList<Stochast> stochasts)
    {
        Interface.SetArrayIntValue(id, "variables", stochasts.Select(p => p.GetId()).ToArray());
    }

    public double this[Stochast stochast1, Stochast stochast2]
    {
        get { return Interface.GetIndexedIndexedValue(id, "correlation", stochast1.GetId(), stochast2.GetId()); }
        set { Interface.SetIndexedIndexedValue(id, "correlation", stochast1.GetId(), stochast2.GetId(), value); }
    }
}