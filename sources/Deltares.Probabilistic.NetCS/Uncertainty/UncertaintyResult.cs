using System;
using System.Collections.Generic;
using Deltares.Probabilistic.Logging;
using Deltares.Probabilistic.Model;
using Deltares.Probabilistic.Statistics;
using Deltares.Probabilistic.Utils;

namespace Deltares.Probabilistic.Uncertainty;

public class UncertaintyResult : IDisposable
{
    private int id = 0;

    private Stochast stochast = null;
    private List<Evaluation> realizations = null;
    private List<Evaluation> quantileRealizations = null;
    private List<Message> messages = null;
    private TagRepository tagRepository = null;

    public UncertaintyResult()
    {
        this.id = Interface.Create("uncertainty_result");
    }

    internal UncertaintyResult(int id, TagRepository tagRepository)
    {
        this.id = id;
        this.tagRepository = tagRepository;
    }

    public void Dispose()
    {
        Interface.Destroy(id);
    }

    internal int GetId()
    {
        return id;
    }

    public string Identifier
    {
        get { return Interface.GetStringValue(id, "identifier"); }
        set { Interface.SetStringValue(id, "identifier", value); }
    }

    public Stochast Stochast
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
                    realizations.Add(new Evaluation(realizationId, tagRepository));
                }
            }

            return realizations;
        }
    }

    public IList<Evaluation> QuantileRealizations
    {
        get
        {
            if (realizations == null)
            {
                realizations = new List<Evaluation>();

                int[] realizationIds = Interface.GetArrayIdValue(id, "quantile_realizations");
                foreach (int realizationId in realizationIds)
                {
                    realizations.Add(new Evaluation(realizationId, tagRepository));
                }
            }

            return realizations;
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