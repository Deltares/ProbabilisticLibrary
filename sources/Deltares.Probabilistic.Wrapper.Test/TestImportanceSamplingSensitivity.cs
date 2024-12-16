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
﻿using System;
using System.Threading;
using NUnit.Framework;
using Deltares.Models.Wrappers;
using Deltares.Statistics.Wrappers;
using Deltares.Sensitivity.Wrappers;

namespace Deltares.Probabilistic.Wrapper.Test
{
    [TestFixture]
    public class TestImportanceSamplingSensitivity
    {
        private const double margin = 0.02;

        [Test]
        public void TestAddOne()
        {
            var project = ProjectBuilder.GetSensitivityProject(ProjectBuilder.GetAddOneProject());

            project.SensitivityMethod = new ImportanceSamplingS();
            ((ImportanceSamplingS)project.SensitivityMethod).Settings.RandomSettings.RandomGeneratorType = RandomGeneratorType.MersenneTwister;

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

            project.SensitivityMethod = new ImportanceSamplingS();
            ((ImportanceSamplingS)project.SensitivityMethod).Settings.RandomSettings.RandomGeneratorType = RandomGeneratorType.MersenneTwister;

            Stochast stochast = project.GetStochast();

            Assert.AreEqual(1.8, stochast.Mean, margin);
            Assert.AreEqual(0.82, stochast.Deviation, margin);
        }

        [Test]
        public void TestLinearDoubleExecuted()
        {
            var project = ProjectBuilder.GetSensitivityProject(ProjectBuilder.GetLinearProject());

            project.SensitivityMethod = new ImportanceSamplingS();
            ((ImportanceSamplingS)project.SensitivityMethod).Settings.RandomSettings.RandomGeneratorType = RandomGeneratorType.MersenneTwister;

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

            project.SensitivityMethod = new ImportanceSamplingS();
            ((ImportanceSamplingS)project.SensitivityMethod).Settings.RandomSettings.RandomGeneratorType = RandomGeneratorType.MersenneTwister;

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

            project.SensitivityMethod = new ImportanceSamplingS();
            ((ImportanceSamplingS)project.SensitivityMethod).Settings.RandomSettings.RandomGeneratorType = RandomGeneratorType.MersenneTwister;
            ((ImportanceSamplingS)project.SensitivityMethod).Settings.MaximumSamples = 100000;

            Stochast stochast = project.GetStochast();

            Assert.AreEqual(1.8, stochast.Mean, margin);
            Assert.AreEqual(0.82, stochast.Deviation, margin);

            stochast.DistributionType = DistributionType.Uniform;

            Assert.AreEqual(-0.2, stochast.Minimum, 10 * margin);
            Assert.AreEqual(3.8, stochast.Maximum, 10 * margin);
        }

        [Test]
        public void TestLinearFullyCorrelated()
        {
            var project = ProjectBuilder.GetSensitivityProject(ProjectBuilder.GetLinearFullyCorrelatedProject());
            project.SensitivityMethod = new ImportanceSamplingS();
            ((ImportanceSamplingS)project.SensitivityMethod).Settings.RandomSettings.RandomGeneratorType = RandomGeneratorType.MersenneTwister;

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
            project.SensitivityMethod = new ImportanceSamplingS();
            ((ImportanceSamplingS)project.SensitivityMethod).Settings.RandomSettings.RandomGeneratorType = RandomGeneratorType.MersenneTwister;

            Stochast stochast = project.GetStochast();

            Assert.AreEqual(1.04, stochast.Mean, margin);
            Assert.AreEqual(0.59, stochast.Deviation, margin);
        }

        [Test]
        public void TestLinearPartialCorrelated()
        {
            var project = ProjectBuilder.GetSensitivityProject(ProjectBuilder.GetLinearPartialCorrelatedProject());
            project.SensitivityMethod = new ImportanceSamplingS();
            ((ImportanceSamplingS)project.SensitivityMethod).Settings.RandomSettings.RandomGeneratorType = RandomGeneratorType.MersenneTwister;

            Stochast stochast = project.GetStochast();

            Assert.AreEqual(1.79, stochast.Mean, margin);
            Assert.AreEqual(0.76, stochast.Deviation, margin);
        }
    }
}
