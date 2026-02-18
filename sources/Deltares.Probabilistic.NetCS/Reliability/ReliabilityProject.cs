using System.Collections.Generic;
using System.Linq;
using Deltares.Probabilistic.Statistics;
using Deltares.Probabilistic.Model;
using Deltares.Probabilistic.Utils;

namespace Deltares.Probabilistic.Reliability
{
    public class ReliabilityProject : ModelProject
    {
        private int id = 0;
        private ReliabilitySettings reliabilitySettings = null;

        public ReliabilityProject() : base(-1)
        {
            this.id = Interface.Create("reliability_project");
            base.SetId(id);
        }

        internal ReliabilityProject(int id) : base(id)
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

        public ReliabilitySettings ReliabilitySettings
        {
            get
            {
                if (reliabilitySettings == null)
                {
                    int settingsId = Interface.GetIdValue(id, "settings");
                    reliabilitySettings = new ReliabilitySettings(settingsId);
                }
                return reliabilitySettings;
            }
            set
            {
                Interface.SetIntValue(id, "settings",  value.GetId()); 
                reliabilitySettings = value;
            }
        }
    }
}
