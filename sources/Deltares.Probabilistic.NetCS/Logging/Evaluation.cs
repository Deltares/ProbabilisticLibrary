// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of the Probabilistic Library.
//
// The Probabilistic Library is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// All names, logos, and references to "Deltares" are registered trademarks of
// Stichting Deltares and remain full property of Stichting Deltares at all times.
// All rights reserved.
//
using Deltares.Probabilistic.Model;
using Deltares.Probabilistic.Utils;

namespace Deltares.Probabilistic.Logging
{
    public class Evaluation
    {
        private int id = 0;
        private double[] inputValues = null;
        private double[] outputValues = null;
        private object tag = null;
        private TagRepository tagRepository = null;

        public Evaluation()
        {
            this.id = Interface.Create("evaluation");
        }

        internal Evaluation(int id, TagRepository tagRepository)
        {
            this.id = id;
            this.tagRepository = tagRepository;
        }

        ~Evaluation()
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
                if (tagRepository != null)
                {
                    int tagId = Interface.GetIntValue(id, "tag");
                    tag = tagRepository.RetrieveTag(tagId);
                    tagRepository = null; // no need to keep longer
                }

                return tag;
            }
            set
            {
                tag = value;
                tagRepository = null;
            }
        }
    }
}
