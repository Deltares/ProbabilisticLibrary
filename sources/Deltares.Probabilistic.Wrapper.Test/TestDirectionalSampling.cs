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
    public class TestDirectionalSampling
    {
        private const double margin = 0.01;

        [Test]
        public void TestLinear()
        {
            Project project = ProjectBuilder.GetLinearProject();

            project.ReliabilityMethod = new DirectionalSampling();
            DesignPoint designPoint = project.GetDesignPoint();

            ClassicAssert.AreEqual(2.56, designPoint.Beta, margin);
        }

        [Test]
        public void TestLinearDoubleExecuted()
        {
            var project = ProjectBuilder.GetLinearProject();

            project.ReliabilityMethod = new DirectionalSampling();
            DesignPoint designPoint = project.GetDesignPoint();

            ClassicAssert.AreEqual(2.56, designPoint.Beta, margin);

            DesignPoint designPoint2 = project.GetDesignPoint();

            ClassicAssert.AreEqual(2.56, designPoint2.Beta, margin);

            ClassicAssert.AreNotSame(designPoint, designPoint2);
            ClassicAssert.AreNotSame(designPoint.Alphas[0], designPoint2.Alphas[0]);
            ClassicAssert.AreSame(designPoint.Alphas[0].Parameter, designPoint2.Alphas[0].Parameter);
        }

        [Test]
        public void TestInverseLinear()
        {
            var project = ProjectBuilder.GetInverseLinearProject();

            project.ReliabilityMethod = new DirectionalSampling();
            DesignPoint designPoint = project.GetDesignPoint();

            ClassicAssert.AreEqual(-2.56, designPoint.Beta, margin);
        }

        [Test]
        public void TestLinearParallel()
        {
            Project project = ProjectBuilder.GetLinearProject();

            project.Settings.MaxParallelProcesses = 4;

            project.ReliabilityMethod = new DirectionalSampling();

            DesignPoint designPoint = project.GetDesignPoint();

            ClassicAssert.AreEqual(2.56, designPoint.Beta, margin);
        }

        [Test]
        public void TestDoubleLinear()
        {
            Project project = ProjectBuilder.GetDoubleLinearProject();

            DirectionalSampling directionalSampling = new DirectionalSampling();
            directionalSampling.Settings.MaximumSamples = 2000;
            directionalSampling.Settings.MinimumSamples = 2000;
            directionalSampling.Settings.DirectionalSettings.ModelVaryingType = VaryingType.Varying;

            project.ReliabilityMethod = directionalSampling;

            DesignPoint designPoint = project.GetDesignPoint();

            ClassicAssert.AreEqual(2.20, designPoint.Beta, margin);
        }

        [Test]
        public void TestNonLinear()
        {
            Project project = ProjectBuilder.GetNonLinearProject();

            project.ReliabilityMethod = new DirectionalSampling();

            DesignPoint designPoint = project.GetDesignPoint();

            ClassicAssert.AreEqual(3.57, designPoint.Beta, margin);
        }

        [Test]
        public void TestBlock()
        {
            Project project = ProjectBuilder.GetBlockProject();

            project.ReliabilityMethod = new DirectionalSampling();

            DesignPoint designPoint = project.GetDesignPoint();

            ClassicAssert.AreEqual(3.57, designPoint.Beta, margin);
        }

        [Test]
        public void TestSemiBlock()
        {
            Project project = ProjectBuilder.GetSemiBlockProject();

            project.ReliabilityMethod = new DirectionalSampling();

            DesignPoint designPoint = project.GetDesignPoint();

            ClassicAssert.AreEqual(3.57, designPoint.Beta, margin);
        }

        [Test]
        public void TestNoisy()
        {
            Project project = ProjectBuilder.GetNoisyProject();

            project.ReliabilityMethod = new DirectionalSampling();

            DesignPoint designPoint = project.GetDesignPoint();

            ClassicAssert.AreEqual(2.47, designPoint.Beta, margin);
        }

        [Test]
        public void TestSeries()
        {
            var project = ProjectBuilder.GetSeriesProject();

            project.ReliabilityMethod = new DirectionalSampling();

            DesignPoint designPoint = project.GetDesignPoint();

            ClassicAssert.AreEqual(2.89, designPoint.Beta, margin);
        }

        [Test]
        public void TestManyVars()
        {
            Project project = ProjectBuilder.GetManyVarsProject();

            project.ReliabilityMethod = new DirectionalSampling();

            DesignPoint designPoint = project.GetDesignPoint();

            ClassicAssert.AreEqual(3.04, designPoint.Beta, margin);
        }

        [Test]
        public void TestSaddle()
        {
            Project project = ProjectBuilder.GetSaddleProject();

            project.ReliabilityMethod = new DirectionalSampling();

            DesignPoint designPoint = project.GetDesignPoint();

            ClassicAssert.AreEqual(2.34, designPoint.Beta, margin);
        }

        [Test]
        public void TestTwoBranches()
        {
            Project project = ProjectBuilder.GetTwoBranchesProject();

            project.ReliabilityMethod = new DirectionalSampling();

            DesignPoint designPoint = project.GetDesignPoint();

            ClassicAssert.AreEqual(5.04, designPoint.Beta, margin);
        }

        [Test] 
        public void TestBligh()
        {
            var project = ProjectBuilder.GetBlighProject();

            project.ReliabilityMethod = new DirectionalSampling();

            DesignPoint designPoint = project.GetDesignPoint();

            ClassicAssert.AreEqual(1.68, designPoint.Beta, margin);
            ClassicAssert.AreEqual(project.Stochasts.Count, designPoint.Alphas.Count, margin);
        }
    }
}
