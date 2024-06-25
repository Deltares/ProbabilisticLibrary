using System.Collections.Generic;
using Deltares.Reliability.Wrappers;
using Deltares.Models.Wrappers;
using NUnit.Framework;
using System.Linq;
using Deltares.Statistics.Wrappers;

namespace Deltares.Probabilistic.Wrapper.Test
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

            DesignPointCombiner combiner = new DesignPointCombiner { CombinerType = CombinerType.DirectionalSampling };

            DesignPoint combined = combiner.Combine(CombinationType.OneFails, designPoints);

            Assert.AreEqual(StandardNormal.GetUFromQ(2 * q - q * q), combined.Beta, margin);

            Assert.AreEqual(designPoints.Count, combined.ContributingDesignPoints.Count);
            Assert.AreEqual(designPoints[0], combined.ContributingDesignPoints[0]);
        }

        [Test]
        public void TestIndependentMultipleSeries()
        {
            double q = 0.01;

            DesignPoint dp1 = ProjectBuilder.GetSimpleDesignPoint(StandardNormal.GetUFromQ(q), 2);
            DesignPoint dp2 = ProjectBuilder.GetSimpleDesignPoint(StandardNormal.GetUFromQ(q), 2);
            DesignPoint dp3 = ProjectBuilder.GetSimpleDesignPoint(StandardNormal.GetUFromQ(q), 2);

            List<DesignPoint> designPoints = new List<DesignPoint> { dp1, dp2, dp3 };

            DesignPointCombiner combiner = new DesignPointCombiner { CombinerType = CombinerType.DirectionalSampling };

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

            DesignPointCombiner combiner = new DesignPointCombiner { CombinerType = CombinerType.DirectionalSampling };

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

            DesignPointCombiner combiner = new DesignPointCombiner { CombinerType = CombinerType.DirectionalSampling };

            DesignPoint combined = combiner.Combine(CombinationType.AllFail, designPoints);

            Assert.AreEqual(StandardNormal.GetUFromQ(q * q * q), combined.Beta, margin);
        }

    }
}

