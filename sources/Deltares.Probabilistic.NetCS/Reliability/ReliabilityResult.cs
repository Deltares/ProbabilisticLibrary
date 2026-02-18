using Deltares.Probabilistic.Utils;

namespace Deltares.Probabilistic.Reliability;

public class ReliabilityResult
{
    private int id = 0;

    public ReliabilityResult()
    {
        id = Interface.Create("reliability_result");
    }

    internal ReliabilityResult(int id)
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

    public int Index
    {
        get { return Interface.GetIntValue(id, "index"); }
        set { Interface.SetIntValue(id, "index", value); }
    }

    public double ReliabilityIndex
    {
        get { return Interface.GetValue(id, "reliability_index"); }
        set { Interface.SetValue(id, "reliability_index", value); }
    }

    public double Convergence
    {
        get { return Interface.GetValue(id, "convergence"); }
        set { Interface.SetValue(id, "convergence", value); }
    }
}