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
using Deltares.Probabilistic.Statistics;
using Deltares.Probabilistic.Utils;
using System.Collections.Generic;
using System.Linq;

namespace Deltares.Probabilistic.Reliability;

public class ExcludingCombineProject
{
    private readonly int id = 0;
    private ExcludingCombineSettings settings = null;
    private CallBackList<DesignPoint> designPoints = null;
    private CallBackList<Scenario> scenarios = null;
    private DesignPoint designPoint = null;

    public ExcludingCombineProject()
    {
        this.id = Interface.Create("combine_project");
    }

    internal ExcludingCombineProject(int id)
    {
        this.id = id;
    }

    ~ExcludingCombineProject()
    {
        Interface.Destroy(id);
    }

    internal int GetId()
    {
        return id;
    }

    public ExcludingCombineSettings Settings
    {
        get
        {
            if (settings == null)
            {
                int settingsId = Interface.GetIdValue(id, "settings");
                if (settingsId == 0)
                {
                    settings = new ExcludingCombineSettings();
                    Interface.SetIntValue(id, "settings", settings.GetId());
                }
                else
                {
                    settings = new ExcludingCombineSettings(settingsId);
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

    public IList<DesignPoint> DesignPoints
    {
        get
        {
            if (designPoints == null)
            {
                designPoints = new CallBackList<DesignPoint>(DesignPointsChanged);

                int[] designPointIds = Interface.GetArrayIdValue(id, "design_points");
                foreach (int designPointId in designPointIds)
                {
                    designPoints.AddWithoutCallBack(ObjectFactory.GetObject<DesignPoint>(designPointId));
                }
            }

            return designPoints;
        }
    }

    private void DesignPointsChanged(ListOperationType listOperation, DesignPoint item)
    {
        Interface.SetArrayIntValue(id, "design_points", this.designPoints.Select(p => p.GetId()).ToArray());
    }

    public IList<Scenario> Scenarios
    {
        get
        {
            if (scenarios == null)
            {
                scenarios = new CallBackList<Scenario>(ScenariosChanged);

                int[] scenarioIds = Interface.GetArrayIdValue(id, "scenarios");
                foreach (int scenarioId in scenarioIds)
                {
                    scenarios.AddWithoutCallBack(ObjectFactory.GetObject<Scenario>(scenarioId));
                }
            }

            return scenarios;
        }
    }

    private void ScenariosChanged(ListOperationType listOperation, Scenario item)
    {
        Interface.SetArrayIntValue(id, "scenarios", this.scenarios.Select(p => p.GetId()).ToArray());
    }

    public void Run()
    {
        designPoint = null;
        Interface.Execute(id, "run");
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

 