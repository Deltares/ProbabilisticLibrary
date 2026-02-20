using System.Collections.Generic;
using System.Linq;
using Deltares.Probabilistic.Statistics;
using Deltares.Probabilistic.Model;
using Deltares.Probabilistic.Utils;
using Deltares.Probabilistic.Logging;

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
                Interface.SetIntValue(id, "settings",  value.GetId()); 
                settings = value;
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

        public int TotalModelRuns
        {
            get { return Interface.GetIntValue(id, "total_model_runs"); }
            set { Interface.SetIntValue(id, "total_model_runs", value); }
        }
    }
}
