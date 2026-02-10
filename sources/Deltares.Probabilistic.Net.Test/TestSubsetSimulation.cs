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
﻿using Deltares.Reliability.Wrappers;
using NUnit.Framework;
using NUnit.Framework.Legacy;

namespace Deltares.Probabilistic.Wrapper.Test
{
    [TestFixture]
    public class TestSubsetSimulation
    {
        private const double margin = 0.01;

        [Test]
        public void TestLinear()
        {
            var project = ProjectBuilder.GetLinearProject();

            SubsetSimulation subsetSimulation = new SubsetSimulation();
            subsetSimulation.Settings.MarkovChainDeviation = 0.5;
            subsetSimulation.Settings.VariationCoefficientFailure = 0;

            project.ReliabilityMethod = subsetSimulation;

            DesignPoint designPoint = project.GetDesignPoint();

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

            SubsetSimulation subsetSimulation = new SubsetSimulation();
            subsetSimulation.Settings.MarkovChainDeviation = 0.5;
            subsetSimulation.Settings.VariationCoefficientFailure = 0;
            subsetSimulation.Settings.SampleMethod = SampleMethod.AdaptiveConditional;

            project.ReliabilityMethod = subsetSimulation;

            DesignPoint designPoint = project.GetDesignPoint();

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

            SubsetSimulation subsetSimulation = new SubsetSimulation();
            subsetSimulation.Settings.MarkovChainDeviation = 0.5;
            subsetSimulation.Settings.VariationCoefficientFailure = 0;

            project.ReliabilityMethod = subsetSimulation;

            DesignPoint designPoint = project.GetDesignPoint();

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

            SubsetSimulation subsetSimulation = new SubsetSimulation();
            subsetSimulation.Settings.MinimumSamples = 500;
            subsetSimulation.Settings.MaximumSamples = 5000;
            subsetSimulation.Settings.VariationCoefficientFailure = 0;
            subsetSimulation.Settings.MarkovChainDeviation = 0.5;
            subsetSimulation.Settings.SampleMethod = SampleMethod.MarkovChain;

            project.ReliabilityMethod = subsetSimulation;

            DesignPoint designPoint = project.GetDesignPoint();

            ClassicAssert.AreEqual(3.80, designPoint.Beta, margin);
        }

        [Test]
        public void TestLinearSmallAdaptiveConditional()
        {
            var project = ProjectBuilder.GetLinearSmallProject();

            SubsetSimulation subsetSimulation = new SubsetSimulation();
            subsetSimulation.Settings.MinimumSamples = 500;
            subsetSimulation.Settings.MaximumSamples = 5000;
            subsetSimulation.Settings.SampleMethod = SampleMethod.AdaptiveConditional;

            project.ReliabilityMethod = subsetSimulation;

            DesignPoint designPoint = project.GetDesignPoint();

            ClassicAssert.AreEqual(3.88, designPoint.Beta, margin);

        }

        [Test]
        public void TestManyVars()
        {
            var project = ProjectBuilder.GetManyVarsProject();

            SubsetSimulation subsetSimulation = new SubsetSimulation();
            subsetSimulation.Settings.SampleMethod = SampleMethod.AdaptiveConditional;
            subsetSimulation.Settings.MarkovChainDeviation = 0.5;

            project.ReliabilityMethod = subsetSimulation;

            DesignPoint designPoint = project.GetDesignPoint();

            ClassicAssert.AreEqual(3.13, designPoint.Beta, margin);
        }

        [Test]
        public void TestNonLinear()
        {
            var project = ProjectBuilder.GetNonLinearProject();

            project.ReliabilityMethod = new SubsetSimulation();

            DesignPoint designPoint = project.GetDesignPoint();

            ClassicAssert.AreEqual(3.41, designPoint.Beta, margin);
        }
    }
}
