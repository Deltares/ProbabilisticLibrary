// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of the Probabilistic Library.
//
// The Probabilistic Library is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// All names, logos, and references to "Deltares" are registered trademarks of
// Stichting Deltares and remain full property of Stichting Deltares at all times.
// All rights reserved.
//
using Deltares.Probabilistic.Logging;
using System.Collections.Generic;
using Deltares.Probabilistic.Model;
using Deltares.Probabilistic.Utils;

namespace Deltares.Probabilistic.Optimization;

public class OptimizationResult
{
    private int id = 0;

    private double[] values = null;
    private List<Evaluation> realizations = null;
    private List<Message> messages = null;
    private TagRepository tagRepository = null;

    public OptimizationResult()
    {
        this.id = Interface.Create("optimization_result");
    }

    internal OptimizationResult(int id, TagRepository tagRepository)
    {
        this.id = id;
        this.tagRepository = tagRepository;
    }

    ~OptimizationResult()
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

    public double MinimumValue
    {
        get { return Interface.GetValue(id, "minimum_value"); }
        set { Interface.SetValue(id, "minimum_value", value); }
    }

    public bool Succeeded
    {
        get { return Interface.GetBoolValue(id, "succeeded"); }
        set { Interface.SetBoolValue(id, "succeeded", value); }
    }

    public int ModelRuns
    {
        get { return Interface.GetIntValue(id, "model_runs"); }
        set { Interface.SetIntValue(id, "model_runs", value); }
    }

    public double[] Values
    {
        get
        {
            if (values == null)
            {
                values = Interface.GetArrayValue(id, "values");
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

                int[] realizationIds = Interface.GetArrayIdValue(id, "evaluations");
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