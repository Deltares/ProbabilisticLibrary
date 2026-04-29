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
using Deltares.Probabilistic.Model;
using Deltares.Probabilistic.Reliability;
using NUnit.Framework;
using NUnit.Framework.Legacy;

namespace Deltares.Probabilistic.Test
{
    [TestFixture]
    public class TestDirectionalSampling
    {
        private const double margin = 0.01;

        [Test]
        public void TestLinear()
        {
            ReliabilityProject project = ProjectBuilder.GetLinearProject();

            project.Settings.ReliabilityMethod = ReliabilityMethod.DirectionalSampling;
            project.Run();

            DesignPoint designPoint = project.DesignPoint;

            ClassicAssert.AreEqual(2.56, designPoint.Beta, margin);
        }

        [Test]
        public void TestLinearDoubleExecuted()
        {
            var project = ProjectBuilder.GetLinearProject();

            project.Settings.ReliabilityMethod = ReliabilityMethod.DirectionalSampling;
            project.Run();

            DesignPoint designPoint = project.DesignPoint;

            ClassicAssert.AreEqual(2.56, designPoint.Beta, margin);

            project.Run();
            DesignPoint designPoint2 = project.DesignPoint;

            ClassicAssert.AreEqual(2.56, designPoint2.Beta, margin);

            ClassicAssert.AreNotSame(designPoint, designPoint2);
            ClassicAssert.AreNotSame(designPoint.Alphas[0], designPoint2.Alphas[0]);
            ClassicAssert.AreSame(designPoint.Alphas[0].Parameter, designPoint2.Alphas[0].Parameter);
        }

        [Test]
        public void TestInverseLinear()
        {
            var project = ProjectBuilder.GetInverseLinearProject();

            project.Settings.ReliabilityMethod = ReliabilityMethod.DirectionalSampling;
            project.Run();

            DesignPoint designPoint = project.DesignPoint;

            ClassicAssert.AreEqual(-2.56, designPoint.Beta, margin);
        }

        [Test]
        public void TestLinearParallel()
        {
            ReliabilityProject project = ProjectBuilder.GetLinearProject();

            project.Settings.ReliabilityMethod = ReliabilityMethod.DirectionalSampling;
            project.Settings.MaxParallelProcesses = 4;
            project.Run();

            DesignPoint designPoint = project.DesignPoint;

            ClassicAssert.AreEqual(2.56, designPoint.Beta, margin);
        }

        [Test]
        public void TestDoubleLinear()
        {
            ReliabilityProject project = ProjectBuilder.GetDoubleLinearProject();

            project.Settings.ReliabilityMethod = ReliabilityMethod.DirectionalSampling;
            project.Settings.MaximumDirections = 2000;
            project.Settings.MinimumDirections = 2000;
            project.Settings.ModelVaryingType = ModelVaryingType.Varying;
            project.Run();

            DesignPoint designPoint = project.DesignPoint;

            ClassicAssert.AreEqual(2.20, designPoint.Beta, margin);
        }

        [Test]
        public void TestNonLinear()
        {
            ReliabilityProject project = ProjectBuilder.GetNonLinearProject();

            project.Settings.ReliabilityMethod = ReliabilityMethod.DirectionalSampling;
            project.Run();

            DesignPoint designPoint = project.DesignPoint;

            ClassicAssert.AreEqual(3.57, designPoint.Beta, margin);
        }

        [Test]
        public void TestBlock()
        {
            ReliabilityProject project = ProjectBuilder.GetBlockProject();

            project.Settings.ReliabilityMethod = ReliabilityMethod.DirectionalSampling;
            project.Run();

            DesignPoint designPoint = project.DesignPoint;

            ClassicAssert.AreEqual(3.57, designPoint.Beta, margin);
        }

        [Test]
        public void TestSemiBlock()
        {
            ReliabilityProject project = ProjectBuilder.GetSemiBlockProject();

            project.Settings.ReliabilityMethod = ReliabilityMethod.DirectionalSampling;
            project.Run();

            DesignPoint designPoint = project.DesignPoint;

            ClassicAssert.AreEqual(3.57, designPoint.Beta, margin);
        }

        [Test]
        public void TestNoisy()
        {
            ReliabilityProject project = ProjectBuilder.GetNoisyProject();

            project.Settings.ReliabilityMethod = ReliabilityMethod.DirectionalSampling;
            project.Run();

            DesignPoint designPoint = project.DesignPoint;

            ClassicAssert.AreEqual(2.47, designPoint.Beta, margin);
        }

        [Test]
        public void TestSeries()
        {
            var project = ProjectBuilder.GetSeriesProject();

            project.Settings.ReliabilityMethod = ReliabilityMethod.DirectionalSampling;
            project.Run();

            DesignPoint designPoint = project.DesignPoint;

            ClassicAssert.AreEqual(2.89, designPoint.Beta, margin);
        }

        [Test]
        public void TestManyVars()
        {
            ReliabilityProject project = ProjectBuilder.GetManyVarsProject();

            project.Settings.ReliabilityMethod = ReliabilityMethod.DirectionalSampling;
            project.Run();

            DesignPoint designPoint = project.DesignPoint;

            ClassicAssert.AreEqual(3.04, designPoint.Beta, margin);
        }

        [Test]
        public void TestSaddle()
        {
            ReliabilityProject project = ProjectBuilder.GetSaddleProject();

            project.Settings.ReliabilityMethod = ReliabilityMethod.DirectionalSampling;
            project.Run();

            DesignPoint designPoint = project.DesignPoint;

            ClassicAssert.AreEqual(2.34, designPoint.Beta, margin);
        }

        [Test]
        public void TestTwoBranches()
        {
            ReliabilityProject project = ProjectBuilder.GetTwoBranchesProject();

            project.Settings.ReliabilityMethod = ReliabilityMethod.DirectionalSampling;
            project.Run();

            DesignPoint designPoint = project.DesignPoint;

            ClassicAssert.AreEqual(5.04, designPoint.Beta, margin);
        }

        [Test] 
        public void TestBligh()
        {
            var project = ProjectBuilder.GetBlighProject();

            project.Settings.ReliabilityMethod = ReliabilityMethod.DirectionalSampling;
            project.Run();

            DesignPoint designPoint = project.DesignPoint;

            ClassicAssert.AreEqual(1.68, designPoint.Beta, margin);
            ClassicAssert.AreEqual(project.Stochasts.Count, designPoint.Alphas.Count, margin);
        }
    }
}
