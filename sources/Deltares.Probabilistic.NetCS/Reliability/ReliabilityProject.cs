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
using Deltares.Probabilistic.Model;
using Deltares.Probabilistic.Utils;

namespace Deltares.Probabilistic.Reliability
{
    public class ReliabilityProject : ModelProject
    {
        private readonly int id = 0;
        private ReliabilitySettings settings = null;
        private DesignPoint designPoint = null;

        public ReliabilityProject() : base(-1)
        {
            this.id = Interface.Create("project");
            base.SetId(id);
        }

        internal ReliabilityProject(int id) : base(id)
        {
            this.id = id;
        }

        public ReliabilitySettings Settings
        {
            get
            {
                if (settings == null)
                {
                    int settingsId = Interface.GetIdValue(id, "settings");
                    if (settingsId == 0)
                    {
                        settings = new ReliabilitySettings();
                        Interface.SetIntValue(id, "settings", settings.GetId());
                    }
                    else
                    {
                        settings = new ReliabilitySettings(settingsId);
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
            designPoint = null;
            Interface.Execute(id, "run");
        }

        public void Stop()
        {
            Interface.Execute(id, "stop");
        }

        public DesignPoint DesignPoint
        {
            get
            {
                if (designPoint == null)
                {
                    int designPointId = Interface.GetIdValue(id, "design_point");
                    designPoint = ObjectFactory.GetObject<DesignPoint>(designPointId);
                    designPoint.SetTagRepository(this.TagRepository);
                }

                return designPoint;
            }
            set
            {
                Interface.SetIntValue(id, "design_point", value.GetId());
                designPoint = value;
            }
        }

        public int TotalModelRuns
        {
            get { return Interface.GetIntValue(id, "total_model_runs"); }
            set { Interface.SetIntValue(id, "total_model_runs", value); }
        }
    }
}
