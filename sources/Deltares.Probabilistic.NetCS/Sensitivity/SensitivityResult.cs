using System;
using Deltares.Probabilistic.Logging;
using Deltares.Probabilistic.Reliability;
using System.Collections.Generic;
using Deltares.Probabilistic.Model;
using Deltares.Probabilistic.Utils;

namespace Deltares.Probabilistic.Sensitivity;

public class SensitivityResult : IDisposable
{
    private int id = 0;

    private List<SensitivityValue> values = null;
    private List<Evaluation> realizations = null;
    private List<Message> messages = null;
    private TagRepository tagRepository = null;
    private IStochastProvider stochastProvider = null;

    public SensitivityResult()
    {
        this.id = Interface.Create("sensitivity_result");
    }

    internal SensitivityResult(int id, TagRepository tagRepository, IStochastProvider stochastProvider)
    {
        this.id = id;
        this.tagRepository = tagRepository;
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

    public string Identifier
    {
        get { return Interface.GetStringValue(id, "identifier"); }
        set { Interface.SetStringValue(id, "identifier", value); }
    }

    public IList<SensitivityValue> Values
    {
        get
        {
            if (values == null)
            {
                values = new List<SensitivityValue>();

                int[] valueIds = Interface.GetArrayIdValue(id, "values");
                foreach (int valueId in valueIds)
                {
                    values.Add(new SensitivityValue(valueId, stochastProvider));
                }
            }

            return values;
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