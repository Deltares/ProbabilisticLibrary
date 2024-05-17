using System.Collections.Generic;
using Deltares.Reliability.Wrappers;
using Deltares.Models.Wrappers;
using NUnit.Framework;
using System.Linq;
using Deltares.Statistics.Wrappers;

namespace Deltares.Probabilistics.Wrappers.Test
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

            Assert.AreEqual(StandardNormal.GetUFromQ(2 * q - q * q), combined.Beta, margin);
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

            Assert.AreEqual(StandardNormal.GetUFromQ(3*q - 3*q*q - q*q*q), combined.Beta, margin);
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

            Assert.AreEqual(StandardNormal.GetUFromQ(q * q), combined.Beta, margin);
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

            Assert.AreEqual(StandardNormal.GetUFromQ(q * q * q), combined.Beta, margin);
        }

    }
}

