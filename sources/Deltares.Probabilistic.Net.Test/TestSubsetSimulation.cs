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
    public class TestSubsetSimulation
    {
        private const double margin = 0.01;

        [Test]
        public void TestLinear()
        {
            var project = ProjectBuilder.GetLinearProject();

            project.Settings.ReliabilityMethod = ReliabilityMethod.SubsetSimulation;
            project.Settings.MarkovChainDeviation = 0.5;
            project.Settings.VariationCoefficient = 0;

            project.Run();

            DesignPoint designPoint = project.DesignPoint;

            ClassicAssert.AreEqual(2.57, designPoint.Beta, margin);
            ClassicAssert.AreEqual(0.005, designPoint.ProbabilityFailure, margin / 10);
            ClassicAssert.AreEqual(0.995, designPoint.ProbabilityNonFailure, margin / 10);

            ClassicAssert.AreEqual(-0.70, designPoint.Alphas[0].Alpha, margin);
            ClassicAssert.AreEqual(-0.71, designPoint.Alphas[1].Alpha, margin);

            ClassicAssert.AreEqual(0.93, designPoint.Alphas[0].X, margin);
            ClassicAssert.AreEqual(0.93, designPoint.Alphas[1].X, margin);
        }

        [Test]
        public void TestLinearAdaptiveConditional()
        {
            var project = ProjectBuilder.GetLinearProject();

            project.Settings.ReliabilityMethod = ReliabilityMethod.SubsetSimulation;
            project.Settings.MarkovChainDeviation = 0.5;
            project.Settings.VariationCoefficient = 0;
            project.Settings.SampleMethod = SampleMethod.AdaptiveConditional;

            project.Run();

            DesignPoint designPoint = project.DesignPoint;

            ClassicAssert.AreEqual(2.57, designPoint.Beta, margin);
            ClassicAssert.AreEqual(0.005, designPoint.ProbabilityFailure, margin / 10);
            ClassicAssert.AreEqual(0.995, designPoint.ProbabilityNonFailure, margin / 10);

            ClassicAssert.AreEqual(-0.71, designPoint.Alphas[0].Alpha, margin);
            ClassicAssert.AreEqual(-0.71, designPoint.Alphas[1].Alpha, margin);

            ClassicAssert.AreEqual(0.93, designPoint.Alphas[0].X, margin);
            ClassicAssert.AreEqual(0.93, designPoint.Alphas[1].X, margin);
        }

        [Test]
        public void TestInverseLinear()
        {
            var project = ProjectBuilder.GetInverseLinearProject();

            project.Settings.ReliabilityMethod = ReliabilityMethod.SubsetSimulation;
            project.Settings.MarkovChainDeviation = 0.5;
            project.Settings.VariationCoefficient = 0;

            project.Run();

            DesignPoint designPoint = project.DesignPoint;

            ClassicAssert.AreEqual(-2.57, designPoint.Beta, margin);

            ClassicAssert.AreEqual(0.69, designPoint.Alphas[0].Alpha, margin);
            ClassicAssert.AreEqual(0.73, designPoint.Alphas[1].Alpha, margin);

            ClassicAssert.AreEqual(0.92, designPoint.Alphas[0].X, margin);
            ClassicAssert.AreEqual(0.94, designPoint.Alphas[1].X, margin);
        }

        [Test]
        public void TestLinearSmallMarkovChain()
        {
            var project = ProjectBuilder.GetLinearSmallProject();

            project.Settings.ReliabilityMethod = ReliabilityMethod.SubsetSimulation;
            project.Settings.MarkovChainDeviation = 0.5;
            project.Settings.VariationCoefficient = 0;
            project.Settings.MinimumSamples = 500;
            project.Settings.MaximumSamples = 5000;
            project.Settings.SampleMethod = SampleMethod.MarkovChain;

            project.Run();

            ClassicAssert.AreEqual(3.80, project.DesignPoint.Beta, margin);
        }

        [Test]
        public void TestLinearSmallAdaptiveConditional()
        {
            var project = ProjectBuilder.GetLinearSmallProject();

            project.Settings.ReliabilityMethod = ReliabilityMethod.SubsetSimulation;
            project.Settings.MinimumSamples = 500;
            project.Settings.MaximumSamples = 5000;
            project.Settings.SampleMethod = SampleMethod.AdaptiveConditional;

            project.Run();

            ClassicAssert.AreEqual(3.88, project.DesignPoint.Beta, margin);
        }

        [Test]
        public void TestManyVars()
        {
            var project = ProjectBuilder.GetManyVarsProject();

            project.Settings.ReliabilityMethod = ReliabilityMethod.SubsetSimulation;
            project.Settings.MarkovChainDeviation = 0.5;
            project.Settings.SampleMethod = SampleMethod.AdaptiveConditional;

            project.Run();

            ClassicAssert.AreEqual(3.13, project.DesignPoint.Beta, margin);
        }

        [Test]
        public void TestNonLinear()
        {
            var project = ProjectBuilder.GetNonLinearProject();

            project.Settings.ReliabilityMethod = ReliabilityMethod.SubsetSimulation;

            project.Run();

            ClassicAssert.AreEqual(3.41, project.DesignPoint.Beta, margin);
        }
    }
}
