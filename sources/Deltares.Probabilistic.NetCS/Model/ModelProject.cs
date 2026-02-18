using System;
using System.Collections.Generic;
using System.Linq;
using Deltares.Probabilistic.Statistics;
using Deltares.Probabilistic.Utils;

namespace Deltares.Probabilistic.Model
{
    public delegate void ZSampleDelegate(ModelSample sample);

    public class ModelProject : IDisposable
    {
        private int id = 0;

        private CallBackList<Stochast> stochasts = null;
        private ZSampleDelegate zFunction = null;

        private bool synchronizing = false;


        public ModelProject()
        {
            this.id = Interface.Create("model_project");
        }

        internal ModelProject(int id)
        {
            if (id > 0)
            {
                this.id = id;
            }
        }

        public void Dispose()
        {
            Interface.Destroy(id);
        }

        internal int GetId()
        {
            return id;
        }

        internal void SetId(int id)
        {
            this.id = id;
        }

        public ZSampleDelegate ZFunction
        {
            get { return zFunction; }
            set { zFunction = value; }
        }

        public IList<Stochast> Stochasts
        {
            get
            {
                if (stochasts == null)
                {
                    synchronizing = true;
                    stochasts = new CallBackList<Stochast>(StochastValuesChanged);

                    int[] stochastIds = Interface.GetArrayIdValue(id, "stochasts");
                    foreach (int stochastId in stochastIds)
                    {
                        stochasts.Add(new Stochast(stochastId));
                    }

                    synchronizing = true;
                }

                return stochasts;
            }
        }

        private void StochastValuesChanged(ListOperationType listOperation, Stochast item)
        {
            if (!synchronizing)
            {
                Interface.SetArrayIntValue(id, "stochasts", this.stochasts.Select(p => p.GetId()).ToArray());
            }
        }
    }
}
