using System;
using System.Collections.Generic;
using System.Linq;
using Deltares.Probabilistic.Statistics;
using Deltares.Probabilistic.Utils;

namespace Deltares.Probabilistic.Reliability;

public class CombineProject : IDisposable
{
    private readonly int id = 0;
    private CombineSettings settings = null;
    private CallBackList<DesignPoint> designPoints = null;
    private DesignPoint designPoint = null;
    private bool synchronizing = false;

    public CombineProject()
    {
        this.id = Interface.Create("combine_project");
    }

    internal CombineProject(int id)
    {
        this.id = id;
    }

    public void Dispose()
    {
        Interface.Destroy(id);
    }

    internal int GetId()
    {
        return id;
    }

    public CombineSettings Settings
    {
        get
        {
            if (settings == null)
            {
                int settingsId = Interface.GetIdValue(id, "settings");
                settings = new CombineSettings(settingsId);
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
                synchronizing = true;
                designPoints = new CallBackList<DesignPoint>(DesignPointsChanged);

                int[] designPointIds = Interface.GetArrayIdValue(id, "design_points");
                foreach (int designPointId in designPointIds)
                {
                    designPoints.Add(new DesignPoint(designPointId));
                }

                synchronizing = true;
            }

            return designPoints;
        }
    }

    private void DesignPointsChanged(ListOperationType listOperation, DesignPoint item)
    {
        if (!synchronizing)
        {
            Interface.SetArrayIntValue(id, "design_points", this.designPoints.Select(p => p.GetId()).ToArray());
        }
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
                designPoint = new DesignPoint(designPointId);
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