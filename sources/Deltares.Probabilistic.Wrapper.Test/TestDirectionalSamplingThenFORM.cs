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
﻿using Deltares.Models.Wrappers;
using Deltares.Reliability.Wrappers;
using NUnit.Framework;

namespace Deltares.Probabilistic.Wrapper.Test
{
    [TestFixture]
    public class TestDirectionalSamplingThenFORM
    {
        private const double margin = 0.01;

        [Test]
        public void TestLinear()
        {
            Project project = ProjectBuilder.GetLinearProject();

            project.ReliabilityMethod = new DirectionalSamplingThenFORM();
            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(2.5824, designPoint.Beta, margin);
            Assert.IsTrue(designPoint.ConvergenceReport.IsConverged);

            Assert.AreEqual(-0.680, designPoint.Alphas[0].Alpha, margin);
            Assert.AreEqual(-0.733, designPoint.Alphas[1].Alpha, margin);

            Assert.AreEqual(0.886, designPoint.Alphas[0].X, margin);
            Assert.AreEqual(0.912, designPoint.Alphas[1].X, margin);
        }

        [Test]
        public void TestLinearDoubleExecuted()
        {
            var project = ProjectBuilder.GetLinearProject();

            project.ReliabilityMethod = new DirectionalSamplingThenFORM();
            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(2.5824, designPoint.Beta, margin);

            DesignPoint designPoint2 = project.GetDesignPoint();

            Assert.AreEqual(2.5824, designPoint2.Beta, margin);

            Assert.AreNotSame(designPoint, designPoint2);
            Assert.AreNotSame(designPoint.Alphas[0], designPoint2.Alphas[0]);
            Assert.AreSame(designPoint.Alphas[0].Parameter, designPoint2.Alphas[0].Parameter);
        }

        [Test]
        public void TestInverseLinear()
        {
            var project = ProjectBuilder.GetInverseLinearProject();

            project.ReliabilityMethod = new DirectionalSamplingThenFORM();
            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(-2.582, designPoint.Beta, margin);
            Assert.IsTrue(designPoint.ConvergenceReport.IsConverged);

            Assert.AreEqual(0.680, designPoint.Alphas[0].Alpha, margin);
            Assert.AreEqual(0.733, designPoint.Alphas[1].Alpha, margin);

            Assert.AreEqual(0.886, designPoint.Alphas[0].X, margin);
            Assert.AreEqual(0.912, designPoint.Alphas[1].X, margin);
        }

        [Test]
        public void TestLinearMessages()
        {
            Project project = ProjectBuilder.GetLinearProject();

            project.Settings.SaveConvergence = true;
            project.Settings.SaveEvaluations = true;

            project.ReliabilityMethod = new DirectionalSamplingThenFORM();
            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(2.582, designPoint.Beta, margin);

            Assert.AreEqual(2, designPoint.ReliabilityResults.Count);
            Assert.AreEqual(6, designPoint.Evaluations.Count);

            Assert.IsTrue(designPoint.Evaluations[0].Tag is ZFunctionOutput);

            for (int i = 0; i < designPoint.ReliabilityResults.Count; i++)
            {
                Assert.AreEqual(i, designPoint.ReliabilityResults[i].Index);
                Assert.AreEqual(i, designPoint.Evaluations[3 * i].Iteration);
                Assert.AreEqual(i, designPoint.Evaluations[3 * i + 1].Iteration);
                Assert.AreEqual(i, designPoint.Evaluations[3 * i + 2].Iteration);
            }
        }

        [Test]
        [SetCulture("en-US")]
        public void TestLinearProgress()
        {
            Project project = ProjectBuilder.GetLinearProject();

            ProgressHolder progressHolder = new ProgressHolder();

            project.ProgressIndicator = new ProgressIndicator(progressHolder.SetProgress, progressHolder.SetDetailedProgress, progressHolder.SetTextualProgress);

            project.ReliabilityMethod = new DirectionalSamplingThenFORM();
            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(2.582, designPoint.Beta, margin);

            Assert.AreEqual(3719, progressHolder.Invocations);
            Assert.AreEqual(0.02, progressHolder.Progress, margin);
            Assert.AreEqual("1/50, Reliability = 2.325, Convergence = 0.006", progressHolder.Text);
            Assert.AreEqual(1, progressHolder.Step);
            Assert.AreEqual(0, progressHolder.Loop);
            Assert.AreEqual(2.325, progressHolder.Reliability, 1e-3);
            Assert.AreEqual(0.005536, progressHolder.Convergence, 1e-3);
        }

        [Test]
        public void TestLinearCorrelated()
        {
            Project project = ProjectBuilder.GetLinearFullyCorrelatedProject();

            project.ReliabilityMethod = new DirectionalSamplingThenFORM();
            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(2.55, designPoint.Beta, margin);

            Assert.AreEqual(0.467, designPoint.Alphas[0].X, margin);
            Assert.AreEqual(0.467, designPoint.Alphas[1].X, margin);
            Assert.AreEqual(0.549, designPoint.Alphas[2].X, margin);

            Assert.AreEqual(0, designPoint.Alphas[1].Alpha, margin);
            Assert.AreEqual(designPoint.Alphas[0].AlphaCorrelated, designPoint.Alphas[1].AlphaCorrelated, margin);
        }

        [Test]
        public void TestLinearPartialCorrelated()
        {
            Project project = ProjectBuilder.GetLinearPartialCorrelatedProject();

            project.ReliabilityMethod = new DirectionalSamplingThenFORM();

            DirectionalSamplingThenFORM fdir = new DirectionalSamplingThenFORM();
            fdir.Settings.MinimumDirections = 10000;
            fdir.Settings.MaximumDirections = 50000;
            fdir.Settings.RandomSettings.RandomGeneratorType = RandomGeneratorType.MersenneTwister;
            project.ReliabilityMethod = fdir;
            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(2.85, designPoint.Beta, margin);

            Assert.AreEqual(0.4475, designPoint.Alphas[0].X, margin);
            Assert.AreEqual(0.4472, designPoint.Alphas[1].X, margin);

            Assert.AreEqual(designPoint.Alphas[0].AlphaCorrelated, designPoint.Alphas[1].AlphaCorrelated, margin);
        }

        [Test]
        public void TestLinearMultiCorrelated()
        {
            Project project = ProjectBuilder.GetLinearMultiCorrelatedProject(false);

            project.CorrelationMatrix.ResolveConflictingCorrelations();

            project.ReliabilityMethod = new DirectionalSamplingThenFORM();
            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(1.65, designPoint.Beta, margin);

            Assert.AreEqual(0.45, designPoint.Alphas[0].X, margin);
            Assert.AreEqual(0.45, designPoint.Alphas[1].X, margin);
            Assert.AreEqual(0.90, designPoint.Alphas[2].X, margin);

            Assert.AreEqual(0, designPoint.Alphas[1].Alpha, margin);
            Assert.AreEqual(0, designPoint.Alphas[2].Alpha, margin);

            Assert.AreEqual(designPoint.Alphas[0].AlphaCorrelated, designPoint.Alphas[1].AlphaCorrelated, margin);
            Assert.AreEqual(designPoint.Alphas[0].AlphaCorrelated, designPoint.Alphas[2].AlphaCorrelated, margin);
        }

        [Test]
        public void TestLinearMultiCorrelatedSwitchOrder()
        {
            Project project = ProjectBuilder.GetLinearMultiCorrelatedProject(true);

            project.CorrelationMatrix.ResolveConflictingCorrelations();

            project.ReliabilityMethod = new DirectionalSamplingThenFORM();
            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(1.65, designPoint.Beta, margin);

            Assert.AreEqual(0.45, designPoint.Alphas[0].X, margin);
            Assert.AreEqual(0.45, designPoint.Alphas[1].X, margin);
            Assert.AreEqual(0.90, designPoint.Alphas[2].X, margin);

            Assert.AreEqual(-1.0, designPoint.Alphas[0].Alpha, margin);
            Assert.AreEqual(0.0, designPoint.Alphas[1].Alpha, margin);
            Assert.AreEqual(0.0, designPoint.Alphas[2].Alpha, margin);

            Assert.AreEqual(designPoint.Alphas[0].AlphaCorrelated, designPoint.Alphas[1].AlphaCorrelated, margin);
            Assert.AreEqual(designPoint.Alphas[0].AlphaCorrelated, designPoint.Alphas[2].AlphaCorrelated, margin);
        }


        [Test]
        public void TestLoadStrength()
        {
            Project project = ProjectBuilder.GetLoadStrengthProject();

            project.ReliabilityMethod = new DirectionalSamplingThenFORM();
            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(1.40, designPoint.Beta, margin);
        }

        [Test]
        public void TestConvexStartPoint()
        {
            Project project = ProjectBuilder.GetConvexProject();

            DirectionalSamplingThenFORM form = new DirectionalSamplingThenFORM();
            form.Settings.StochastSettings.Add(new StochastSettings { StartValue = 1, Stochast = project.Stochasts[0] });
            form.Settings.StochastSettings.Add(new StochastSettings { StartValue = 1, Stochast = project.Stochasts[1] });
            project.ReliabilityMethod = form;

            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(2.635, designPoint.Beta, margin);
        }

        [Test]
        public void TestNoisy()
        {
            Project project = ProjectBuilder.GetNoisyProject();

            project.ReliabilityMethod = new DirectionalSamplingThenFORM();
            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(2.505, designPoint.Beta, margin);
        }

        [Test]
        public void TestOblateSpheroid()
        {
            Project project = ProjectBuilder.GetOblateSpheroidProject();

            project.ReliabilityMethod = new DirectionalSamplingThenFORM();
            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(1.10, designPoint.Beta, margin);
        }

        [Test]
        public void TestQuadratic()
        {
            Project project = ProjectBuilder.GetQuadraticProject();

            project.ReliabilityMethod = new DirectionalSamplingThenFORM();
            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(-4.417, designPoint.Beta, margin);
        }

        [Test]
        public void TestWaveRelaxationLoops()
        {
            Project project = ProjectBuilder.GetWaveProject();

            DirectionalSamplingThenFORM form = new DirectionalSamplingThenFORM();
            form.Settings.RelaxationFactor = 0.7;
            form.Settings.RelaxationLoops = 5;
            project.ReliabilityMethod = form;

            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(2.076, designPoint.Beta, margin);
            Assert.AreEqual(3, designPoint.ContributingDesignPoints.Count);
        }

        [Test]
        public void TestWaveRelaxationLoopsDoubleGradient()
        {
            Project project = ProjectBuilder.GetWaveProject();

            DirectionalSamplingThenFORM form = new DirectionalSamplingThenFORM();
            form.Settings.RelaxationFactor = 0.7;
            form.Settings.RelaxationLoops = 5;
            form.Settings.GradientCalculatorSettings.GradientType = GradientType.TwoDirections;
            project.ReliabilityMethod = form;

            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(2.08, designPoint.Beta, margin);
            Assert.AreEqual(1, designPoint.ContributingDesignPoints.Count);

            Assert.IsTrue(designPoint.ConvergenceReport.IsConverged);
        }

        [Test]
        public void TestNonLinear()
        {
            Project project = ProjectBuilder.GetNonLinearProject();

            DirectionalSamplingThenFORM fdir = new DirectionalSamplingThenFORM();
            fdir.Settings.RelaxationFactor = 0.75;
            fdir.Settings.RelaxationLoops = 4;
            fdir.Settings.FilterAtNonConvergence = false;
            fdir.Settings.MinimumDirections = 10000;
            fdir.Settings.MaximumDirections = 50000;
            fdir.Settings.VariationCoefficient = 0.04;
            fdir.Settings.RandomSettings.RandomGeneratorType = RandomGeneratorType.MersenneTwister;
            fdir.Settings.RandomSettings.Seed = 1234;
            project.ReliabilityMethod = fdir;

            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(3.57, designPoint.Beta, margin);
            Assert.AreEqual(4, designPoint.ContributingDesignPoints.Count);

            Assert.IsFalse(designPoint.ConvergenceReport.IsConverged);
        }

        [Test]
        public void TestBligh()
        {
            Project project = ProjectBuilder.GetBlighProject();

            project.ReliabilityMethod = new DirectionalSamplingThenFORM();
            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(1.6566, designPoint.Beta, margin);
        }

        [Test]
        public void TestNonVarying()
        {
            Project project = ProjectBuilder.GetNonVaryingProject();

            project.Settings.SaveMessages = true;
            project.Settings.MaxMessages = 1000;
            project.Settings.LowestMessageType = MessageType.Warning;

            project.ReliabilityMethod = new DirectionalSamplingThenFORM();
            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(40.0, designPoint.Beta, margin);
            Assert.AreEqual(1, designPoint.Messages.Count);

            Assert.AreEqual(MessageType.Error, designPoint.Messages[0].Type);
            Assert.AreEqual("No variation in model results found at start point", designPoint.Messages[0].Text);
        }

    }
}
