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

namespace Deltares.Probabilistic.Sensitivity;

public class SensitivityResult
{
    private int id = 0;

    private List<SensitivityValue> values = null;
    private List<Evaluation> realizations = null;
    private List<Message> messages = null;
    private TagRepository tagRepository = null;

    public SensitivityResult()
    {
        this.id = Interface.Create("sensitivity_result");
    }

    internal SensitivityResult(int id, TagRepository tagRepository)
    {
        this.id = id;
        this.tagRepository = tagRepository;
    }

    ~SensitivityResult()
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
                    values.Add(new SensitivityValue(valueId));
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