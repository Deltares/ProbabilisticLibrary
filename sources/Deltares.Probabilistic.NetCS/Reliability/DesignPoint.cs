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
using System.Collections.Generic;
using Deltares.Probabilistic.Logging;
using Deltares.Probabilistic.Model;
using Deltares.Probabilistic.Utils;

namespace Deltares.Probabilistic.Reliability;

public class DesignPoint : StochastPoint
{
    private readonly int id = 0;
    private List<DesignPoint> contributingDesignPoints = null;
    private List<Evaluation> realizations = null;
    private List<ReliabilityResult> reliabilityResults = null;
    private List<Message> messages = null;
    private TagRepository tagRepository = null;

    public DesignPoint() : base(-1)
    {
        id = Interface.Create("design_point");
        base.SetId(id);
        ObjectFactory.Register(this, id);
    }

    public DesignPoint(int id) : base(id)
    {
        this.id = id;
        ObjectFactory.Register(this, id);
    }

    internal void SetTagRepository(TagRepository tagRepository)
    {
        this.tagRepository = tagRepository;
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
    }

    public double ProbabilityNonFailure
    {
        get { return Interface.GetValue(id, "probability_non_failure"); }
    }

    public double ReturnPeriod
    {
        get { return Interface.GetValue(id, "return_period"); }
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
                    DesignPoint contributingDesignPoint = ObjectFactory.GetObject<DesignPoint>(designPointId);
                    contributingDesignPoint.SetTagRepository(this.tagRepository);
                    contributingDesignPoints.Add(contributingDesignPoint);
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

                int[] realizationIds = Interface.GetArrayIdValue(id, "evaluations");
                foreach (int realizationId in realizationIds)
                {
                    realizations.Add(new Evaluation(realizationId, tagRepository));
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
