using Deltares.Probabilistic.Utils;

namespace Deltares.Probabilistic.Statistics;

public class FragilityValue
{
    private int id = 0;

    public FragilityValue()
    {
        this.id = Interface.Create("fragility_value");
    }

    internal FragilityValue(int id)
    {
        this.id = id;
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

    public double DesignPoint
    {
        get { return Interface.GetValue(id, "design_point"); }
        set { Interface.SetValue(id, "design_point", value); }
    }

}