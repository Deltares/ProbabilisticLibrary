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
    public class TestImportanceSampling
    {
        private const double margin = 0.01;

        [Test]
        public void TestLinear()
        {
            var project = ProjectBuilder.GetLinearProject();

            project.Settings.ReliabilityMethod = ReliabilityMethod.ImportanceSampling;
            project.Run();

            DesignPoint designPoint = project.DesignPoint;

            ClassicAssert.AreEqual(2.58, designPoint.Beta, margin);

            ClassicAssert.AreEqual(0.005, designPoint.ProbabilityFailure, margin / 10);
            ClassicAssert.AreEqual(0.995, designPoint.ProbabilityNonFailure, margin / 10);
            ClassicAssert.AreEqual(203.3, designPoint.ReturnPeriod, margin * 10);

            ClassicAssert.AreEqual(-0.69, designPoint.Alphas[0].Alpha, margin);
            ClassicAssert.AreEqual(-0.716, designPoint.Alphas[1].Alpha, margin);

            ClassicAssert.AreEqual(0.929, designPoint.Alphas[0].X, margin);
            ClassicAssert.AreEqual(0.935, designPoint.Alphas[1].X, margin);
        }

        [Test]
        public void TestInverseLinear()
        {
            var project = ProjectBuilder.GetInverseLinearProject();

            project.Settings.ReliabilityMethod = ReliabilityMethod.ImportanceSampling;
            project.Run();

            DesignPoint designPoint = project.DesignPoint;

            ClassicAssert.AreEqual(-2.58, designPoint.Beta, margin);

            ClassicAssert.AreEqual(0.69, designPoint.Alphas[0].Alpha, margin);
            ClassicAssert.AreEqual(0.72, designPoint.Alphas[1].Alpha, margin);

            ClassicAssert.AreEqual(0.93, designPoint.Alphas[0].X, margin);
            ClassicAssert.AreEqual(0.93, designPoint.Alphas[1].X, margin);

            project.Settings.ReliabilityMethod = ReliabilityMethod.DirectionReliability;
            project.Settings.SetStartPoint(designPoint);
            project.Run();

            DesignPoint limitStatePoint = project.DesignPoint;

            ModelSample sample = limitStatePoint.GetModelSample();
            project.ZFunction.Invoke(sample);
            ClassicAssert.AreEqual(0, sample.Z, margin);

            ClassicAssert.AreEqual(0.90, limitStatePoint.Alphas[0].X, margin);
            ClassicAssert.AreEqual(0.90, limitStatePoint.Alphas[1].X, margin);
        }

        [Test]
        public void TestLinearMessages()
        {
            ReliabilityProject project = ProjectBuilder.GetLinearProject();

            project.Settings.ReliabilityMethod = ReliabilityMethod.ImportanceSampling;
            project.Settings.SaveRealizations = true;
            project.Settings.SaveConvergence = true;
            project.Run();

            DesignPoint designPoint = project.DesignPoint;

            ClassicAssert.AreEqual(2.58, designPoint.Beta, margin);

            ClassicAssert.AreEqual(1565, designPoint.ReliabilityResults.Count);
            ClassicAssert.AreEqual(10001, designPoint.Realizations.Count);

            ClassicAssert.IsFalse(double.IsNaN(designPoint.Realizations[1].Weight));
        }

        [Test]
        public void TestLinearSmall()
        {
            ReliabilityProject project = ProjectBuilder.GetLinearSmallProject();

            project.Settings.ReliabilityMethod = ReliabilityMethod.ImportanceSampling;
            project.Settings.MaximumSamples = 10000;
            project.Settings.MinimumSamples = 5000;

            for (int i = 0; i < project.Stochasts.Count; i++)
            {
                project.Settings.StochastSettings.Add(new StochastSettings { StartValue = 0, VarianceFactor = 2, Stochast = project.Stochasts[i] });
            }

            project.Run();

            ClassicAssert.AreEqual(3.81, project.DesignPoint.Beta, margin);
        }

        [Test]
        public void TestLinearCorrelated()
        {
            ReliabilityProject project = ProjectBuilder.GetLinearFullyCorrelatedProject();

            project.Settings.ReliabilityMethod = ReliabilityMethod.ImportanceSampling;
            project.Run();

            ClassicAssert.AreEqual(2.55, project.DesignPoint.Beta, margin);
        }

        [Test]
        public void TestBligh()
        {
            ReliabilityProject project = ProjectBuilder.GetBlighProject();

            project.Settings.ReliabilityMethod = ReliabilityMethod.ImportanceSampling;
            project.Run();

            ClassicAssert.AreEqual(1.70, project.DesignPoint.Beta, margin);
        }

        [Test]
        public void TestLoadStrength()
        {
            ReliabilityProject project = ProjectBuilder.GetLoadStrengthProject();

            project.Settings.ReliabilityMethod = ReliabilityMethod.ImportanceSampling;
            project.Run();

            ClassicAssert.AreEqual(1.395, project.DesignPoint.Beta, margin);
        }

        [Test]
        public void TestLoadStrengthSurvived()
        {
            ReliabilityProject project = ProjectBuilder.GetLoadStrengthSurvivedProject();

            project.Settings.ReliabilityMethod = ReliabilityMethod.ImportanceSampling;
            project.Run();

            ClassicAssert.AreEqual(1.75, project.DesignPoint.Beta, margin);
        }

        [Test]
        public void TestConvex()
        {
            ReliabilityProject project = ProjectBuilder.GetConvexProject();

            project.Settings.ReliabilityMethod = ReliabilityMethod.ImportanceSampling;
            project.Run();

            ClassicAssert.AreEqual(2.62, project.DesignPoint.Beta, margin);
        }

        [Test]
        public void TestNoisy()
        {
            ReliabilityProject project = ProjectBuilder.GetNoisyProject();

            project.Settings.ReliabilityMethod = ReliabilityMethod.ImportanceSampling;
            project.Run();

            ClassicAssert.AreEqual(2.47, project.DesignPoint.Beta, margin);
        }
    }
}
