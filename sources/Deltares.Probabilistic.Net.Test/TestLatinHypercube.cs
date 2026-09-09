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
using Deltares.Probabilistic.Logging;
using Deltares.Probabilistic.Reliability;
using NUnit.Framework;
using NUnit.Framework.Legacy;

namespace Deltares.Probabilistic.Test
{
    [TestFixture]
    public class TestLatinHypercube
    {
        private const double margin = 0.01;

        [Test]
        public void TestLinear()
        {
            var project = ProjectBuilder.GetLinearProject();

            project.Settings.ReliabilityMethod = ReliabilityMethod.LatinHypercube;
            project.Run();

            DesignPoint designPoint = project.DesignPoint;

            ClassicAssert.AreEqual(2.652, designPoint.Beta, margin);
        }

        [Test]
        public void TestLinearProgress()
        {
            var project = ProjectBuilder.GetLinearProject();

            ProgressHolder progressHolder = new ProgressHolder();
            project.ProgressIndicator = new ProgressIndicator(progressHolder.SetProgress, progressHolder.SetDetailedProgress, progressHolder.SetTextualProgress);

            project.Settings.ReliabilityMethod = ReliabilityMethod.LatinHypercube;
            project.Run();

            ClassicAssert.AreEqual(1000, progressHolder.Invocations);
            ClassicAssert.AreEqual(1, progressHolder.Progress, margin);
            ClassicAssert.AreEqual("1000/1000, Reliability = 2.652, Convergence = 0.499", progressHolder.Text);
        }
    }
}
