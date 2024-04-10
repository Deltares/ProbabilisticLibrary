using System;
using Deltares.Statistics.Wrappers;
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
            var stochast = new Stochast
            {
                DistributionType = DistributionType.Deterministic,
                Mean = 3,
                Deviation = 1
            };

            Assert.AreEqual(3, stochast.GetXFromU(0));
            Assert.AreEqual(3, stochast.GetXFromU(1));

            Assert.AreEqual(0, stochast.GetUFromX(3));
            Assert.AreEqual(0, stochast.GetUFromX(5));
        }

        [Test]
        public void TestNormal()
        {
            Stochast stochast = new Stochast { DistributionType = DistributionType.Normal, Mean = 3, Deviation = 1 };

            Assert.AreEqual(3, stochast.GetXFromU(0));
            Assert.AreEqual(4, stochast.GetXFromU(1));
            Assert.AreEqual(1, stochast.GetUFromX(4));

            Assert.AreEqual(0, stochast.GetUFromX(3));
            Assert.AreEqual(1, stochast.GetUFromX(4));

            TestStochast(stochast);

            // SetXAtU
            stochast.Mean = 0;
            stochast.Deviation = 1;
            stochast.SetXAtU(2, 0.05, ConstantParameterType.Deviation);
            Assert.AreEqual(stochast.GetXFromU(0.05), 2, margin);

            stochast.Mean = 0;
            stochast.Deviation = 1;
            stochast.SetXAtU(2, 0.05, ConstantParameterType.VariationCoefficient);
            Assert.AreEqual(stochast.GetXFromU(0.05), 2, margin);

            TestFit(stochast);

            stochast.Fit(new[] { 2.5, 3.5, 4.5, 5.5, 6.5 });
            Assert.AreEqual(4.5, stochast.Mean, margin);
            Assert.AreEqual(1.581, stochast.Deviation, margin);
        }


        [Test]
        public void TestNormalTruncated()
        {
            Stochast stochast = new Stochast
            {
                DistributionType = DistributionType.Normal,
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

            double u = StandardNormal.GetUFromQ(0.2);
            Assert.AreEqual(4.28155, stochast.GetXFromU(u), margin);

            double u2 = stochast.GetUFromX(4.28155);
            Assert.AreEqual(0.2, StandardNormal.GetQFromU(u2), margin);

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
        public void TestLogNormal()
        {
            Stochast stochast = new Stochast { DistributionType = DistributionType.LogNormal, Mean = 10, Deviation = 2 };

            TestSettingValuesOrder(stochast);

            TestSettingProperties(stochast);

            stochast = new Stochast { DistributionType = DistributionType.LogNormal, Mean = 10, Deviation = 2 };

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

            // SetXAtU
            stochast.Shift = 1;
            stochast.Deviation = 1;
            stochast.Mean = 10;

            TestSetXAtU(stochast, 2, 0.05);
            TestSetXAtU(stochast, 1.1, 0.05);
            TestSetXAtU(stochast, 1, 0.05, true);

            stochast.SetXAtU(0.5, 0.05, ConstantParameterType.Deviation);
            Assert.AreEqual(stochast.GetXFromU(0.05), 1, margin); // not below shift

            stochast.Shift = 1;
            stochast.Mean = 10;
            stochast.Deviation = 1;
            TestInvert(stochast, true);

            stochast.Inverted = false;
            stochast.Truncated = true;
            stochast.Minimum = 5;
            stochast.Maximum = 15;

            TestMinMax(stochast);

            TestInvert(stochast, true);

            stochast.Inverted = false;
            stochast.Truncated = false;
            stochast.Shift = 0;
            stochast.Minimum = 0;
            stochast.Maximum = 0;
            stochast.Mean = 3;
            stochast.Deviation = 1;
            TestFit(stochast);
        }

        [Test]
        public void TestUniform()
        {
            Stochast stochast = new Stochast { DistributionType = DistributionType.Uniform, Minimum = 5, Maximum = 9 };

            Assert.AreEqual(7, stochast.GetXFromU(0), margin);
            Assert.AreEqual(5.4, stochast.GetXFromP(0.1), margin);
            Assert.AreEqual(8.6, stochast.GetXFromP(0.9), margin);

            Assert.AreEqual(1.28155, stochast.GetUFromX(8.6), margin);

            TestStochast(stochast);

            TestMinMax(stochast);

            TestSetXAtU(stochast, 5.5, 0.05);

            stochast.Fit(new[] { 2.5, 3.0, 3.5, 5.5, 6.5 });
            Assert.AreEqual(1.7, stochast.Minimum, margin);
            Assert.AreEqual(7.3, stochast.Maximum, margin);

            TestFit(stochast);
        }

        [Test]
        public void TestGumbel()
        {
            Stochast stochast = new Stochast { DistributionType = DistributionType.Gumbel, Scale = 1, Shift = 0 };

            TestSettingValuesOrder(stochast);

            TestSettingPropertiesGumbel(stochast);

            stochast = new Stochast { DistributionType = DistributionType.Gumbel, Scale = 1, Shift = 0 };

            TestStochast(stochast);

            Assert.AreEqual(-1.89, stochast.GetXFromU(-3), 0.01);
            Assert.AreEqual(-1.33, stochast.GetXFromU(-2), 0.01);
            Assert.AreEqual(0.37, stochast.GetXFromU(0), 0.01);
            Assert.AreEqual(1.76, stochast.GetXFromU(1), 0.01);
            Assert.AreEqual(3.77, stochast.GetXFromU(2), 0.01);

            stochast.Scale = 2;
            stochast.Shift = 3;

            const double eulerMascheroni = 0.5772156649;
            Assert.AreEqual(3 + 2 * eulerMascheroni, stochast.Mean, 0.01);

            stochast.Mean = 5;
            Assert.AreEqual(5 - 2 * eulerMascheroni, stochast.Shift, 0.01);

            stochast.Scale = 2;
            stochast.Shift = 3;

            TestSetXAtU(stochast, 3.5, 0.05);

            TestInvert(stochast, true);

            stochast.Inverted = false;
            stochast.Truncated = true;
            stochast.Minimum = 2;
            stochast.Maximum = 10;

            TestMinMax(stochast);

            TestInvert(stochast, true);

            stochast.Inverted = false;
            stochast.Truncated = false;
            stochast.Scale = 2;
            stochast.Shift = 3;
            stochast.Minimum = 0;
            stochast.Maximum = 0;

            TestFit(stochast);
        }

        [Test]
        public void TestWeibull()
        {
            var stochast = new Stochast { DistributionType = DistributionType.Weibull, Scale = 1, Shape = 1 };

            TestStochast(stochast);

            stochast.Scale = 2;
            stochast.Shape = 3;
            stochast.Shift = 0.5;

            TestInvert(stochast, true);
            TestFit(stochast, 0.4);
        }

        [Test]
        public void TestFrechet()
        {
            var stochast = new Stochast { DistributionType = DistributionType.Frechet, Scale = 1, Shape = 1 };

            Assert.AreEqual(0.54, stochast.GetPDF(0.5), 0.01);
            Assert.AreEqual(0.37, stochast.GetPDF(1), 0.01);
            Assert.AreEqual(0.16, stochast.GetPDF(2), 0.01);

            Assert.AreEqual(0.14, stochast.GetCDF(0.5), 0.01);
            Assert.AreEqual(0.37, stochast.GetCDF(1), 0.01);
            Assert.AreEqual(0.61, stochast.GetCDF(2), 0.01);

            TestStochast(stochast);

            stochast.Scale = 2;
            stochast.Shape = 3;

            TestInvert(stochast, true);
            TestFit(stochast, 1.1);
        }

        [Test]
        public void TestGeneralizedExtremeValue()
        {
            var stochast = new Stochast { DistributionType = DistributionType.GeneralizedExtremeValue, Scale = 1, Shape = 1 };

            TestStochast(stochast);

            // gumbel

            stochast.Scale = 2;
            stochast.Shape = 0;
            stochast.Shift = 3;

            TestInvert(stochast, true);
            TestFit(stochast, 1);

            // frechet

            stochast.Scale = 2;
            stochast.Shape = 0.2;
            stochast.Shift = 0;

            TestInvert(stochast, true);
            TestFit(stochast, 5);

            // weibull

            stochast.Scale = 2;
            stochast.Shape = -0.5;
            stochast.Shift = 0;

            TestInvert(stochast, true);
            TestFit(stochast, 1);
        }

        [Test]
        public void TestRayleigh()
        {
            var stochast = new Stochast { DistributionType = DistributionType.Rayleigh, Scale = 1 };

            TestStochast(stochast);

            stochast.Scale = 1.1;

            TestInvert(stochast, true);

            TestFit(stochast);

            stochast.Shift = 2;
            stochast.Scale = 2;
            TestFit(stochast);
        }

        [Test]
        public void TestRayleighN()
        {
            var stochast = new Stochast { DistributionType = DistributionType.RayleighN, Scale = 1, Shape = 2 };

            TestStochast(stochast);

            stochast.Scale = 1.1;

            TestInvert(stochast, true);

            //TestFit(stochast);

            //stochast.Shift = 2;
            //stochast.Scale = 2;
            //TestFit(stochast);
        }

        [Test]
        public void TestDiscrete()
        {
            var stochast = new Stochast { DistributionType = DistributionType.Discrete };
            stochast.DiscreteValues.Add(new DiscreteValue(1, 10));
            stochast.DiscreteValues.Add(new DiscreteValue(2, 20));
            stochast.DiscreteValues.Add(new DiscreteValue(3, 20));

            stochast.InitializeForRun();

            Assert.AreEqual(1, stochast.GetXFromU(StandardNormal.GetUFromP(0.1)), margin);
            Assert.AreEqual(2, stochast.GetXFromU(0), margin);
            Assert.AreEqual(3, stochast.GetXFromU(StandardNormal.GetUFromP(0.9)), margin);

            Assert.AreEqual(0.2, stochast.GetPDF(1), margin);
            Assert.AreEqual(0.4, stochast.GetPDF(2), margin);
            Assert.AreEqual(0.4, stochast.GetPDF(3), margin);
            Assert.AreEqual(0, stochast.GetPDF(1.5), margin);

            Assert.AreEqual(StandardNormal.GetUFromP(0.1), stochast.GetUFromX(1), margin);
            Assert.AreEqual(StandardNormal.GetUFromP(0.4), stochast.GetUFromX(2), margin);
            Assert.AreEqual(StandardNormal.GetUFromP(0.6), stochast.GetUFromX(2.5), margin);
            Assert.AreEqual(StandardNormal.GetUFromP(0.8), stochast.GetUFromX(3), margin);
            Assert.AreEqual(StandardNormal.BetaMax, stochast.GetUFromX(4), margin);

            Assert.AreEqual(0, stochast.GetCDF(0), margin);
            Assert.AreEqual(0.1, stochast.GetCDF(1), margin);
            Assert.AreEqual(0.4, stochast.GetCDF(2), margin);
            Assert.AreEqual(0.6, stochast.GetCDF(2.5), margin);
            Assert.AreEqual(0.8, stochast.GetCDF(3), margin);
            Assert.AreEqual(1, stochast.GetCDF(4), margin);

            Assert.IsTrue(stochast.IsVarying());
        }

        [Test]
        public void TestVariableStochast()
        {
            Stochast realizedStochast = new Stochast { DistributionType = DistributionType.Uniform, Minimum = 0, Maximum = 1 };
            realizedStochast.IsVariableStochast = true;

            realizedStochast.ValueSet.StochastValues.Add(new VariableStochastValue { X = 0, Minimum = 0, Maximum = 1 });
            realizedStochast.ValueSet.StochastValues.Add(new VariableStochastValue { X = 2, Minimum = 11, Maximum = 12 });
            realizedStochast.ValueSet.StochastValues.Add(new VariableStochastValue { X = 1, Minimum = 1, Maximum = 2 });

            realizedStochast.InitializeForRun();

            // exact value
            Assert.AreEqual(0.5, realizedStochast.GetXFromUAndSource(0, 0), margin);
            Assert.AreEqual(1.5, realizedStochast.GetXFromUAndSource(1, 0), margin);
            Assert.AreEqual(11.5, realizedStochast.GetXFromUAndSource(2, 0), margin);

            // interpolated
            Assert.AreEqual(1, realizedStochast.GetXFromUAndSource(0.5, 0), margin);
            Assert.AreEqual(6.5, realizedStochast.GetXFromUAndSource(1.5, 0), margin);

            // extrapolated
            Assert.AreEqual(0.5, realizedStochast.GetXFromUAndSource(-1, 0), margin);
            Assert.AreEqual(11.5, realizedStochast.GetXFromUAndSource(3, 0), margin);
        }

        private void TestStochast(Stochast stochast, double delta = 0.01)
        {
            foreach (var u in new[] { -7, -6, -5, -4, -3, -2, -1.5, -1, -0.5, -0.1, 0, 0.1, 0.5, 1, 1.5, 2, 3, 4, 5, 6, 7 })
            {
                double x = stochast.GetXFromU(u);
                double uu = stochast.GetUFromX(x);

                Assert.AreEqual(u, uu, delta);
            }
        }

        private static void TestSettingValuesOrder(Stochast stochast)
        {
            Stochast newStochast = new Stochast { DistributionType = stochast.DistributionType, Mean = stochast.Mean, Deviation = stochast.Deviation };
            Assert.AreEqual(stochast.Mean, newStochast.Mean, margin);
            Assert.AreEqual(stochast.Deviation, newStochast.Deviation, margin);

            newStochast = new Stochast { Mean = stochast.Mean, Deviation = stochast.Deviation };
            newStochast.SetDistributionType(stochast.DistributionType);
            Assert.AreEqual(stochast.Mean, newStochast.Mean, margin);
            Assert.AreEqual(stochast.Deviation, newStochast.Deviation, margin);
        }

        private static void TestSettingProperties(Stochast stochast)
        {
            stochast.Mean = 3;
            stochast.Deviation = 1.5;

            Assert.AreEqual(stochast.Mean, 3, margin);

            stochast.Mean = -3;
            stochast.SetShift(-5);

            Assert.AreEqual(stochast.Mean, -3, margin);

            stochast.Inverted = true;
            stochast.Mean = -1;
            stochast.SetShift(2);

            Assert.AreEqual(stochast.Mean, -1, margin);
        }

        private static void TestSettingPropertiesGumbel(Stochast stochast)
        {
            stochast.Mean = 3;
            stochast.Deviation = 1.5;

            Assert.AreEqual(stochast.Mean, 3, margin);

            stochast.Deviation = -1;
            stochast.Mean = -3;
            stochast.Deviation = 1;

            Assert.AreEqual(stochast.Mean, -3, margin);
        }


        private void TestMinMax(Stochast stochast)
        {
            Assert.AreEqual(-StandardNormal.UMax, stochast.GetUFromX(stochast.Minimum - 1), 0.01);
            Assert.AreEqual(StandardNormal.UMax, stochast.GetUFromX(stochast.Maximum + 1), 0.01);
        }

        private void TestSetXAtU(Stochast stochast, double x, double u, bool deviationZero = false)
        {
            const double delta = 0.025;

            double deviation = deviationZero ? 0 : stochast.Deviation;

            stochast.SetXAtU(x, u, ConstantParameterType.Deviation);

            Assert.AreEqual(x, stochast.GetXFromU(u), delta);
            Assert.AreEqual(deviation, stochast.Deviation, delta);

            double modifiedU = u < 0.5 ? 2 * u : 1 - 2 * (1 - u);

            double vc = stochast.Deviation / stochast.Mean;

            stochast.SetXAtU(x, modifiedU, ConstantParameterType.VariationCoefficient);

            Assert.AreEqual(x, stochast.GetXFromU(modifiedU), delta);
            Assert.AreEqual(vc, stochast.Deviation / stochast.Mean, delta);
        }

        private void TestInvert(Stochast stochast, bool useShift, double delta = 0.01)
        {
            double center = stochast.Shift;

            Assert.AreEqual(false, stochast.Inverted);

            // test mean inverted
            stochast.Inverted = true;
            InvertMinMax(stochast, useShift);
            double meanInverted = stochast.Mean;

            stochast.Inverted = false;
            InvertMinMax(stochast, useShift);
            double mean = stochast.Mean;

            Assert.AreEqual(mean, 2 * center - meanInverted, delta);

            foreach (var u in new[] { -7, -6, -5, -4, -3, -2, -1.5, -1, -0.5, -0.1, 0, 0.1, 0.5, 1, 1.5, 2, 3, 4, 5, 6, 7 })
            {
                stochast.Inverted = true;
                InvertMinMax(stochast, useShift);
                double xInverted = stochast.GetXFromU(-u);

                stochast.Inverted = false;
                InvertMinMax(stochast, useShift);
                double x = stochast.GetXFromU(u);

                Assert.AreEqual(x, 2 * center - xInverted, delta);

                double uu = stochast.GetUFromX(x);
                Assert.AreEqual(u, uu, delta);
            }

            // reset for further tests

            if (stochast.Inverted)
            {
                stochast.Inverted = false;
                InvertMinMax(stochast, useShift);
            }

            Assert.AreEqual(false, stochast.Inverted);
        }

        private void InvertMinMax(Stochast stochast, bool useShift)
        {
            double center = useShift ? stochast.Shift : 0;

            double minimum = stochast.Minimum;
            double maximum = stochast.Maximum;

            stochast.Minimum = 2 * center - maximum;
            stochast.Maximum = 2 * center - minimum;
        }

        private void TestFit(Stochast stochast, double fitMargin = defaultFitMargin, int number = 1000)
        {
            Random random = new Random(0);

            Stochast fittedStochast = new Stochast();
            fittedStochast.DistributionType = stochast.DistributionType;

            double[] values = new double[number];

            for (int i = 0; i < number; i++)
            {
                double p = random.NextDouble();
                double u = StandardNormal.GetUFromP(p);
                values[i] = stochast.GetXFromU(u);
            }

            fittedStochast.Fit(values);

            Assert.AreEqual(stochast.Location, fittedStochast.Location, fitMargin);
            Assert.AreEqual(stochast.Scale, fittedStochast.Scale, fitMargin);
            Assert.AreEqual(stochast.Shift, fittedStochast.Shift, fitMargin);
            Assert.AreEqual(stochast.ShiftB, fittedStochast.ShiftB, fitMargin);
            Assert.AreEqual(stochast.Minimum, fittedStochast.Minimum, fitMargin);
            Assert.AreEqual(stochast.Maximum, fittedStochast.Maximum, fitMargin);
            Assert.AreEqual(stochast.Shape, fittedStochast.Shape, fitMargin);
            Assert.AreEqual(stochast.ShapeB, fittedStochast.ShapeB, fitMargin);
        }
    }
}