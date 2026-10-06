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

namespace Deltares.Probabilistic.Optimization
{
    public class OptimizationProject : ModelProject
    {
        private int id = 0;
        private OptimizationSettings settings = null;
        private CallBackList<ModelParameter> sensitivityParameters = null;
        private OptimizationResult result = null;

        public OptimizationProject() : base(-1)
        {
            this.id = Interface.Create("optimization_project");
            base.SetId(id);
        }

        internal OptimizationProject(int id) : base(id)
        {
            this.id = id;
        }

        public OptimizationSettings Settings
        {
            get
            {
                if (settings == null)
                {
                    int settingsId = Interface.GetIdValue(id, "settings");
                    if (settingsId == 0)
                    {
                        settings = new OptimizationSettings();
                        Interface.SetIntValue(id, "settings", settings.GetId());
                    }
                    else
                    {
                        settings = new OptimizationSettings(settingsId);
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
            Interface.Execute(id, "run");
        }

        public void Stop()
        {
            Interface.Execute(id, "stop");
        }

        public OptimizationResult Result
        {
            get
            {
                if (result == null)
                {
                    int resultId = Interface.GetIdValue(id, "result");
                    result = new OptimizationResult(resultId, TagRepository);
                }

                return result;
            }
            set
            {
                Interface.SetIntValue(id, "result", value.GetId());
                result = value;
            }
        }
    }
}