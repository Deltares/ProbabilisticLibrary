using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Deltares.Probabilistic.NetCS.Utils;

namespace Deltares.Probabilistic.NetCS.Statistics
{
    public class Stochast
    {
        private int id = 0;

        public Stochast(int id = 0)
        {
            if (id == 0)
            {
                this.id = Interface.Create("stochast");
            }
            else
            {
                this.id = id;
            }
        }

        public bool Inverted
        {
            get { return Interface.GetBoolValue(id, "inverted"); }
            set { Interface.SetBoolValue(id, "inverted", value); }
        }

        public bool Truncated
        {
            get { return Interface.GetBoolValue(id, "truncated"); }
            set { Interface.SetBoolValue(id, "truncated", value); }
        }

        public double Mean
        {
            get { return Interface.GetValue(id, "mean"); }
            set { Interface.SetValue(id, "mean", value); }
        }

        public double Deviation
        {
            get { return Interface.GetValue(id, "Deviation"); }
            set { Interface.SetValue(id, "Deviation", value); }
        }

        public double Variation
        {
            get { return Interface.GetValue(id, "variation"); }
            set { Interface.SetValue(id, "variation", value); }
        }


    }
}
