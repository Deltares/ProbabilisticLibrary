using System;
using System.Collections.Generic;
using Deltares.Probabilistic.Kernels;
using NUnit.Framework;

namespace Deltares.Probabilistics.Wrappers.Test
{
    [TestFixture]
    public class TestDistributions
    {
        private const double margin = 0.001;
        private const double defaultFitMargin = 0.1;


        [Test]
        public void TestDeterministic()
        {
            var stochast = new StochastWrapper
            {
                DistributionType = WrapperDistributionType.Deterministic,
                Mean = 3,
                Deviation = 1
            };

            Assert.AreEqual(3, stochast.GetXFromU(0));
            Assert.AreEqual(3, stochast.GetXFromU(1));

            Assert.AreEqual(0, stochast.GetUFromX(3));
            Assert.AreEqual(0, stochast.GetUFromX(5));
        }


        [Test]
        public void TestGumbel()
        {
            StochastWrapper stochast = new StochastWrapper { DistributionType = WrapperDistributionType.Gumbel, Scale = 1, Shift = 0 };

            TestStochast(stochast);

            Assert.AreEqual(-1.89, stochast.GetXFromU(-3), 0.01);
            Assert.AreEqual(-1.33, stochast.GetXFromU(-2), 0.01);
            Assert.AreEqual(0.37, stochast.GetXFromU(0), 0.01);
            Assert.AreEqual(1.76, stochast.GetXFromU(1), 0.01);
            Assert.AreEqual(3.77, stochast.GetXFromU(2), 0.01);

            stochast.Scale = 2;
            stochast.Shift = 3;

            //TestInvert(stochast);

            const double eulerMascheroni = 0.5772156649;
            Assert.AreEqual(3 + 2 * eulerMascheroni, stochast.Mean, 0.01);

            stochast.Mean = 5;
            Assert.AreEqual(5 - 2 * eulerMascheroni, stochast.Shift, 0.01);

            TestInvert(stochast, true);

            stochast.Inverted = false;
            stochast.Truncated = true;
            stochast.Minimum = 2;
            stochast.Maximum = 10;

            TestMinMax(stochast);

            TestInvert(stochast, true);
        }

        [Test]
        public void TestLogNormal()
        {
            StochastWrapper stochast = new StochastWrapper { DistributionType = WrapperDistributionType.LogNormal, Mean = 10, Deviation = 2 };

            // 1.04 = 1 + (2/10)^2
            double expected = Math.Exp(Math.Log(10) - 0.5 * Math.Log(1.04));
            Assert.AreEqual(expected, stochast.GetXFromU(0), margin);

            Assert.AreEqual(0, stochast.GetUFromX(expected));

            // Test derived values
            Assert.AreEqual(0.198, stochast.Scale, 0.01);
            Assert.AreEqual(2.283, stochast.Location, 0.01);

            // modify and set back
            stochast.Scale = 2;
            stochast.Location = 2;

            stochast.Scale = 0.198;
            stochast.Location = 2.283;

            Assert.AreEqual(10, stochast.Mean, 0.01);
            Assert.AreEqual(2, stochast.Deviation, 0.01);

            TestStochast(stochast);

            TestInvert(stochast, true);

            stochast.Inverted = false;
            stochast.Truncated = true;
            stochast.Minimum = 5;
            stochast.Maximum = 15;

            TestMinMax(stochast);

            TestInvert(stochast, true);
        }

        [Test]
        public void TestNormal()
        {
            StochastWrapper stochast = new StochastWrapper { DistributionType = WrapperDistributionType.Normal, Mean = 3, Deviation = 1 };

            Assert.AreEqual(3, stochast.GetXFromU(0));
            Assert.AreEqual(4, stochast.GetXFromU(1));
            Assert.AreEqual(1, stochast.GetUFromX(4));

            Assert.AreEqual(0, stochast.GetUFromX(3));
            Assert.AreEqual(1, stochast.GetUFromX(4));

            TestStochast(stochast);
        }


        [Test]
        public void TestNormalTruncated()
        {
            StochastWrapper stochast = new StochastWrapper
            {
                DistributionType = WrapperDistributionType.Normal,
                Truncated = true,
                Mean = 3,
                Deviation = 1,
                Minimum = 0,
                Maximum = 6
            };

            // Test bounded stochast
            Assert.AreEqual(3, stochast.GetXFromU(0), margin);
            Assert.AreEqual(6, stochast.GetXFromU(10000), margin);
            Assert.AreEqual(0, stochast.GetXFromU(-10000), margin);

            TestMinMax(stochast);

            // Test with no boundaries
            stochast.Minimum = Double.NegativeInfinity;
            stochast.Maximum = Double.PositiveInfinity;

            Assert.AreEqual(4, stochast.GetXFromU(1), margin);
            Assert.AreEqual(1, stochast.GetUFromX(4), margin);

            // Test one sided boundary
            stochast.Minimum = stochast.Mean;
            stochast.Maximum = Double.PositiveInfinity;

            double u = StandardNormalWrapper.GetUFromQ(0.2);
            Assert.AreEqual(4.28155, stochast.GetXFromU(u), margin);

            double u2 = stochast.GetUFromX(4.28155);
            Assert.AreEqual(0.2, StandardNormalWrapper.GetQFromU(u2), margin);

            // Test with one huge boundary
            stochast.Minimum = 0;
            stochast.Maximum = 30;

            Assert.AreEqual(0, stochast.GetXFromU(-1000), margin);
            Assert.AreEqual(30, stochast.GetXFromU(1000), margin);

            stochast.Minimum = -30;
            stochast.Maximum = 0;

            Assert.AreEqual(-30, stochast.GetXFromU(-1000), margin);
            Assert.AreEqual(0, stochast.GetXFromU(1000), margin);
        }

        [Test]
        public void TestUniform()
        {
            StochastWrapper stochast = new StochastWrapper { DistributionType = WrapperDistributionType.Uniform, Minimum = 5, Maximum = 9 };

            Assert.AreEqual(7, stochast.GetXFromU(0), margin);
            Assert.AreEqual(5.4, stochast.GetXFromP(0.1), margin);
            Assert.AreEqual(8.6, stochast.GetXFromP(0.9), margin);

            Assert.AreEqual(1.28155, stochast.GetUFromX(8.6), margin);

            TestStochast(stochast);

            TestMinMax(stochast);
        }

        [Test]
        public void TestDiscrete()
        {
            var stochast = new StochastWrapper { DistributionType = WrapperDistributionType.Discrete };
            stochast.DiscreteValues.Add(new DiscreteValueWrapper(1, 10));
            stochast.DiscreteValues.Add(new DiscreteValueWrapper(2, 20));
            stochast.DiscreteValues.Add(new DiscreteValueWrapper(3, 20));

            stochast.InitializeForRun();

            Assert.AreEqual(1, stochast.GetXFromU(StandardNormalWrapper.GetUFromP(0.1)), margin);
            Assert.AreEqual(2, stochast.GetXFromU(0), margin);
            Assert.AreEqual(3, stochast.GetXFromU(StandardNormalWrapper.GetUFromP(0.9)), margin);

            //Assert.AreEqual(0.2, stochast.GetPDF(1), margin);
            //Assert.AreEqual(0.4, stochast.GetPDF(2), margin);
            //Assert.AreEqual(0.4, stochast.GetPDF(3), margin);
            //Assert.AreEqual(0, stochast.GetPDF(1.5), margin);

            Assert.AreEqual(StandardNormalWrapper.GetUFromP(0.1), stochast.GetUFromX(1), margin);
            Assert.AreEqual(StandardNormalWrapper.GetUFromP(0.4), stochast.GetUFromX(2), margin);
            Assert.AreEqual(StandardNormalWrapper.GetUFromP(0.6), stochast.GetUFromX(2.5), margin);
            Assert.AreEqual(StandardNormalWrapper.GetUFromP(0.8), stochast.GetUFromX(3), margin);
            Assert.AreEqual(StandardNormalWrapper.BetaMax, stochast.GetUFromX(4), margin);
        }

        private void TestStochast(StochastWrapper stochast, double delta = 0.01)
        {
            foreach (var u in new[] { -7, -6, -5, -4, -3, -2, -1.5, -1, -0.5, -0.1, 0, 0.1, 0.5, 1, 1.5, 2, 3, 4, 5, 6, 7 })
            {
                double x = stochast.GetXFromU(u);
                double uu = stochast.GetUFromX(x);

                Assert.AreEqual(u, uu, delta);
            }
        }

        private void TestMinMax(StochastWrapper stochast)
        {
            Assert.AreEqual(-StandardNormalWrapper.UMax, stochast.GetUFromX(stochast.Minimum - 1), 0.01);
            Assert.AreEqual(StandardNormalWrapper.UMax, stochast.GetUFromX(stochast.Maximum + 1), 0.01);
        }

        private void TestInvert(StochastWrapper stochast, bool useShift, double delta = 0.01)
        {
            double center = stochast.Shift;

            // test mean inverted
            stochast.Inverted = true;
            double meanInverted = stochast.Mean;

            stochast.Inverted = false;
            double mean = stochast.Mean;

            Assert.AreEqual(mean, 2 * center - meanInverted, delta);

            foreach (var u in new[] { -7, -6, -5, -4, -3, -2, -1.5, -1, -0.5, -0.1, 0, 0.1, 0.5, 1, 1.5, 2, 3, 4, 5, 6, 7 })
            {
                stochast.Inverted = true;
                double xInverted = stochast.GetXFromU(-u);

                stochast.Inverted = false;
                double x = stochast.GetXFromU(u);

                Assert.AreEqual(x, 2 * center - xInverted, delta);

                double uu = stochast.GetUFromX(x);
                Assert.AreEqual(u, uu, delta);
            }

            // reset for further tests
            stochast.Inverted = false;
        }
    }
}