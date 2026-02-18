using System;
using Deltares.Probabilistic.Utils;

namespace Deltares.Probabilistic.Statistics;

public class ContributingStochast : IDisposable
{
    private int id = 0;
    private Stochast variable = null;

    public ContributingStochast()
    {
        this.id = Interface.Create("contributing_stochast");
    }

    internal ContributingStochast(int id)
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

    public double Probability
    {
        get { return Interface.GetValue(id, "probability"); }
        set { Interface.SetValue(id, "probability", value); }
    }

    public Stochast Stochast
    {
        get
        {
            if (variable == null)
            {
                int variableId = Interface.GetIdValue(id, "variable");
                if (variableId > 0)
                {
                    variable = new Stochast(variableId);
                }
            }

            return variable;
        }
        set
        {
            if (value != null)
            {
                Interface.SetIntValue(id, "variable", value.GetId());
            }

            variable = value;
        }
    }
}