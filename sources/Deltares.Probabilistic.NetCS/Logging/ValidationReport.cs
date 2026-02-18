using System;
using System.Collections.Generic;
using Deltares.Probabilistic.Utils;

namespace Deltares.Probabilistic.Logging;

public class ValidationReport : IDisposable
{
    private List<Message> messages = null;

    private int id = 0;

    public ValidationReport()
    {
        this.id = Interface.Create("validation_report");
    }

    internal ValidationReport(int id)
    {
        this.id = id;
    }

    public void Dispose()
    {
        Interface.Destroy(id);
    }

    public bool IsValid()
    {
        return Interface.GetBoolValue(id, "is_valid");
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