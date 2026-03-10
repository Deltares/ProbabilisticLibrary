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
using System.Linq;
using Deltares.Probabilistic.Model;
using Deltares.Probabilistic.Utils;
using Deltares.Probabilistic.Statistics;

namespace Deltares.Probabilistic.Uncertainty
{
    public class UncertaintyProject : ModelProject
    {
        private int id = 0;
        private UncertaintySettings settings = null;
        private UncertaintyResult result = null;
        private List<UncertaintyResult> results = null;
        private CorrelationMatrix outputCorrelationMatrix = null;
        private Stochast stochast = null;
        private List<Stochast> stochasts = null;

        public UncertaintyProject() : base(-1)
        {
            this.id = Interface.Create("uncertainty_project");
            base.SetId(id);
        }

        internal UncertaintyProject(int id) : base(id)
        {
            this.id = id;
        }

        public UncertaintySettings Settings
        {
            get
            {
                if (settings == null)
                {
                    int settingsId = Interface.GetIdValue(id, "settings");
                    if (settingsId == 0)
                    {
                        settings = new UncertaintySettings();
                        Interface.SetIntValue(id, "settings", settings.GetId());
                    }
                    else
                    {
                        settings = new UncertaintySettings(settingsId);
                    }
                }
                return settings;
            }
            set
            {
                Interface.SetIntValue(id, "settings", value.GetId());
                settings = value;
            }
        }

        public void Run()
        {
            result = null;
            results = null;
            stochast = null;
            stochasts = null;
            outputCorrelationMatrix = null;
            Interface.Execute(id, "run");
        }

        public void Stop()
        {
            Interface.Execute(id, "stop");
        }

        public UncertaintyResult Result
        {
            get
            {
                if (result == null)
                {
                    int resultId = Interface.GetIdValue(id, "uncertainty_result");
                    result = new UncertaintyResult(resultId, TagRepository);
                }

                return result;
            }
            set
            {
                Interface.SetIntValue(id, "uncertainty_result", value.GetId());
                result = value;
            }
        }

        public IList<UncertaintyResult> Results
        {
            get
            {
                if (results == null)
                {
                    results = new List<UncertaintyResult>();

                    int[] resultIds = Interface.GetArrayIdValue(id, "uncertainty_results");
                    foreach (int resultId in resultIds)
                    {
                        results.Add(new UncertaintyResult(resultId, TagRepository));
                    }
                }

                return results;
            }
        }

        public Stochast Stochast
        {
            get
            {
                if (stochast == null)
                {
                    stochast = Result.Stochast;
                }

                return stochast;
            }
        }

        public IList<Stochast> StochastResults
        {
            get
            {
                if (stochasts == null)
                {
                    stochasts = new List<Stochast>();
                    stochasts.AddRange(Results.Select(p => p.Stochast));
                }

                return stochasts;
            }
        }

        public CorrelationMatrix OutputCorrelationMatrix
        {
            get
            {
                if (outputCorrelationMatrix == null)
                {
                    int matrixId = Interface.GetIdValue(id, "output_correlation_matrix");
                    outputCorrelationMatrix = new CorrelationMatrix(matrixId);
                }

                return outputCorrelationMatrix;
            }
            set
            {
                Interface.SetIntValue(id, "output_correlation_matrix", value.GetId());
                outputCorrelationMatrix = value;
            }
        }

        public int TotalModelRuns
        {
            get { return Interface.GetIntValue(id, "total_model_runs"); }
            set { Interface.SetIntValue(id, "total_model_runs", value); }
        }
    }
}