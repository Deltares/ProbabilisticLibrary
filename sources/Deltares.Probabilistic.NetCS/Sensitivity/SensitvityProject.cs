using System.Linq;
using Deltares.Probabilistic.Statistics;
using Deltares.Probabilistic.Model;
using Deltares.Probabilistic.Utils;
using Deltares.Probabilistic.Logging;

namespace Deltares.Probabilistic.Sensitivity
{
    public class SensitvityProject : ModelProject
    {
        private int id = 0;
        private SensitivitySettings settings = null;
        private Evaluation realization = null;

        public SensitvityProject() : base(-1)
        {
            this.id = Interface.Create("sensitivity_project");
            base.SetId(id);
        }

        internal SensitvityProject(int id) : base(id)
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
                    settings = new SensitivitySettings(settingsId);
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
                    realization = new Evaluation(realizationId);
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