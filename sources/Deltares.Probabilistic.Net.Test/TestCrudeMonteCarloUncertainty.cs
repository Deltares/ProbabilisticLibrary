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
using Deltares.Probabilistic.Model;
using Deltares.Probabilistic.Statistics;
using Deltares.Probabilistic.Uncertainty;

namespace Deltares.Probabilistic.Test
{
    [TestFixture]
    public class TestCrudeMonteCarloUncertainty
    {
        private const double margin = 0.02;

        [Test]
        public void TestAddOne()
        {
            var project = ProjectBuilder.GetUncertaintyProject(ProjectBuilder.GetAddOneProject());

            project.Settings.UncertaintyMethod = UncertaintyMethod.CrudeMonteCarlo;
            project.Run();

            Stochast stochast = project.Stochast;

            ClassicAssert.AreEqual(1, stochast.Mean, margin);

            stochast.DistributionType = DistributionType.Uniform;

            ClassicAssert.AreEqual(0, stochast.Minimum, margin);
            ClassicAssert.AreEqual(2, stochast.Maximum, margin);
        }

        [Test]
        public void TestLinear()
        {
            var project = ProjectBuilder.GetUncertaintyProject(ProjectBuilder.GetLinearProject());

            project.Settings.UncertaintyMethod = UncertaintyMethod.CrudeMonteCarlo;
            project.Run();

            Stochast stochast = project.Stochast;

            ClassicAssert.AreEqual(DistributionType.Histogram, stochast.DistributionType);
            ClassicAssert.AreEqual(84, stochast.HistogramValues.Count);

            ClassicAssert.AreEqual(1.8, stochast.Mean, margin);
            ClassicAssert.AreEqual(0.82, stochast.Deviation, margin);
        }

        [Test]
        public void TestLinearDoubleExecuted()
        {
            var project = ProjectBuilder.GetUncertaintyProject(ProjectBuilder.GetLinearProject());

            project.Settings.UncertaintyMethod = UncertaintyMethod.CrudeMonteCarlo;
            project.Run();

            Stochast stochast = project.Stochast;

            ClassicAssert.AreEqual(1.8, stochast.Mean, margin);
            ClassicAssert.AreEqual(0.82, stochast.Deviation, margin);

            project.Run();
            Stochast stochast2 = project.Stochast;

            ClassicAssert.AreNotSame(stochast, stochast2);
            ClassicAssert.AreEqual(stochast.Mean, stochast2.Mean);
        }

        [Test]
        public void TestLinearParallel()
        {
            var project = ProjectBuilder.GetUncertaintyProject(ProjectBuilder.GetLinearProject());

            project.Settings.UncertaintyMethod = UncertaintyMethod.CrudeMonteCarlo;
            project.Settings.MaxParallelProcesses = 4;
            project.Run();

            Stochast stochast = project.Stochast;

            ClassicAssert.AreEqual(1.8, stochast.Mean, margin);
            ClassicAssert.AreEqual(0.82, stochast.Deviation, margin);
        }

        [Test]
        public void TestLinearManySamples()
        {
            var project = ProjectBuilder.GetUncertaintyProject(ProjectBuilder.GetLinearProject());

            project.Settings.UncertaintyMethod = UncertaintyMethod.CrudeMonteCarlo;
            project.Settings.SaveConvergence = false;
            project.Settings.SaveRealizations = false;
            project.Settings.SaveMessages = false;
            project.Settings.MaximumSamples = 100000;
            project.Run();

            Stochast stochast = project.Stochast;

            ClassicAssert.AreEqual(1.8, stochast.Mean, margin);
            ClassicAssert.AreEqual(0.82, stochast.Deviation, margin);

            stochast.DistributionType = DistributionType.Uniform;

            ClassicAssert.AreEqual(-0.2, stochast.Minimum, 10 * margin);
            ClassicAssert.AreEqual(3.8, stochast.Maximum, 10 * margin);
        }

        [Test]
        public void TestLinearAutoSamples()
        {
            var project = ProjectBuilder.GetUncertaintyProject(ProjectBuilder.GetLinearProject());

            project.Settings.UncertaintyMethod = UncertaintyMethod.CrudeMonteCarlo;
            project.Settings.SaveConvergence = false;
            project.Settings.SaveRealizations = false;
            project.Settings.SaveMessages = false;
            project.Settings.DeriveSamplesFromVariationCoefficient = true;

            ClassicAssert.AreEqual(7600, project.Settings.GetRequiredSamples());

            project.Run();

            Stochast stochast = project.Stochast;

            ClassicAssert.AreEqual(1.8, stochast.Mean, margin);
            ClassicAssert.AreEqual(0.82, stochast.Deviation, margin);
        }

        [Test]
        public void TestLinearFullyCorrelated()
        {
            var project = ProjectBuilder.GetUncertaintyProject(ProjectBuilder.GetLinearFullyCorrelatedProject());
            project.Settings.UncertaintyMethod = UncertaintyMethod.CrudeMonteCarlo;
            project.Run();

            Stochast stochast = project.Stochast;

            ClassicAssert.AreEqual(1.79, stochast.Mean, margin);
            ClassicAssert.AreEqual(0.82, stochast.Deviation, margin);

            stochast.DistributionType = DistributionType.Uniform;

            ClassicAssert.AreEqual(-0.2, stochast.Minimum, 10 * margin);
            ClassicAssert.AreEqual(3.8, stochast.Maximum, 10 * margin);
        }

        [Test]
        public void TestLinearNegativeFullyCorrelated()
        {
            var project = ProjectBuilder.GetUncertaintyProject(ProjectBuilder.GetLinearNegativeFullyCorrelatedProject());

            project.Settings.UncertaintyMethod = UncertaintyMethod.CrudeMonteCarlo;
            project.Run();

            Stochast stochast = project.Stochast;

            ClassicAssert.AreEqual(1.04, stochast.Mean, margin);
            ClassicAssert.AreEqual(0.59, stochast.Deviation, margin);
        }

        [Test]
        public void TestLinearPartialCorrelated()
        {
            var project = ProjectBuilder.GetUncertaintyProject(ProjectBuilder.GetLinearPartialCorrelatedProject());

            project.Settings.UncertaintyMethod = UncertaintyMethod.CrudeMonteCarlo;
            project.Run();

            Stochast stochast = project.Stochast;

            ClassicAssert.AreEqual(1.79, stochast.Mean, margin);
            ClassicAssert.AreEqual(0.76, stochast.Deviation, margin);
        }

        [Test]
        public void TestCorrelationMatrix()
        {
            var project = ProjectBuilder.GetUncertaintyProject(ProjectBuilder.GetLinearProject());
            project.Settings.UncertaintyMethod = UncertaintyMethod.CrudeMonteCarlo;
            project.Settings.CalculateCorrelations = true;
            project.Run();

            Stochast stochast1 = project.Stochast;

            var project2 = ProjectBuilder.GetUncertaintyProject(ProjectBuilder.GetUnbalancedLinearProject());
            project2.Settings.UncertaintyMethod = UncertaintyMethod.CrudeMonteCarlo;
            project2.Settings.CalculateCorrelations = true;
            project2.Run();

            Stochast stochast2 = project2.Stochast;

            CorrelationMatrix correlationMatrix = project.OutputCorrelationMatrix;

            double correlationValue = correlationMatrix.GetCorrelation(stochast1, stochast2);

            ClassicAssert.AreEqual(0.84, correlationValue, margin);
        }
    }
}
