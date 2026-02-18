using Deltares.Probabilistic.Statistics;
using Deltares.Probabilistic.Utils;

namespace Deltares.Probabilistic.Model;

public class StochastPointAlpha
{
    private int id = 0;
    private Stochast stochast = null;

    public StochastPointAlpha()
    {
        this.id = Interface.Create("alpha");
    }

    internal StochastPointAlpha(int id)
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

    public Stochast Parameter
    {
        get
        {
            if (stochast == null)
            {
                int stochastId = Interface.GetIdValue(id, "variable");
                stochast = new Stochast(stochastId);
            }

            return stochast;
        }
        set
        {
            Interface.SetIntValue(id, "variable", value.GetId());
            stochast = value;
        }
    }

    public int Index
    {
        get { return Interface.GetIntValue(id, "index"); }
        set { Interface.SetIntValue(id, "index", value); }
    }

    public string Identifier
    {
        get { return Interface.GetStringValue(id, "identifier"); }
        set { Interface.SetStringValue(id, "identifier", value); }
    }

    public double Alpha
    {
        get { return Interface.GetValue(id, "alpha"); }
        set { Interface.SetValue(id, "alpha", value); }
    }

    public double AlphaCorrelated
    {
        get { return Interface.GetValue(id, "alpha_correlated"); }
        set { Interface.SetValue(id, "alpha_correlated", value); }
    }

    public double InfluenceFactor
    {
        get { return Interface.GetValue(id, "influence_factor"); }
        set { Interface.SetValue(id, "influence_factor", value); }
    }

    public double X
    {
        get { return Interface.GetValue(id, "x"); }
        set { Interface.SetValue(id, "x", value); }
    }

    public double U
    {
        get { return Interface.GetValue(id, "u"); }
        set { Interface.SetValue(id, "u", value); }
    }
}