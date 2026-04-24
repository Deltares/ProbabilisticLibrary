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
        public Evaluation()
        {
        }

        internal Evaluation(int id, TagRepository tagRepository)
        {
            this.Iteration = Interface.GetIntValue(id, "iteration");
            this.Beta = Interface.GetValue(id, "beta");
            this.Z = Interface.GetValue(id, "z");
            this.Quantile = Interface.GetValue(id, "quantile");
            this.Weight = Interface.GetValue(id, "weight");
            this.InputValues = Interface.GetArrayValue(id, "input_values");
            this.OutputValues = Interface.GetArrayValue(id, "output_values");
            this.Tag = tagRepository.RetrieveTag(Interface.GetIntValue(id, "tag"));

            Interface.Destroy(id);
        }

        public int Iteration { get; set; }
        public double Beta { get; set; }
        public double Z { get; set; }
        public double Quantile { get; set; }
        public double Weight { get; set; }
        public double[] InputValues { get; private set; }
        public double[] OutputValues { get; private set; }
        public object Tag { get; set; }
    }
}
