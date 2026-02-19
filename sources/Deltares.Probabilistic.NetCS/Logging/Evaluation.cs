using System;
using Deltares.Probabilistic.Utils;

namespace Deltares.Probabilistic.Logging
{
    public class Evaluation : IDisposable
    {
        private int id = 0;
        private double[] inputValues = null;
        private double[] outputValues = null;
        private object tag = null;

        public Evaluation()
        {
            this.id = Interface.Create("evaluation");
        }

        internal Evaluation(int id)
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

        public int Iteration
        {
            get { return Interface.GetIntValue(id, "iteration"); }
            set { Interface.SetIntValue(id, "iteration", value); }
        }

        public double Beta
        {
            get { return Interface.GetValue(id, "beta"); }
            set { Interface.SetValue(id, "beta", value); }
        }

        public double Z
        {
            get { return Interface.GetValue(id, "z"); }
            set { Interface.SetValue(id, "z", value); }
        }

        public double Quantile
        {
            get { return Interface.GetValue(id, "quantile"); }
            set { Interface.SetValue(id, "quantile", value); }
        }

        public double Weight
        {
            get { return Interface.GetValue(id, "weight"); }
            set { Interface.SetValue(id, "weight", value); }
        }

        public double[] InputValues
        {
            get
            {
                if (inputValues == null)
                {
                    inputValues = Interface.GetArrayValue(id, "input_values");
                }

                return inputValues;
            }
            set
            {
                inputValues = value;
            }
        }

        public double[] OutputValues
        {
            get
            {
                if (outputValues == null)
                {
                    outputValues = Interface.GetArrayValue(id, "output_values");
                }

                return outputValues;
            }
            set
            {
                outputValues = value;
            }
        }

        public object Tag
        {
            get
            {
                if (tag == null)
                {
                    int tagId = Interface.GetIntValue(id, "tag");

                }

                return tag;
            }
        }
    }
}
