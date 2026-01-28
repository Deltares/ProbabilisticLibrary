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
using Deltares.Reliability.Wrappers;
using NUnit.Framework;
using NUnit.Framework.Legacy;
using Deltares.Statistics.Wrappers;

namespace Deltares.Probabilistic.Wrapper.Test
{
    [TestFixture]
    public class TestImportanceSamplingCombiner
    {
        private const double margin = 0.01;

        [Test]
        public void TestIndependentSeries()
        {
            double q = 0.01;

            DesignPoint dp1 = ProjectBuilder.GetSimpleDesignPoint(StandardNormal.GetUFromQ(q), 2);
            DesignPoint dp2 = ProjectBuilder.GetSimpleDesignPoint(StandardNormal.GetUFromQ(q), 2);

            List<DesignPoint> designPoints = new List<DesignPoint> { dp1, dp2 };

            DesignPointCombiner combiner = new DesignPointCombiner { CombinerType = CombinerType.ImportanceSampling };

            DesignPoint combined = combiner.Combine(CombinationType.OneFails, designPoints);

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

            List<DesignPoint> designPoints = new List<DesignPoint> { dp1, dp2, dp3 };

            DesignPointCombiner combiner = new DesignPointCombiner { CombinerType = CombinerType.ImportanceSampling };

            DesignPoint combined = combiner.Combine(CombinationType.OneFails, designPoints);

            ClassicAssert.AreEqual(StandardNormal.GetUFromQ(3*q - 3*q*q - q*q*q), combined.Beta, margin);
        }

        [Test]
        public void TestIndependentParallel()
        {
            double q = 0.01;

            DesignPoint dp1 = ProjectBuilder.GetSimpleDesignPoint(StandardNormal.GetUFromQ(q), 2);
            DesignPoint dp2 = ProjectBuilder.GetSimpleDesignPoint(StandardNormal.GetUFromQ(q), 2);

            List<DesignPoint> designPoints = new List<DesignPoint> { dp1, dp2 };

            DesignPointCombiner combiner = new DesignPointCombiner { CombinerType = CombinerType.ImportanceSampling };

            DesignPoint combined = combiner.Combine(CombinationType.AllFail, designPoints);

            ClassicAssert.AreEqual(StandardNormal.GetUFromQ(q * q), combined.Beta, margin);
        }

        [Test]
        public void TestIndependentMultipleParallel()
        {
            double q = 0.01;

            DesignPoint dp1 = ProjectBuilder.GetSimpleDesignPoint(StandardNormal.GetUFromQ(q), 2);
            DesignPoint dp2 = ProjectBuilder.GetSimpleDesignPoint(StandardNormal.GetUFromQ(q), 2);
            DesignPoint dp3 = ProjectBuilder.GetSimpleDesignPoint(StandardNormal.GetUFromQ(q), 2);

            List<DesignPoint> designPoints = new List<DesignPoint> { dp1, dp2, dp3 };

            DesignPointCombiner combiner = new DesignPointCombiner { CombinerType = CombinerType.ImportanceSampling };

            DesignPoint combined = combiner.Combine(CombinationType.AllFail, designPoints);

            ClassicAssert.AreEqual(StandardNormal.GetUFromQ(q * q * q), combined.Beta, margin);
        }

    }
}

