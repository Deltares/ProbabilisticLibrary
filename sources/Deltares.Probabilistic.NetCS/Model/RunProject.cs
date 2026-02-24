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
using System.Linq;
using Deltares.Probabilistic.Statistics;
using Deltares.Probabilistic.Model;
using Deltares.Probabilistic.Utils;
using System;
using Deltares.Probabilistic.Logging;
using Deltares.Probabilistic.Uncertainty;

namespace Deltares.Probabilistic.Reliability
{
    public class RunProject : ModelProject
    {
        private int id = 0;
        private RunSettings settings = null;
        private Evaluation realization = null;

        public RunProject() : base(-1)
        {
            this.id = Interface.Create("run_project");
            base.SetId(id);
        }

        internal RunProject(int id) : base(id)
        {
            this.id = id;
        }

        public RunSettings Settings
        {
            get
            {
                if (settings == null)
                {
                    int settingsId = Interface.GetIdValue(id, "settings");
                    if (settingsId == 0)
                    {
                        settings = new RunSettings();
                        Interface.SetIntValue(id, "settings", settings.GetId());
                    }
                    else
                    {
                        settings = new RunSettings(settingsId);
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
            realization = null;
            Interface.Execute(id, "run");
        }

        public Evaluation Realization
        {
            get
            {
                if (realization == null)
                {
                    int realizationId = Interface.GetIdValue(id, "realization");
                    realization = new Evaluation(realizationId, this.TagRepository);
                }

                return realization;
            }
            set
            {
                Interface.SetIntValue(id, "realization", value.GetId());
                realization = value;
            }
        }
    }
}