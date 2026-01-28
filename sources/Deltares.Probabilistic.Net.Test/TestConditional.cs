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
using Deltares.Models.Wrappers;
using Deltares.Reliability.Wrappers;
using Deltares.Statistics.Wrappers;
using NUnit.Framework;
using NUnit.Framework.Legacy;

namespace Deltares.Probabilistic.Wrapper.Test
{
    [TestFixture]
    public class TestConditional
    {
        private const double margin = 0.01;

        [Test]
        public void TestConditionalSimple()
        {
            Stochast stochast = new Stochast();
            stochast.DistributionType = DistributionType.Normal;
            stochast.Mean = 0;
            stochast.Deviation = 1;

            ClassicAssert.AreEqual(1.0, stochast.GetXFromU(1.0), margin);

            stochast.IsVariableStochast = true;

            VariableStochastValue conditional1 = new VariableStochastValue();
            conditional1.X = 0;
            conditional1.Location = 0;
            conditional1.Scale = 1;
            stochast.ValueSet.StochastValues.Add(conditional1);

            VariableStochastValue conditional2 = new VariableStochastValue();
            conditional2.X = 1;
            conditional2.Location = 1;
            conditional2.Scale = 2;
            stochast.ValueSet.StochastValues.Add(conditional2);

            ClassicAssert.AreEqual(2.0, stochast.GetXFromUAndSource(1.0, 0.5), margin);
        }
    }
}
