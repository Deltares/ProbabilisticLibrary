using System;
using System.Collections.Generic;
using System.Linq;
using Deltares.Probabilistic.Utils;

namespace Deltares.Probabilistic.Statistics;

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

    public double GetCorrelation(Stochast stochast1, Stochast stochast2)
    {
        return Interface.GetIndexedIndexedValue(id, "correlation", stochast1.GetId(), stochast2.GetId());
    }

    public void SetCorrelation(Stochast stochast1, Stochast stochast2, double value)
    {
        Interface.SetIndexedIndexedValue(id, "correlation", stochast1.GetId(), stochast2.GetId(), value);
    }

    public double this[Stochast stochast1, Stochast stochast2]
    {
        get { return GetCorrelation(stochast1, stochast2); }
        set { SetCorrelation(stochast1, stochast2, value); }
    }

    public void ResolveConflictingCorrelations()
    {
        Interface.Execute(id, "resolve_conflicting_correlations"); 
    }

    public bool IsIdentity()
    {
        return Interface.GetBoolValue(id, "is_identity");
    }

    public int CountCorrelations()
    {
        return Interface.GetIntValue(id, "count_correlations");
    }

    public bool HasConflictingCorrelations()
    {
        return Interface.GetBoolValue(id, "has_conflicting_correlations");
    }
}