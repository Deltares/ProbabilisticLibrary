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
using Deltares.Probabilistic.Utils;

namespace Deltares.Probabilistic.Statistics
{
    public class ConditionalValue
    {
        private int id = 0;

        public ConditionalValue()
        {
            this.id = Interface.Create("conditional_value");
        }

        internal ConditionalValue(int id)
        {
            this.id = id;
        }

        ~ConditionalValue()
        {
            Interface.Destroy(id);
        }

        internal int GetId()
        {
            return id;
        }

        public double X
        {
            get { return Interface.GetValue(id, "x"); }
            set { Interface.SetValue(id, "x", value); }
        }

        public double Mean
        {
            get { return Interface.GetValue(id, "mean"); }
            set { Interface.SetValue(id, "mean", value); }
        }

        public double Deviation
        {
            get { return Interface.GetValue(id, "deviation"); }
            set { Interface.SetValue(id, "deviation", value); }
        }

        public double Location
        {
            get { return Interface.GetValue(id, "location"); }
            set { Interface.SetValue(id, "location", value); }
        }

        public double Scale
        {
            get { return Interface.GetValue(id, "scale"); }
            set { Interface.SetValue(id, "scale", value); }
        }

        public double Shift
        {
            get { return Interface.GetValue(id, "shift"); }
            set { Interface.SetValue(id, "shift", value); }
        }

        public double ShiftB
        {
            get { return Interface.GetValue(id, "shift_b"); }
            set { Interface.SetValue(id, "shift_b", value); }
        }

        public double Minimum
        {
            get { return Interface.GetValue(id, "minimum"); }
            set { Interface.SetValue(id, "minimum", value); }
        }

        public double Maximum
        {
            get { return Interface.GetValue(id, "maximum"); }
            set { Interface.SetValue(id, "maximum", value); }
        }

        public double Shape
        {
            get { return Interface.GetValue(id, "shape"); }
            set { Interface.SetValue(id, "shape", value); }
        }

        public double ShapeB
        {
            get { return Interface.GetValue(id, "shape_b"); }
            set { Interface.SetValue(id, "shape_b", value); }
        }

        public int Observations
        {
            get { return Interface.GetIntValue(id, "observations"); }
            set { Interface.SetIntValue(id, "observations", value); }
        }
    }
}
