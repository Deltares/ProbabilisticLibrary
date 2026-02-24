using System.Collections.Generic;
using System.Linq;
using Deltares.Probabilistic.Statistics;
using Deltares.Probabilistic.Model;
using Deltares.Probabilistic.Utils;
using Deltares.Probabilistic.Logging;
using Deltares.Probabilistic.Reliability;

namespace Deltares.Probabilistic.Sensitivity
{
    public class SensitivityProject : ModelProject
    {
        private int id = 0;
        private SensitivitySettings settings = null;
        private SensitivityResult result = null;
        private List<SensitivityResult> results = null;

        public SensitivityProject() : base(-1)
        {
            this.id = Interface.Create("sensitivity_project");
            base.SetId(id);
        }

        internal SensitivityProject(int id) : base(id)
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
                    if (settingsId == 0)
                    {
                        settings = new SensitivitySettings();
                        Interface.SetIntValue(id, "settings", settings.GetId());
                    }
                    else
                    {
                        settings = new SensitivitySettings(settingsId);
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
            result = null;
            results = null;
            Interface.Execute(id, "run");
        }

        public SensitivityResult Result
        {
            get
            {
                if (result == null)
                {
                    int resultId = Interface.GetIdValue(id, "result");
                    result = new SensitivityResult(resultId, TagRepository, this);
                }

                return result;
            }
            set
            {
                Interface.SetIntValue(id, "result", value.GetId());
                result = value;
            }
        }

        public IList<SensitivityResult> Results
        {
            get
            {
                if (results == null)
                {
                    results = new List<SensitivityResult>();

                    int[] resultIds = Interface.GetArrayIdValue(id, "results");
                    foreach (int resultId in resultIds)
                    {
                        results.Add(new SensitivityResult(resultId, TagRepository, this));
                    }
                }

                return results;
            }
        }

        public int TotalModelRuns
        {
            get { return Interface.GetIntValue(id, "total_model_runs"); }
            set { Interface.SetIntValue(id, "total_model_runs", value); }
        }
    }
}