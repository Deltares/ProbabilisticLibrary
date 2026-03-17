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
﻿using Deltares.Probabilistic.Model;
using Deltares.Probabilistic.Reliability;
using Deltares.Probabilistic.Statistics;
using NUnit.Framework.Legacy;
using System.Linq;
using NUnit.Framework;

namespace Deltares.Probabilistic.Test
{
    [TestFixture]
    public class TestAdaptiveImportanceSampling
    {
        private const double margin = 0.01;

        [Test]
        public void TestLinear()
        {
            var project = ProjectBuilder.GetLinearProject();

            project.Settings.ReliabilityMethod = ReliabilityMethod.AdaptiveImportanceSampling;
            project.Run();

            DesignPoint designPoint = project.DesignPoint;

            ClassicAssert.AreEqual(2.60, designPoint.Beta, margin);

            ClassicAssert.AreEqual(0.93, designPoint.Alphas[0].X, margin);
            ClassicAssert.AreEqual(0.93, designPoint.Alphas[1].X, margin);
        }

        [Test]
        public void TestLinearSmall()
        {
            var project = ProjectBuilder.GetLinearSmallProject();

            project.Settings.ReliabilityMethod = ReliabilityMethod.AdaptiveImportanceSampling;
            project.Settings.MaximumVarianceLoops = 5;
            project.Settings.MinimumVarianceLoops = 2;
            project.Settings.MaximumSamples = 10000;
            project.Run();

            DesignPoint designPoint = project.DesignPoint;

            ClassicAssert.AreEqual(3.90, designPoint.Beta, margin);
        }

        [Test]
        public void TestLinearSmallSlow()
        {
            ReliabilityProject project = ProjectBuilder.GetLinearSmallSlowProject();

            project.Settings.ReliabilityMethod = ReliabilityMethod.AdaptiveImportanceSampling;
            project.Settings.MaximumVarianceLoops = 5;
            project.Settings.MinimumVarianceLoops = 2;

            project.Settings.MaximumSamplesNoResult = 100;
            project.Settings.MinimumSamples = 100;
            project.Settings.MaximumSamples = 200;

            project.Run();

            ClassicAssert.AreEqual(3.91, project.DesignPoint.Beta, margin);
        }

        [Test]
        public void TestLinearSmallSlowParallel()
        {
            ReliabilityProject project = ProjectBuilder.GetLinearSmallSlowProject();

            project.Settings.MaxParallelProcesses = 4;

            project.Settings.ReliabilityMethod = ReliabilityMethod.AdaptiveImportanceSampling;
            project.Settings.MaximumVarianceLoops = 5;
            project.Settings.MinimumVarianceLoops = 2;
            project.Settings.MaximumSamplesNoResult = 100;
            project.Settings.MinimumSamples = 100;
            project.Settings.MaximumSamples = 200;

            project.Run();

            ClassicAssert.AreEqual(3.91, project.DesignPoint.Beta, margin);
        }

        [Test]
        public void TestLinearClusters()
        {
            ReliabilityProject project = ProjectBuilder.GetLinearAbsoluteSmallProject();

            project.Settings.ReliabilityMethod = ReliabilityMethod.AdaptiveImportanceSampling;
            project.Settings.MaximumVarianceLoops = 5;
            project.Settings.MinimumVarianceLoops = 2;

            project.Settings.Clustering = true;
            project.Settings.MaxClusters = 5;
            project.Settings.OptimizeNumberClusters = false;

            project.Settings.MinimumSamples = 1000;
            project.Settings.MaximumSamples = 5000;

            project.Run();

            ClassicAssert.AreEqual(3.72, project.DesignPoint.Beta, margin);
        }

        [Test]
        public void TestEdgeClusters()
        {
            ReliabilityProject project = ProjectBuilder.GetEdgeProject();

            project.Settings.ReliabilityMethod = ReliabilityMethod.AdaptiveImportanceSampling;
            project.Settings.MaximumVarianceLoops = 5;
            project.Settings.MinimumVarianceLoops = 2;

            project.Settings.Clustering = true;
            project.Settings.MaxClusters = 5;
            project.Settings.OptimizeNumberClusters = true;

            project.Settings.MinimumSamples = 1000;
            project.Settings.MaximumSamples = 5000;

            project.Run();

            double pSingle = 0.0001;
            double betaExpected = StandardNormal.GetUFromQ(3 * pSingle - 3 * pSingle * pSingle);
            ClassicAssert.AreEqual(betaExpected, project.DesignPoint.Beta, 2 * margin);

            ClassicAssert.AreEqual(3, project.DesignPoint.ContributingDesignPoints.Count(p => p.Identifier.StartsWith("Cluster")));
        }
    }
}
