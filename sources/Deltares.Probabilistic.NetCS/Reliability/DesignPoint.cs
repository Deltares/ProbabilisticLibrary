using System.Collections.Generic;
using Deltares.Probabilistic.Logging;
using Deltares.Probabilistic.Model;
using Deltares.Probabilistic.Utils;

namespace Deltares.Probabilistic.Reliability;

public class DesignPoint : StochastPoint
{
    private int id = 0;
    private List<DesignPoint> contributingDesignPoints = null;
    private List<Evaluation> realizations = null;
    private List<ReliabilityResult> reliabilityResults = null;
    private List<Message> messages = null;

    public DesignPoint() : base(-1)
    {
        id = Interface.Create("design_point");
        base.SetId(id);
    }

    internal DesignPoint(int id) : base(id)
    {
        this.id = id;
    }

    public string Identifier
    {
        get { return Interface.GetStringValue(id, "identifier"); }
        set { Interface.SetStringValue(id, "identifier", value); }
    }

    public double ReliabilityIndex
    {
        get { return Interface.GetValue(id, "reliability_index"); }
        set { Interface.SetValue(id, "reliability_index", value); }
    }

    public double ProbabilityFailure
    {
        get { return Interface.GetValue(id, "probability_failure"); }
        set { Interface.SetValue(id, "probability_failure", value); }
    }

    public double Convergence
    {
        get { return Interface.GetValue(id, "convergence"); }
        set { Interface.SetValue(id, "convergence", value); }
    }

    public bool IsConverged
    {
        get { return Interface.GetBoolValue(id, "is_converged"); }
        set { Interface.SetBoolValue(id, "is_converged", value); }
    }

    public int TotalDirections
    {
        get { return Interface.GetIntValue(id, "total_directions"); }
        set { Interface.SetIntValue(id, "total_directions", value); }
    }

    public int TotalIterations
    {
        get { return Interface.GetIntValue(id, "total_iterations"); }
        set { Interface.SetIntValue(id, "total_iterations", value); }
    }

    public int TotalModelRuns
    {
        get { return Interface.GetIntValue(id, "total_model_runs"); }
        set { Interface.SetIntValue(id, "total_model_runs", value); }
    }

    public IList<DesignPoint> ContributingDesignPoints
    {
        get
        {
            if (contributingDesignPoints == null)
            {
                contributingDesignPoints = new List<DesignPoint>();

                int[] designPointIds = Interface.GetArrayIdValue(id, "contributing_design_points");
                foreach (int designPointId in designPointIds)
                {
                    contributingDesignPoints.Add(new DesignPoint(designPointId));
                }
            }

            return contributingDesignPoints;
        }
    }

    public IList<Evaluation> Realizations
    {
        get
        {
            if (realizations == null)
            {
                realizations = new List<Evaluation>();

                int[] realizationIds = Interface.GetArrayIdValue(id, "realizations");
                foreach (int realizationId in realizationIds)
                {
                    realizations.Add(new Evaluation(realizationId));
                }
            }

            return realizations;
        }
    }

    public IList<ReliabilityResult> ReliabilityResults
    {
        get
        {
            if (reliabilityResults == null)
            {
                reliabilityResults = new List<ReliabilityResult>();

                int[] resultIds = Interface.GetArrayIdValue(id, "reliability_results");
                foreach (int resultId in resultIds)
                {
                    reliabilityResults.Add(new ReliabilityResult(resultId));
                }
            }

            return reliabilityResults;
        }
    }

    public IList<Message> Messages
    {
        get
        {
            if (messages == null)
            {
                messages = new List<Message>();

                int[] messageIds = Interface.GetArrayIdValue(id, "messages");
                foreach (int messageId in messageIds)
                {
                    messages.Add(new Message(messageId));
                }
            }

            return messages;
        }
    }
}