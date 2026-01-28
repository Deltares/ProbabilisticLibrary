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
    public class TestSingleVariationSensitivity
    {
        private const double margin = 0.02;

        [Test]
        public void TestLinear()
        {
            var project = ProjectBuilder.GetSensitivityProject(ProjectBuilder.GetLinearProject());

            project.SensitivityMethod = new SingleVariation();

            SensitivityResult result = project.GetResult();

            ClassicAssert.AreEqual(2, result.Values.Count);

            ClassicAssert.AreEqual(2.7, result.Values[0].Low, margin);
            ClassicAssert.AreEqual(1.8, result.Values[0].Medium, margin);
            ClassicAssert.AreEqual(0.9, result.Values[0].High, margin);
            ClassicAssert.AreEqual(project.Stochasts[0], result.Values[0].Parameter);
        }

        [Test]
        public void TestUnbalanced()
        {
            var project = ProjectBuilder.GetSensitivityProject(ProjectBuilder.GetUnbalancedLinearProject());

            project.SensitivityMethod = new SingleVariation();

            SensitivityResult result = project.GetResult();

            ClassicAssert.AreEqual(2, result.Values.Count);

            ClassicAssert.AreEqual(2.07, result.Values[0].Low, margin);
            ClassicAssert.AreEqual(1.8, result.Values[0].Medium, margin);
            ClassicAssert.AreEqual(1.53, result.Values[0].High, margin);
            ClassicAssert.AreEqual(project.Stochasts[0], result.Values[0].Parameter);

            ClassicAssert.AreEqual(3.42, result.Values[1].Low, margin);
            ClassicAssert.AreEqual(1.8, result.Values[1].Medium, margin);
            ClassicAssert.AreEqual(0.18, result.Values[1].High, margin);
            ClassicAssert.AreEqual(project.Stochasts[1], result.Values[1].Parameter);
        }
    }
}
