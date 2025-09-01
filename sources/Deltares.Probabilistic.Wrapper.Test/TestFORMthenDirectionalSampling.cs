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
﻿using Deltares.Models.Wrappers;
using Deltares.Reliability.Wrappers;
using NUnit.Framework;
using NUnit.Framework.Legacy;

namespace Deltares.Probabilistic.Wrapper.Test
{
    [TestFixture]
    public class TestFORMthenDirectionalSampling
    {
        private const double margin = 0.01;

        [Test]
        public void TestLinear()
        {
            ReliabilityProject project = ProjectBuilder.GetLinearProject();

            project.ReliabilityMethod = new FORMThenDirectionalSampling();
            DesignPoint designPoint = project.GetDesignPoint();

            ClassicAssert.AreEqual(2.33, designPoint.Beta, margin);
            ClassicAssert.IsTrue(designPoint.ConvergenceReport.IsConverged);

            ClassicAssert.AreEqual(0.01, designPoint.ProbabilityFailure, margin / 10);
            ClassicAssert.AreEqual(0.99, designPoint.ProbabilityNonFailure, margin / 10);
            ClassicAssert.AreEqual(99.7, designPoint.ReturnPeriod, margin * 10);

            ClassicAssert.AreEqual(-0.7, designPoint.Alphas[0].Alpha, margin);
            ClassicAssert.AreEqual(-0.7, designPoint.Alphas[1].Alpha, margin);

            ClassicAssert.AreEqual(0.9, designPoint.Alphas[0].X, margin);
            ClassicAssert.AreEqual(0.9, designPoint.Alphas[1].X, margin);
        }

        [Test]
        public void TestLinearDoubleExecuted()
        {
            var project = ProjectBuilder.GetLinearProject();

            project.ReliabilityMethod = new FORMThenDirectionalSampling();
            DesignPoint designPoint = project.GetDesignPoint();

            ClassicAssert.AreEqual(2.33, designPoint.Beta, margin);

            DesignPoint designPoint2 = project.GetDesignPoint();

            ClassicAssert.AreEqual(2.33, designPoint2.Beta, margin);

            ClassicAssert.AreNotSame(designPoint, designPoint2);
            ClassicAssert.AreNotSame(designPoint.Alphas[0], designPoint2.Alphas[0]);
            ClassicAssert.AreSame(designPoint.Alphas[0].Parameter, designPoint2.Alphas[0].Parameter);
        }

        [Test]
        public void TestInverseLinear()
        {
            var project = ProjectBuilder.GetInverseLinearProject();

            project.ReliabilityMethod = new FORMThenDirectionalSampling();
            DesignPoint designPoint = project.GetDesignPoint();

            ClassicAssert.AreEqual(-2.33, designPoint.Beta, margin);
            ClassicAssert.IsTrue(designPoint.ConvergenceReport.IsConverged);

            ClassicAssert.AreEqual(0.7, designPoint.Alphas[0].Alpha, margin);
            ClassicAssert.AreEqual(0.7, designPoint.Alphas[1].Alpha, margin);

            ClassicAssert.AreEqual(0.9, designPoint.Alphas[0].X, margin);
            ClassicAssert.AreEqual(0.9, designPoint.Alphas[1].X, margin);
        }

        [Test]
        public void TestLinearMessages()
        {
            ReliabilityProject project = ProjectBuilder.GetLinearProject();

            project.Settings.SaveConvergence = true;
            project.Settings.SaveEvaluations = true;

            project.ReliabilityMethod = new FORMThenDirectionalSampling();
            DesignPoint designPoint = project.GetDesignPoint();

            ClassicAssert.AreEqual(2.33, designPoint.Beta, margin);

            ClassicAssert.AreEqual(6, designPoint.ReliabilityResults.Count);
            ClassicAssert.AreEqual(18, designPoint.Evaluations.Count);

            ClassicAssert.IsTrue(designPoint.Evaluations[0].Tag is ZFunctionOutput);

            for (int i = 0; i < designPoint.ReliabilityResults.Count; i++)
            {
                ClassicAssert.AreEqual(i, designPoint.ReliabilityResults[i].Index);
                ClassicAssert.AreEqual(i, designPoint.Evaluations[3 * i].Iteration);
                ClassicAssert.AreEqual(i, designPoint.Evaluations[3 * i + 1].Iteration);
                ClassicAssert.AreEqual(i, designPoint.Evaluations[3 * i + 2].Iteration);
            }
        }

        [Test]
        [SetCulture("en-US")]
        public void TestLinearProgress()
        {
            ReliabilityProject project = ProjectBuilder.GetLinearProject();

            ProgressHolder progressHolder = new ProgressHolder();

            project.ProgressIndicator = new ProgressIndicator(progressHolder.SetProgress, progressHolder.SetDetailedProgress, progressHolder.SetTextualProgress);

            project.ReliabilityMethod = new FORMThenDirectionalSampling();
            DesignPoint designPoint = project.GetDesignPoint();

            ClassicAssert.AreEqual(2.33, designPoint.Beta, margin);

            ClassicAssert.AreEqual(6, progressHolder.Invocations);
            ClassicAssert.AreEqual(0.1, progressHolder.Progress, margin);
            ClassicAssert.AreEqual("5/50, Reliability = 2.327, Convergence = 0.009", progressHolder.Text);
            ClassicAssert.AreEqual(5, progressHolder.Step);
            ClassicAssert.AreEqual(0, progressHolder.Loop);
            ClassicAssert.AreEqual(2.327, progressHolder.Reliability, 1e-3);
            ClassicAssert.AreEqual(0.009, progressHolder.Convergence, 1e-3);
        }

        [Test]
        public void TestLinearCorrelated()
        {
            ReliabilityProject project = ProjectBuilder.GetLinearFullyCorrelatedProject();

            project.ReliabilityMethod = new FORMThenDirectionalSampling();
            DesignPoint designPoint = project.GetDesignPoint();

            ClassicAssert.AreEqual(2.33, designPoint.Beta, margin);

            ClassicAssert.AreEqual(0.45, designPoint.Alphas[0].X, margin);
            ClassicAssert.AreEqual(0.45, designPoint.Alphas[1].X, margin);
            ClassicAssert.AreEqual(0.90, designPoint.Alphas[2].X, margin);

            ClassicAssert.AreEqual(0, designPoint.Alphas[1].Alpha, margin);
            ClassicAssert.AreEqual(designPoint.Alphas[0].AlphaCorrelated, designPoint.Alphas[1].AlphaCorrelated, margin);
        }

        [Test]
        public void TestLinearPartialCorrelated()
        {
            ReliabilityProject project = ProjectBuilder.GetLinearPartialCorrelatedProject();

            project.ReliabilityMethod = new FORMThenDirectionalSampling();

            FORMThenDirectionalSampling fdir = new FORMThenDirectionalSampling();
            fdir.Settings.MinimumDirections = 10000;
            fdir.Settings.MaximumDirections = 50000;
            fdir.Settings.RandomSettings.RandomGeneratorType = RandomGeneratorType.MersenneTwister;
            project.ReliabilityMethod = fdir;
            DesignPoint designPoint = project.GetDesignPoint();

            ClassicAssert.AreEqual(2.835, designPoint.Beta, margin);

            ClassicAssert.AreEqual(0.4656, designPoint.Alphas[0].X, margin);
            ClassicAssert.AreEqual(0.4658, designPoint.Alphas[1].X, margin);

            ClassicAssert.AreEqual(designPoint.Alphas[0].AlphaCorrelated, designPoint.Alphas[1].AlphaCorrelated, margin);
        }

        [Test]
        public void TestLinearMultiCorrelated()
        {
            ReliabilityProject project = ProjectBuilder.GetLinearMultiCorrelatedProject(false);

            project.CorrelationMatrix.ResolveConflictingCorrelations();

            project.ReliabilityMethod = new FORMThenDirectionalSampling();
            DesignPoint designPoint = project.GetDesignPoint();

            ClassicAssert.AreEqual(1.65, designPoint.Beta, margin);

            ClassicAssert.AreEqual(0.45, designPoint.Alphas[0].X, margin);
            ClassicAssert.AreEqual(0.45, designPoint.Alphas[1].X, margin);
            ClassicAssert.AreEqual(0.90, designPoint.Alphas[2].X, margin);

            ClassicAssert.AreEqual(0, designPoint.Alphas[1].Alpha, margin);
            ClassicAssert.AreEqual(0, designPoint.Alphas[2].Alpha, margin);

            ClassicAssert.AreEqual(designPoint.Alphas[0].AlphaCorrelated, designPoint.Alphas[1].AlphaCorrelated, margin);
            ClassicAssert.AreEqual(designPoint.Alphas[0].AlphaCorrelated, designPoint.Alphas[2].AlphaCorrelated, margin);
        }

        [Test]
        public void TestLinearMultiCorrelatedSwitchOrder()
        {
            ReliabilityProject project = ProjectBuilder.GetLinearMultiCorrelatedProject(true);

            project.CorrelationMatrix.ResolveConflictingCorrelations();

            project.ReliabilityMethod = new FORMThenDirectionalSampling();
            DesignPoint designPoint = project.GetDesignPoint();

            ClassicAssert.AreEqual(1.65, designPoint.Beta, margin);

            ClassicAssert.AreEqual(0.45, designPoint.Alphas[0].X, margin);
            ClassicAssert.AreEqual(0.45, designPoint.Alphas[1].X, margin);
            ClassicAssert.AreEqual(0.90, designPoint.Alphas[2].X, margin);

            ClassicAssert.AreEqual(-1.0, designPoint.Alphas[0].Alpha, margin);
            ClassicAssert.AreEqual(0.0, designPoint.Alphas[1].Alpha, margin);
            ClassicAssert.AreEqual(0.0, designPoint.Alphas[2].Alpha, margin);

            ClassicAssert.AreEqual(designPoint.Alphas[0].AlphaCorrelated, designPoint.Alphas[1].AlphaCorrelated, margin);
            ClassicAssert.AreEqual(designPoint.Alphas[0].AlphaCorrelated, designPoint.Alphas[2].AlphaCorrelated, margin);
        }


        [Test]
        public void TestLoadStrength()
        {
            ReliabilityProject project = ProjectBuilder.GetLoadStrengthProject();

            project.ReliabilityMethod = new FORMThenDirectionalSampling();
            DesignPoint designPoint = project.GetDesignPoint();

            ClassicAssert.AreEqual(1.41, designPoint.Beta, margin);
        }

        [Test]
        public void TestConvexStartPoint()
        {
            ReliabilityProject project = ProjectBuilder.GetConvexProject();

            FORMThenDirectionalSampling form = new FORMThenDirectionalSampling();
            form.Settings.StochastSettings.Add(new StochastSettings { StartValue = 1, Stochast = project.Stochasts[0] });
            form.Settings.StochastSettings.Add(new StochastSettings { StartValue = 1, Stochast = project.Stochasts[1] });
            project.ReliabilityMethod = form;

            DesignPoint designPoint = project.GetDesignPoint();

            ClassicAssert.AreEqual(2.5, designPoint.Beta, margin);
        }

        [Test]
        public void TestConvexRayStartPoint()
        {
            ReliabilityProject project = ProjectBuilder.GetConvexProject();

            FORMThenDirectionalSampling form = new FORMThenDirectionalSampling();
            form.Settings.StochastSettings.Add(new StochastSettings { StartValue = 1, Stochast = project.Stochasts[0] });
            form.Settings.StochastSettings.Add(new StochastSettings { StartValue = 1, Stochast = project.Stochasts[1] });
            form.Settings.StartPointCalculatorSettings.StartMethod = StartMethodType.RaySearch;
            project.ReliabilityMethod = form;

            DesignPoint designPoint = project.GetDesignPoint();

            ClassicAssert.AreEqual(2.5, designPoint.Beta, margin);
        }

        [Test]
        public void TestConvexRayStartPointNoInitialization()
        {
            ReliabilityProject project = ProjectBuilder.GetConvexProject();

            FORMThenDirectionalSampling form = new FORMThenDirectionalSampling();
            form.Settings.StartPointCalculatorSettings.StartMethod = StartMethodType.RaySearch;
            project.ReliabilityMethod = form;

            DesignPoint designPoint = project.GetDesignPoint();

            ClassicAssert.AreEqual(2.5, designPoint.Beta, margin);
        }

        [Test]
        public void TestConvexRaySphereStartPoint()
        {
            ReliabilityProject project = ProjectBuilder.GetConvexProject();

            FORMThenDirectionalSampling form = new FORMThenDirectionalSampling();
            form.Settings.StartPointCalculatorSettings.StartMethod = StartMethodType.SphereSearch;
            form.Settings.StartPointCalculatorSettings.AllQuadrants = true;
            project.ReliabilityMethod = form;

            DesignPoint designPoint = project.GetDesignPoint();

            ClassicAssert.AreEqual(2.5, designPoint.Beta, margin);

            ClassicAssert.AreEqual(1, designPoint.ContributingDesignPoints.Count, margin);
            ClassicAssert.AreEqual(3.54, designPoint.ContributingDesignPoints[0].Beta, margin);
        }

        [Test]
        public void TestConvexRaySensitivityStartPoint()
        {
            ReliabilityProject project = ProjectBuilder.GetConvexProject();

            FORMThenDirectionalSampling form = new FORMThenDirectionalSampling();
            form.Settings.StartPointCalculatorSettings.StartMethod = StartMethodType.SensitivitySearch;
            project.ReliabilityMethod = form;

            DesignPoint designPoint = project.GetDesignPoint();

            ClassicAssert.AreEqual(2.5, designPoint.Beta, margin);
        }

        [Test]
        public void TestNoisy()
        {
            ReliabilityProject project = ProjectBuilder.GetNoisyProject();

            project.ReliabilityMethod = new FORMThenDirectionalSampling();
            DesignPoint designPoint = project.GetDesignPoint();

            ClassicAssert.AreEqual(2.59, designPoint.Beta, margin);
        }

        [Test]
        public void TestOblateSpheroid()
        {
            ReliabilityProject project = ProjectBuilder.GetOblateSpheroidProject();

            project.ReliabilityMethod = new FORMThenDirectionalSampling();
            DesignPoint designPoint = project.GetDesignPoint();

            ClassicAssert.AreEqual(3.39, designPoint.Beta, margin);
        }

        [Test]
        public void TestQuadratic()
        {
            ReliabilityProject project = ProjectBuilder.GetQuadraticProject();

            project.ReliabilityMethod = new FORMThenDirectionalSampling();
            DesignPoint designPoint = project.GetDesignPoint();

            ClassicAssert.AreEqual(-4.5, designPoint.Beta, margin);
        }

        [Test]
        public void TestWaveRelaxationLoops()
        {
            ReliabilityProject project = ProjectBuilder.GetWaveProject();

            FORMThenDirectionalSampling form = new FORMThenDirectionalSampling();
            form.Settings.RelaxationFactor = 0.7;
            form.Settings.RelaxationLoops = 5;
            project.ReliabilityMethod = form;

            DesignPoint designPoint = project.GetDesignPoint();

            ClassicAssert.AreEqual(2.09, designPoint.Beta, margin);
            ClassicAssert.AreEqual(2, designPoint.ContributingDesignPoints.Count);
        }

        [Test]
        public void TestWaveRelaxationLoopsDoubleGradient()
        {
            ReliabilityProject project = ProjectBuilder.GetWaveProject();

            FORMThenDirectionalSampling form = new FORMThenDirectionalSampling();
            form.Settings.RelaxationFactor = 0.7;
            form.Settings.RelaxationLoops = 5;
            form.Settings.GradientCalculatorSettings.GradientType = GradientType.TwoDirections;
            project.ReliabilityMethod = form;

            DesignPoint designPoint = project.GetDesignPoint();

            ClassicAssert.AreEqual(2.08, designPoint.Beta, margin);
            ClassicAssert.AreEqual(1, designPoint.ContributingDesignPoints.Count);

            ClassicAssert.IsTrue(designPoint.ConvergenceReport.IsConverged);
        }

        [Test]
        public void TestNonLinear()
        {
            ReliabilityProject project = ProjectBuilder.GetNonLinearProject();

            FORMThenDirectionalSampling fdir = new FORMThenDirectionalSampling();
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

            ClassicAssert.AreEqual(3.594, designPoint.Beta, margin);
            ClassicAssert.AreEqual(1, designPoint.ContributingDesignPoints.Count);

            ClassicAssert.IsTrue(designPoint.ConvergenceReport.IsConverged);
        }

        [Test]
        public void TestBligh()
        {
            ReliabilityProject project = ProjectBuilder.GetBlighProject();

            project.ReliabilityMethod = new FORMThenDirectionalSampling();
            DesignPoint designPoint = project.GetDesignPoint();

            ClassicAssert.AreEqual(1.63, designPoint.Beta, margin);
        }

        [Test]
        public void TestNonVarying()
        {
            ReliabilityProject project = ProjectBuilder.GetNonVaryingProject();

            project.Settings.SaveMessages = true;
            project.Settings.MaxMessages = 1000;
            project.Settings.LowestMessageType = MessageType.Warning;

            project.ReliabilityMethod = new FORMThenDirectionalSampling();
            DesignPoint designPoint = project.GetDesignPoint();

            ClassicAssert.AreEqual(40.0, designPoint.Beta, margin);
            ClassicAssert.AreEqual(1, designPoint.ContributingDesignPoints[0].Messages.Count);

            ClassicAssert.AreEqual(MessageType.Error, designPoint.ContributingDesignPoints[0].Messages[0].Type);
            ClassicAssert.AreEqual("No variation in model results found at start point", designPoint.ContributingDesignPoints[0].Messages[0].Text);
        }

    }
}
