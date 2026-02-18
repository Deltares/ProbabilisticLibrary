using System.Collections.Generic;
using System.Linq;
using Deltares.Probabilistic.Model;
using Deltares.Probabilistic.Utils;
using Deltares.Probabilistic.Logging;
using Deltares.Probabilistic.Sensitivity;
using Deltares.Probabilistic.Statistics;

namespace Deltares.Probabilistic.Uncertainty
{
    public class UncertaintyProject : ModelProject
    {
        private int id = 0;
        private UncertaintySettings settings = null;
        private UncertaintyResult result = null;
        private List<UncertaintyResult> results = null;
        private Stochast stochast = null;
        private List<Stochast> stochasts = null;

        public UncertaintyProject() : base(-1)
        {
            this.id = Interface.Create("uncertainty_project");
            base.SetId(id);
        }

        internal UncertaintyProject(int id) : base(id)
        {
            this.id = id;
        }

        public UncertaintySettings Settings
        {
            get
            {
                if (settings == null)
                {
                    int settingsId = Interface.GetIdValue(id, "settings");
                    settings = new UncertaintySettings(settingsId);
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
            stochast = null;
            stochasts = null;
            Interface.Execute(id, "run");
        }

        public UncertaintyResult Result
        {
            get
            {
                if (result == null)
                {
                    int resultId = Interface.GetIdValue(id, "result");
                    result = new UncertaintyResult(resultId);
                }

                return result;
            }
            set
            {
                Interface.SetIntValue(id, "result", value.GetId());
                result = value;
            }
        }

        public IList<UncertaintyResult> Results
        {
            get
            {
                if (results == null)
                {
                    results = new List<UncertaintyResult>();

                    int[] resultIds = Interface.GetArrayIdValue(id, "results");
                    foreach (int resultId in resultIds)
                    {
                        results.Add(new UncertaintyResult(resultId));
                    }
                }

                return results;
            }
        }

        public Stochast Stochast
        {
            get
            {
                if (stochast == null)
                {
                    stochast = Result.Stochast;
                }

                return stochast;
            }
        }

        public IList<Stochast> Stochasts
        {
            get
            {
                if (stochasts == null)
                {
                    stochasts = new List<Stochast>(this.Results.Select(p => p.Stochast));
                }

                return stochasts;
            }
        }

        public int TotalModelRuns
        {
            get { return Interface.GetIntValue(id, "total_model_runs"); }
            set { Interface.SetIntValue(id, "total_model_runs", value); }
        }
    }
}