using System.Collections.Generic;
using System.Linq;
using Deltares.Probabilistic.Statistics;
using Deltares.Probabilistic.Utils;

namespace Deltares.Probabilistic.Model;

public class StochastPoint : IStochastProvider
{
    private int id = 0;
    private List<StochastPointAlpha> alphas = null;
    private readonly IStochastProvider stochastProvider = null;

    public StochastPoint()
    {
        this.id = Interface.Create("stochast_point");
    }

    internal StochastPoint(int id, IStochastProvider stochastProvider)
    {
        if (id > 0)
        {
            this.id = id;
        }

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

    internal void SetId(int id)
    {
        this.id = id;
    }

    public double Beta
    {
        get { return Interface.GetValue(id, "beta"); }
        set { Interface.SetValue(id, "beta", value); }
    }

    public IList<StochastPointAlpha> Alphas
    {
        get
        {
            if (alphas == null)
            {
                alphas = new List<StochastPointAlpha>();

                int[] alphaIds = Interface.GetArrayIdValue(id, "alphas");
                foreach (int alphaId in alphaIds)
                {
                    alphas.Add(new StochastPointAlpha(alphaId, stochastProvider));
                }
            }

            return alphas;
        }
    }

    public ModelSample GetModelSample()
    {
        return new ModelSample(this.Alphas.Select(p => p.X).ToArray());
    }

    internal IStochastProvider GetStochastProvider()
    {
        return stochastProvider;
    }

    public Stochast GetStochast(int stochastId)
    {
        if (stochastProvider != null)
        {
            return stochastProvider.GetStochast(stochastId);
        }
        else
        {
            StochastPointAlpha alpha = this.Alphas.FirstOrDefault(p => p.Parameter.GetId() == stochastId);
            return alpha?.Parameter ?? null;
        }
    }
}