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

using System;
using System.Collections.Generic;
using System.Linq;
using Deltares.Probabilistic.Model;
using Deltares.Probabilistic.Utils;

namespace Deltares.Probabilistic.Sensitivity
{
    public class SensitivityProject : ModelProject
    {
        private int id = 0;
        private SensitivitySettings settings = null;
        private CallBackList<ModelParameter> sensitivityParameters = null;
        private SensitivityResult result = null;
        private List<SensitivityResult> results = null;

        public SensitivityProject() : base(-1)
        {
            this.id = Interface.Create("sensitivity_project");
            base.SetId(id);
        }

        internal SensitivityProject(int id) : base(id)
        {
            this.id = id;
        }

        public SensitivitySettings Settings
        {
            get
            {
                if (settings == null)
                {
                    int settingsId = Interface.GetIdValue(id, "settings");
                    if (settingsId == 0)
                    {
                        settings = new SensitivitySettings();
                        Interface.SetIntValue(id, "settings", settings.GetId());
                    }
                    else
                    {
                        settings = new SensitivitySettings(settingsId);
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

        public IList<ModelParameter> SensitivityParameters
        {
            get
            {
                if (sensitivityParameters == null)
                {
                    sensitivityParameters = new CallBackList<ModelParameter>(SensitivityParametersChanged);

                    int[] parameterIds = Interface.GetArrayIdValue(id, "sensitivity_parameters");
                    foreach (int parameterId in parameterIds)
                    {
                        sensitivityParameters.AddWithoutCallBack(new ModelParameter(parameterId));
                    }
                }

                return sensitivityParameters;
            }
        }

        private void SensitivityParametersChanged(ListOperationType listOperation, ModelParameter item)
        {
            Interface.SetArrayIntValue(id, "sensitivity_parameters", this.sensitivityParameters.Select(p => p.GetId()).ToArray());
        }

        public string Parameter
        {
            get { return Interface.GetStringValue(id, "parameter"); }
            set { Interface.SetStringValue(id, "parameter", value); }
        }

        public void Run()
        {
            result = null;
            results = null;
            Interface.Execute(id, "run");
        }

        public void Stop()
        {
            Interface.Execute(id, "stop");
        }

        public SensitivityResult Result
        {
            get
            {
                if (result == null)
                {
                    int resultId = Interface.GetIdValue(id, "result");
                    result = new SensitivityResult(resultId, TagRepository);
                }

                return result;
            }
            set
            {
                Interface.SetIntValue(id, "result", value.GetId());
                result = value;
            }
        }

        public IList<SensitivityResult> Results
        {
            get
            {
                if (results == null)
                {
                    results = new List<SensitivityResult>();

                    int[] resultIds = Interface.GetArrayIdValue(id, "results");
                    foreach (int resultId in resultIds)
                    {
                        results.Add(new SensitivityResult(resultId, TagRepository));
                    }
                }

                return results;
            }
        }

        public int TotalModelRuns
        {
            get { return Interface.GetIntValue(id, "total_model_runs"); }
            set { Interface.SetIntValue(id, "total_model_runs", value); }
        }
    }
}