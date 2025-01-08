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
﻿using System;
using System.Threading;
using NUnit.Framework;
using Deltares.Models.Wrappers;
using Deltares.Statistics.Wrappers;
using Deltares.Sensitivity.Wrappers;

namespace Deltares.Probabilistic.Wrapper.Test
{
    [TestFixture]
    public class TestCrudeMonteCarloSensitivity
    {
        private const double margin = 0.02;

        [Test]
        public void TestAddOne()
        {
            var project = ProjectBuilder.GetSensitivityProject(ProjectBuilder.GetAddOneProject());

            project.SensitivityMethod = new CrudeMonteCarloS();
            ((CrudeMonteCarloS)project.SensitivityMethod).Settings.RandomSettings.RandomGeneratorType = RandomGeneratorType.MersenneTwister;

            Stochast stochast = project.GetStochast();

            Assert.AreEqual(1, stochast.Mean, margin);

            stochast.DistributionType = DistributionType.Uniform;

            Assert.AreEqual(0, stochast.Minimum, margin);
            Assert.AreEqual(2, stochast.Maximum, margin);
        }

        [Test]
        public void TestLinear()
        {
            var project = ProjectBuilder.GetSensitivityProject(ProjectBuilder.GetLinearProject());

            project.SensitivityMethod = new CrudeMonteCarloS();
            ((CrudeMonteCarloS)project.SensitivityMethod).Settings.RandomSettings.RandomGeneratorType = RandomGeneratorType.MersenneTwister;

            Stochast stochast = project.GetStochast();

            Assert.AreEqual(DistributionType.Histogram, stochast.DistributionType);
            Assert.AreEqual(84, stochast.HistogramValues.Count);

            Assert.AreEqual(1.8, stochast.Mean, margin);
            Assert.AreEqual(0.82, stochast.Deviation, margin);
        }

        [Test]
        public void TestLinearDoubleExecuted()
        {
            var project = ProjectBuilder.GetSensitivityProject(ProjectBuilder.GetLinearProject());

            project.SensitivityMethod = new CrudeMonteCarloS();
            ((CrudeMonteCarloS)project.SensitivityMethod).Settings.RandomSettings.RandomGeneratorType = RandomGeneratorType.MersenneTwister;

            Stochast stochast = project.GetStochast();

            Assert.AreEqual(1.8, stochast.Mean, margin);
            Assert.AreEqual(0.82, stochast.Deviation, margin);

            Stochast stochast2 = project.GetStochast();

            Assert.AreNotSame(stochast, stochast2);
            Assert.AreEqual(stochast.Mean, stochast2.Mean);
        }

        [Test]
        public void TestLinearParallel()
        {
            var project = ProjectBuilder.GetSensitivityProject(ProjectBuilder.GetLinearProject());
            project.Settings.MaxParallelProcesses = 4;

            project.SensitivityMethod = new CrudeMonteCarloS();
            ((CrudeMonteCarloS)project.SensitivityMethod).Settings.RandomSettings.RandomGeneratorType = RandomGeneratorType.MersenneTwister;

            Stochast stochast = project.GetStochast();

            Assert.AreEqual(1.8, stochast.Mean, margin);
            Assert.AreEqual(0.82, stochast.Deviation, margin);
        }

        [Test]
        public void TestLinearManySamples()
        {
            var project = ProjectBuilder.GetSensitivityProject(ProjectBuilder.GetLinearProject());
            project.Settings.SaveConvergence = false;
            project.Settings.SaveEvaluations = false;
            project.Settings.SaveMessages = false;

            project.SensitivityMethod = new CrudeMonteCarloS();
            ((CrudeMonteCarloS)project.SensitivityMethod).Settings.RandomSettings.RandomGeneratorType = RandomGeneratorType.MersenneTwister;
            ((CrudeMonteCarloS)project.SensitivityMethod).Settings.Samples = 100000;

            Stochast stochast = project.GetStochast();

            Assert.AreEqual(1.8, stochast.Mean, margin);
            Assert.AreEqual(0.82, stochast.Deviation, margin);

            stochast.DistributionType = DistributionType.Uniform;

            Assert.AreEqual(-0.2, stochast.Minimum, 10 * margin);
            Assert.AreEqual(3.8, stochast.Maximum, 10 * margin);
        }

        [Test]
        public void TestLinearAutoSamples()
        {
            var project = ProjectBuilder.GetSensitivityProject(ProjectBuilder.GetLinearProject());
            project.Settings.SaveConvergence = false;
            project.Settings.SaveEvaluations = false;
            project.Settings.SaveMessages = false;

            project.SensitivityMethod = new CrudeMonteCarloS();
            ((CrudeMonteCarloS)project.SensitivityMethod).Settings.RandomSettings.RandomGeneratorType = RandomGeneratorType.MersenneTwister;
            ((CrudeMonteCarloS)project.SensitivityMethod).Settings.DeriveSamplesFromVariationCoefficient = true;

            Assert.AreEqual(7600, ((CrudeMonteCarloS)project.SensitivityMethod).Settings.GetRequiredSamples());

            Stochast stochast = project.GetStochast();

            Assert.AreEqual(1.8, stochast.Mean, margin);
            Assert.AreEqual(0.82, stochast.Deviation, margin);
        }

        [Test]
        public void TestLinearFullyCorrelated()
        {
            var project = ProjectBuilder.GetSensitivityProject(ProjectBuilder.GetLinearFullyCorrelatedProject());
            project.SensitivityMethod = new CrudeMonteCarloS();
            ((CrudeMonteCarloS)project.SensitivityMethod).Settings.RandomSettings.RandomGeneratorType = RandomGeneratorType.MersenneTwister;

            Stochast stochast = project.GetStochast();

            Assert.AreEqual(1.79, stochast.Mean, margin);
            Assert.AreEqual(0.82, stochast.Deviation, margin);

            stochast.DistributionType = DistributionType.Uniform;

            Assert.AreEqual(-0.2, stochast.Minimum, 10 * margin);
            Assert.AreEqual(3.8, stochast.Maximum, 10 * margin);
        }

        [Test]
        public void TestLinearNegativeFullyCorrelated()
        {
            var project = ProjectBuilder.GetSensitivityProject(ProjectBuilder.GetLinearNegativeFullyCorrelatedProject());
            project.SensitivityMethod = new CrudeMonteCarloS();
            ((CrudeMonteCarloS)project.SensitivityMethod).Settings.RandomSettings.RandomGeneratorType = RandomGeneratorType.MersenneTwister;

            Stochast stochast = project.GetStochast();

            Assert.AreEqual(1.04, stochast.Mean, margin);
            Assert.AreEqual(0.59, stochast.Deviation, margin);
        }

        [Test]
        public void TestLinearPartialCorrelated()
        {
            var project = ProjectBuilder.GetSensitivityProject(ProjectBuilder.GetLinearPartialCorrelatedProject());
            project.SensitivityMethod = new CrudeMonteCarloS();
            ((CrudeMonteCarloS)project.SensitivityMethod).Settings.RandomSettings.RandomGeneratorType = RandomGeneratorType.MersenneTwister;

            Stochast stochast = project.GetStochast();

            Assert.AreEqual(1.79, stochast.Mean, margin);
            Assert.AreEqual(0.76, stochast.Deviation, margin);
        }

        [Test]
        public void TestCorrelationMatrix()
        {
            CrudeMonteCarloS sensitivityMethod = new CrudeMonteCarloS();

            sensitivityMethod.Settings.RandomSettings.RandomGeneratorType = RandomGeneratorType.MersenneTwister;
            sensitivityMethod.Settings.CalculateCorrelations = true;

            var project = ProjectBuilder.GetSensitivityProject(ProjectBuilder.GetLinearProject());
            project.SensitivityMethod = sensitivityMethod;
            Stochast stochast1 = project.GetStochast();

            var project2 = ProjectBuilder.GetSensitivityProject(ProjectBuilder.GetUnbalancedLinearProject());
            project2.SensitivityMethod = sensitivityMethod;
            Stochast stochast2 = project2.GetStochast();

            CorrelationMatrix correlationMatrix = sensitivityMethod.GetCorrelationMatrix();

            double correlationValue = correlationMatrix.GetCorrelation(stochast1, stochast2);

            Assert.AreEqual(0.84, correlationValue, margin);
        }
    }
}
