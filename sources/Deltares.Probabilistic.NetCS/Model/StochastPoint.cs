using System.Collections.Generic;
using System.Linq;
using Deltares.Probabilistic.Reliability;
using Deltares.Probabilistic.Statistics;
using Deltares.Probabilistic.Utils;

namespace Deltares.Probabilistic.Model;

public class StochastPoint : IStochastProvider
{
    private int id = 0;
    private CallBackList<StochastPointAlpha> alphas = null;
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
                alphas = new CallBackList<StochastPointAlpha>(AlphasChanged);

                int[] alphaIds = Interface.GetArrayIdValue(id, "alphas");
                foreach (int alphaId in alphaIds)
                {
                    alphas.AddWithoutCallBack(new StochastPointAlpha(alphaId, stochastProvider));
                }
            }

            return alphas;
        }
    }

    private void AlphasChanged(ListOperationType listOperation, StochastPointAlpha item)
    {
        Interface.SetArrayIntValue(id, "alphas", this.alphas.Select(p => p.GetId()).ToArray());
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