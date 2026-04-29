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
using Deltares.Probabilistic.Statistics;
using Deltares.Probabilistic.Utils;

namespace Deltares.Probabilistic.Reliability
{
    public class FragilityCurveProject
    {
        private readonly int id = 0;
        private FragilitySettings settings = null;
        private Stochast integrand = null;
        private FragilityCurve fragilityCurve = null;
        private FragilityCurve fragilityCurveNormalized = null;
        private DesignPoint designPoint = null;

        public FragilityCurveProject()
        {
            this.id = Interface.Create("fragility_curve_project");
        }

        internal FragilityCurveProject(int id)
        {
            this.id = id;
        }

        public FragilitySettings Settings
        {
            get
            {
                if (settings == null)
                {
                    int settingsId = Interface.GetIdValue(id, "settings");
                    if (settingsId == 0)
                    {
                        settings = new FragilitySettings();
                        Interface.SetIntValue(id, "settings", settings.GetId());
                    }
                    else
                    {
                        settings = new FragilitySettings(settingsId);
                    }
                }
                return settings;
            }
            set
            {
                Interface.SetIntValue(id, "settings",  value?.GetId() ?? 0); 
                settings = value;
            }
        }

        public Stochast Integrand
        {
            get
            {
                return integrand;
            }
            set
            {
                Interface.SetIntValue(id, "integrand", value?.GetId() ?? 0);
                integrand = value;
            }
        }

        public FragilityCurve FragilityCurve
        {
            get
            {
                return fragilityCurve;
            }
            set
            {
                Interface.SetIntValue(id, "fragility_curve", value?.GetId() ?? 0);
                fragilityCurve = value;
            }
        }

        public FragilityCurve FragilityCurveNormalized
        {
            get
            {
                return fragilityCurveNormalized;
            }
            set
            {
                Interface.SetIntValue(id, "fragility_curve_normalized", value?.GetId() ?? 0);
                fragilityCurveNormalized = value;
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
                }

                return designPoint;
            }
            set
            {
                Interface.SetIntValue(id, "design_point", value.GetId());
                designPoint = value;
            }
        }
    }
}
