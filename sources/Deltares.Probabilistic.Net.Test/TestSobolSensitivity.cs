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
using NUnit.Framework;
using NUnit.Framework.Legacy;
using Deltares.Sensitivity.Wrappers;

namespace Deltares.Probabilistic.Wrapper.Test
{
    [TestFixture]
    public class TestSobolSensitivity
    {
        private const double margin = 0.001;

        [Test]
        public void TestLinear()
        {
            var project = ProjectBuilder.GetSensitivityProject(ProjectBuilder.GetLinearProject());

            project.SensitivityMethod = new Sobol();

            SensitivityResult result = project.GetResult();

            ClassicAssert.AreEqual(2, result.Values.Count);

            ClassicAssert.AreEqual(0.501, result.Values[0].FirstOrderIndex, margin);
            ClassicAssert.AreEqual(0.504, result.Values[0].TotalIndex, margin);
        }

        [Test]
        public void TestUnbalanced()
        {
            var project = ProjectBuilder.GetSensitivityProject(ProjectBuilder.GetUnbalancedLinearProject());

            project.SensitivityMethod = new Sobol();

            SensitivityResult result = project.GetResult();

            ClassicAssert.AreEqual(2, result.Values.Count);

            ClassicAssert.AreEqual(0.027, result.Values[0].FirstOrderIndex, margin);
            ClassicAssert.AreEqual(0.027, result.Values[0].TotalIndex, margin);
            ClassicAssert.AreEqual(project.Stochasts[0], result.Values[0].Parameter);

            ClassicAssert.AreEqual(0.979, result.Values[1].FirstOrderIndex, margin);
            ClassicAssert.AreEqual(0.976, result.Values[1].TotalIndex, margin);
            ClassicAssert.AreEqual(project.Stochasts[1], result.Values[1].Parameter);
        }
    }
}
