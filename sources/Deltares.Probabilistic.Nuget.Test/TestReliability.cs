// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of Streams.
//
// Streams is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// All names, logos, and references to "Deltares" are registered trademarks of
// Stichting Deltares and remain full property of Stichting Deltares at all times.
// All rights reserved.
//

using System.Linq;
using Deltares.Models.Wrappers;
using Deltares.Reliability.Wrappers;
using Deltares.Statistics.Wrappers;
using NUnit.Framework;

namespace Deltares.Probabilistic.Nuget.Test
{
    [TestFixture]
    public class TestReliability
    {
        private const double margin = 0.01;

        [Test]
        public void TestLinear()
        {
            Project project = GetLinearProject();

            project.ReliabilityMethod = new FORM();
            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(2.33, designPoint.Beta, margin);
            Assert.IsTrue(designPoint.ConvergenceReport.IsConverged);

            Assert.AreEqual(0.01, designPoint.ProbabilityFailure, margin / 10);
            Assert.AreEqual(0.99, designPoint.ProbabilityNonFailure, margin / 10);
            Assert.AreEqual(99.7, designPoint.ReturnPeriod, margin * 10);

            Assert.AreEqual(-0.7, designPoint.Alphas[0].Alpha, margin);
            Assert.AreEqual(-0.7, designPoint.Alphas[1].Alpha, margin);

            Assert.AreEqual(0.9, designPoint.Alphas[0].X, margin);
            Assert.AreEqual(0.9, designPoint.Alphas[1].X, margin);
        }

        private static Project GetLinearProject()
        {
            var project = new Project();

            project.Stochasts.Add(new Stochast { DistributionType = DistributionType.Uniform, Minimum = -1, Maximum = 1 });
            project.Stochasts.Add(new Stochast { DistributionType = DistributionType.Uniform, Minimum = -1, Maximum = 1 });

            project.CorrelationMatrix.Initialize(project.Stochasts);

            project.ZFunction = LinearZFunction;

            return project;
        }

        private static void LinearZFunction(ModelSample sample)
        {
            sample.Z = 1.8 - sample.Values.Sum();
        }
    }
}
