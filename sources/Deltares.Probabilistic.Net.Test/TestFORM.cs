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
using Deltares.Statistics.Wrappers;
using NUnit.Framework;
using NUnit.Framework.Legacy;

namespace Deltares.Probabilistic.Wrapper.Test
{
    [TestFixture]
    public class TestFORM
    {
        private const double margin = 0.01;

        [Test]
        public void TestLinear()
        {
            ReliabilityProject project = ProjectBuilder.GetLinearProject();

            project.ReliabilityMethod = new FORM();
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
        public void TestLinearArray()
        {
            ReliabilityProject project = ProjectBuilder.GetLinearArrayProject();

            project.ReliabilityMethod = new FORM();
            DesignPoint designPoint = project.GetDesignPoint();

            ClassicAssert.AreEqual(0.71, designPoint.Beta, margin);
            ClassicAssert.IsTrue(designPoint.ConvergenceReport.IsConverged);

            ClassicAssert.AreEqual(0.24, designPoint.ProbabilityFailure, margin);
            ClassicAssert.AreEqual(0.76, designPoint.ProbabilityNonFailure, margin);
            ClassicAssert.AreEqual(3.7, designPoint.ReturnPeriod, margin * 10);
            ClassicAssert.AreEqual(11, designPoint.Alphas.Count);

            ClassicAssert.AreEqual(0, designPoint.Alphas[0].Alpha, margin);
            ClassicAssert.AreEqual(1.8, designPoint.Alphas[0].X, margin);
            ClassicAssert.AreEqual(0, designPoint.Alphas[0].Index);
            ClassicAssert.AreEqual(project.Stochasts[0], designPoint.Alphas[0].Parameter);

            ClassicAssert.AreEqual(0.31, designPoint.Alphas[1].Alpha, margin);
            ClassicAssert.AreEqual(-0.17, designPoint.Alphas[1].X, margin);
            ClassicAssert.AreEqual(0, designPoint.Alphas[1].Index);
            ClassicAssert.AreEqual(4, designPoint.Alphas[5].Index);
            ClassicAssert.AreEqual(project.Stochasts[1], designPoint.Alphas[1].Parameter);

            ClassicAssert.AreEqual(0.31, designPoint.Alphas[6].Alpha, margin);
            ClassicAssert.AreEqual(-0.17, designPoint.Alphas[6].X, margin);
            ClassicAssert.AreEqual(0, designPoint.Alphas[6].Index);
            ClassicAssert.AreEqual(project.Stochasts[2], designPoint.Alphas[6].Parameter);
        }

        [Test]
        public void TestLinearConditional()
        {
            ReliabilityProject project = ProjectBuilder.GetLinearProject();

            project.Stochasts[1].IsVariableStochast = true;

            VariableStochastValue conditional1 = new VariableStochastValue();
            conditional1.X = -1;
            conditional1.Minimum = -1.2;
            conditional1.Maximum = -0.8;
            project.Stochasts[1].ValueSet.StochastValues.Add(conditional1);

            VariableStochastValue conditional2 = new VariableStochastValue();
            conditional2.X = 1;
            conditional2.Minimum = 0.8;
            conditional2.Maximum = 1.2;
            project.Stochasts[1].ValueSet.StochastValues.Add(conditional2);

            project.Stochasts[1].VariableSource = project.Stochasts[0];

            project.ReliabilityMethod = new FORM();
            DesignPoint designPoint = project.GetDesignPoint();

            ClassicAssert.AreEqual(1.56, designPoint.Beta, margin);
            ClassicAssert.IsTrue(designPoint.ConvergenceReport.IsConverged);

            ClassicAssert.AreEqual(-0.96, designPoint.Alphas[0].Alpha, margin);
            ClassicAssert.AreEqual(-0.29, designPoint.Alphas[1].Alpha, margin);

            ClassicAssert.AreEqual(0.86, designPoint.Alphas[0].X, margin);
            ClassicAssert.AreEqual(0.93, designPoint.Alphas[1].X, margin);
        }

        [Test]
        public void TestLinearConditionalArray()
        {
            ReliabilityProject project = ProjectBuilder.GetLinearArrayProject();

            project.Stochasts[0].Name = "L";
            project.Stochasts[1].Name = "a";
            project.Stochasts[2].Name = "b";

            project.Stochasts[2].IsVariableStochast = true;

            VariableStochastValue conditional1 = new VariableStochastValue();
            conditional1.X = -1;
            conditional1.Minimum = -1.2;
            conditional1.Maximum = -0.8;
            project.Stochasts[2].ValueSet.StochastValues.Add(conditional1);

            VariableStochastValue conditional2 = new VariableStochastValue();
            conditional2.X = 1;
            conditional2.Minimum = 0.8;
            conditional2.Maximum = 1.2;
            project.Stochasts[2].ValueSet.StochastValues.Add(conditional2);

            project.Stochasts[2].VariableSource = project.Stochasts[1];

            project.ReliabilityMethod = new FORM();
            DesignPoint designPoint = project.GetDesignPoint();

            ClassicAssert.AreEqual(0.51, designPoint.Beta, margin);
            ClassicAssert.IsTrue(designPoint.ConvergenceReport.IsConverged);

            ClassicAssert.AreEqual(11, designPoint.Alphas.Count);

            ClassicAssert.AreEqual(0, designPoint.Alphas[0].Alpha, margin);
            ClassicAssert.AreEqual(1.8, designPoint.Alphas[0].X, margin);
            ClassicAssert.AreEqual(0, designPoint.Alphas[0].Index);
            ClassicAssert.AreEqual(project.Stochasts[0], designPoint.Alphas[0].Parameter);

            ClassicAssert.AreEqual(0.44, designPoint.Alphas[1].Alpha, margin);
            ClassicAssert.AreEqual(0.44, designPoint.Alphas[2].Alpha, margin);
            ClassicAssert.AreEqual(0.045, designPoint.Alphas[6].Alpha, margin / 10);
            ClassicAssert.AreEqual(0.045, designPoint.Alphas[7].Alpha, margin / 10);

            ClassicAssert.AreEqual(project.Stochasts[1], designPoint.Alphas[1].Parameter);
            ClassicAssert.AreEqual(project.Stochasts[1], designPoint.Alphas[2].Parameter);
            ClassicAssert.AreEqual(project.Stochasts[2], designPoint.Alphas[6].Parameter);
            ClassicAssert.AreEqual(project.Stochasts[2], designPoint.Alphas[7].Parameter);

            ClassicAssert.AreEqual(0, designPoint.Alphas[1].Index);
            ClassicAssert.AreEqual(1, designPoint.Alphas[2].Index);
            ClassicAssert.AreEqual(0, designPoint.Alphas[6].Index);
            ClassicAssert.AreEqual(1, designPoint.Alphas[7].Index);

            ClassicAssert.AreEqual(-0.18, designPoint.Alphas[1].X, margin);
            ClassicAssert.AreEqual(-0.18, designPoint.Alphas[2].X, margin);
            ClassicAssert.AreEqual(-0.18, designPoint.Alphas[6].X, margin);
            ClassicAssert.AreEqual(-0.18, designPoint.Alphas[7].X, margin);
        }

        [Test]
        public void TestLinearDoubleExecuted()
        {
            var project = ProjectBuilder.GetLinearProject();

            project.ReliabilityMethod = new FORM();
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

            project.ReliabilityMethod = new FORM();
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

            project.ReliabilityMethod = new FORM();
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

            project.ReliabilityMethod = new FORM();
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

            project.ReliabilityMethod = new FORM();
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

            project.ReliabilityMethod = new FORM();
            DesignPoint designPoint = project.GetDesignPoint();

            ClassicAssert.AreEqual(2.15, designPoint.Beta, margin);

            ClassicAssert.AreEqual(0.45, designPoint.Alphas[0].X, margin);
            ClassicAssert.AreEqual(0.45, designPoint.Alphas[1].X, margin);

            ClassicAssert.AreEqual(designPoint.Alphas[0].AlphaCorrelated, designPoint.Alphas[1].AlphaCorrelated, margin);
        }

        [Test]
        public void TestLinearMultiCorrelated()
        {
            ReliabilityProject project = ProjectBuilder.GetLinearMultiCorrelatedProject(false);

            project.CorrelationMatrix.ResolveConflictingCorrelations();

            project.ReliabilityMethod = new FORM();
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

            project.ReliabilityMethod = new FORM();
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

            project.ReliabilityMethod = new FORM();
            DesignPoint designPoint = project.GetDesignPoint();

            ClassicAssert.AreEqual(1.41, designPoint.Beta, margin);
        }

        [Test]
        public void TestConvexStartPoint()
        {
            ReliabilityProject project = ProjectBuilder.GetConvexProject();

            FORM form = new FORM();
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

            FORM form = new FORM();
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

            FORM form = new FORM();
            form.Settings.StartPointCalculatorSettings.StartMethod = StartMethodType.RaySearch;
            project.ReliabilityMethod = form;

            DesignPoint designPoint = project.GetDesignPoint();

            ClassicAssert.AreEqual(2.5, designPoint.Beta, margin);
        }

        [Test]
        public void TestConvexRaySphereStartPoint()
        {
            ReliabilityProject project = ProjectBuilder.GetConvexProject();

            FORM form = new FORM();
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

            FORM form = new FORM();
            form.Settings.StartPointCalculatorSettings.StartMethod = StartMethodType.SensitivitySearch;
            project.ReliabilityMethod = form;

            DesignPoint designPoint = project.GetDesignPoint();

            ClassicAssert.AreEqual(2.5, designPoint.Beta, margin);
        }

        [Test]
        public void TestNoisy()
        {
            ReliabilityProject project = ProjectBuilder.GetNoisyProject();

            project.ReliabilityMethod = new FORM();
            DesignPoint designPoint = project.GetDesignPoint();

            ClassicAssert.AreEqual(2.59, designPoint.Beta, margin);
        }

        [Test]
        public void TestOblateSpheroid()
        {
            ReliabilityProject project = ProjectBuilder.GetOblateSpheroidProject();

            project.ReliabilityMethod = new FORM();
            DesignPoint designPoint = project.GetDesignPoint();

            ClassicAssert.AreEqual(3.39, designPoint.Beta, margin);
        }

        [Test]
        public void TestQuadratic()
        {
            ReliabilityProject project = ProjectBuilder.GetQuadraticProject();

            project.ReliabilityMethod = new FORM();
            DesignPoint designPoint = project.GetDesignPoint();

            ClassicAssert.AreEqual(-4.5, designPoint.Beta, margin);
        }

        [Test]
        public void TestWaveRelaxationLoops()
        {
            ReliabilityProject project = ProjectBuilder.GetWaveProject();

            FORM form = new FORM();
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

            FORM form = new FORM();
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

            FORM form = new FORM();
            form.Settings.RelaxationFactor = 0.75;
            form.Settings.RelaxationLoops = 8;
            form.Settings.FilterAtNonConvergence = false;
            project.ReliabilityMethod = form;

            DesignPoint designPoint1 = project.GetDesignPoint();

            ClassicAssert.AreEqual(3.01, designPoint1.Beta, margin);
            ClassicAssert.AreEqual(7, designPoint1.ContributingDesignPoints.Count);
            ClassicAssert.AreEqual("Relaxation loop 1", designPoint1.ContributingDesignPoints[0].Identifier);

            ClassicAssert.IsFalse(designPoint1.ConvergenceReport.IsConverged);
            form.Settings.FilterAtNonConvergence = true;

            DesignPoint designPoint2 = project.GetDesignPoint();

            ClassicAssert.AreEqual(0.674, designPoint2.Beta, margin);
            ClassicAssert.AreEqual(7, designPoint2.ContributingDesignPoints.Count);
            ClassicAssert.AreEqual("Relaxation loop 1", designPoint2.ContributingDesignPoints[0].Identifier);

            ClassicAssert.IsFalse(designPoint2.ConvergenceReport.IsConverged);
        }

        [Test]
        public void TestBligh()
        {
            ReliabilityProject project = ProjectBuilder.GetBlighProject();

            project.ReliabilityMethod = new FORM();
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

            project.ReliabilityMethod = new FORM();
            DesignPoint designPoint = project.GetDesignPoint();

            ClassicAssert.AreEqual(40, designPoint.Beta, margin);
            ClassicAssert.AreEqual(1, designPoint.Messages.Count);

            ClassicAssert.AreEqual(MessageType.Error, designPoint.Messages[0].Type);
            ClassicAssert.AreEqual("No variation in model results found at start point", designPoint.Messages[0].Text);
        }
    }
}
