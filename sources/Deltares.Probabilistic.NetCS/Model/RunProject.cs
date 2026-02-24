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