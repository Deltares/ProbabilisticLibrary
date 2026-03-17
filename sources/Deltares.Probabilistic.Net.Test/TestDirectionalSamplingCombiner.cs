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
﻿using System.Collections.Generic;
using Deltares.Probabilistic.Model;
using Deltares.Probabilistic.Statistics;
using Deltares.Probabilistic.Reliability;
using NUnit.Framework;
using NUnit.Framework.Legacy;
using System.Linq;
using System.Runtime.Intrinsics.Arm;

namespace Deltares.Probabilistic.Test
{
    [TestFixture]
    public class TestDirectionalSamplingCombiner
    {
        private const double margin = 0.05;

        [Test]
        public void TestIndependentSeries()
        {
            double q = 0.01;

            DesignPoint dp1 = ProjectBuilder.GetSimpleDesignPoint(StandardNormal.GetUFromQ(q), 2);
            DesignPoint dp2 = ProjectBuilder.GetSimpleDesignPoint(StandardNormal.GetUFromQ(q), 2);

            List<DesignPoint> designPoints = new List<DesignPoint> { dp1, dp2 };

            CombineProject project = new CombineProject();
            project.Settings.CombineMethod = CombineMethod.DirectionaSampling;
            project.Settings.CombineType = CombineType.Series;

            project.DesignPoints.Add(dp1);
            project.DesignPoints.Add(dp2);

            project.Run();

            DesignPoint combined = project.DesignPoint;

            ClassicAssert.AreEqual(StandardNormal.GetUFromQ(2 * q - q * q), combined.Beta, margin);

            ClassicAssert.AreEqual(designPoints.Count, combined.ContributingDesignPoints.Count);
            ClassicAssert.AreEqual(designPoints[0], combined.ContributingDesignPoints[0]);
        }

        [Test]
        public void TestIndependentMultipleSeries()
        {
            double q = 0.01;

            DesignPoint dp1 = ProjectBuilder.GetSimpleDesignPoint(StandardNormal.GetUFromQ(q), 2);
            DesignPoint dp2 = ProjectBuilder.GetSimpleDesignPoint(StandardNormal.GetUFromQ(q), 2);
            DesignPoint dp3 = ProjectBuilder.GetSimpleDesignPoint(StandardNormal.GetUFromQ(q), 2);

            CombineProject project = new CombineProject();
            project.Settings.CombineMethod = CombineMethod.DirectionaSampling;
            project.Settings.CombineType = CombineType.Series;

            project.DesignPoints.Add(dp1);
            project.DesignPoints.Add(dp2);
            project.DesignPoints.Add(dp3);

            project.Run();

            ClassicAssert.AreEqual(StandardNormal.GetUFromQ(3*q - 3*q*q - q*q*q), project.DesignPoint.Beta, margin);
        }

        [Test]
        public void TestIndependentParallel()
        {
            double q = 0.01;

            DesignPoint dp1 = ProjectBuilder.GetSimpleDesignPoint(StandardNormal.GetUFromQ(q), 2);
            DesignPoint dp2 = ProjectBuilder.GetSimpleDesignPoint(StandardNormal.GetUFromQ(q), 2);

            CombineProject project = new CombineProject();
            project.Settings.CombineMethod = CombineMethod.DirectionaSampling;
            project.Settings.CombineType = CombineType.Parallel;

            project.DesignPoints.Add(dp1);
            project.DesignPoints.Add(dp2);

            project.Run();

            ClassicAssert.AreEqual(StandardNormal.GetUFromQ(q * q), project.DesignPoint.Beta, margin);
        }

        [Test]
        public void TestIndependentMultipleParallel()
        {
            double q = 0.01;

            DesignPoint dp1 = ProjectBuilder.GetSimpleDesignPoint(StandardNormal.GetUFromQ(q), 2);
            DesignPoint dp2 = ProjectBuilder.GetSimpleDesignPoint(StandardNormal.GetUFromQ(q), 2);
            DesignPoint dp3 = ProjectBuilder.GetSimpleDesignPoint(StandardNormal.GetUFromQ(q), 2);

            CombineProject project = new CombineProject();
            project.Settings.CombineMethod = CombineMethod.DirectionaSampling;
            project.Settings.CombineType = CombineType.Parallel;

            project.DesignPoints.Add(dp1);
            project.DesignPoints.Add(dp2);
            project.DesignPoints.Add(dp3);

            project.Run();

            ClassicAssert.AreEqual(StandardNormal.GetUFromQ(q * q * q), project.DesignPoint.Beta, margin);
        }

    }
}

