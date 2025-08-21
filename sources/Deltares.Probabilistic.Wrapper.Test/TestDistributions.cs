
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
using System;
using System.Collections.Generic;
using Deltares.Statistics.Wrappers;
using NUnit.Framework;
using NUnit.Framework.Legacy;

namespace Deltares.Probabilistic.Wrapper.Test
{
    [TestFixture]
    public class TestDistributions
    {
        private const double margin = 0.001;
        private const double defaultFitMargin = 0.1;


        [Test]
        public void TestStochastName()
        {
            var stochast = new Stochast();

            foreach (string name in new[] { "simple", "with space", "and Γρεεκ symbols", "Ϣ" })
            {
                stochast.Name = name;
                ClassicAssert.AreEqual(name, stochast.Name);
            }
        }

        [Test]
        public void TestDeterministic()
        {
            var stochast = new Stochast
            {
                DistributionType = DistributionType.Deterministic,
                Mean = 3,
                Deviation = 1
            };

            ClassicAssert.AreEqual(3, stochast.GetXFromU(0));
            ClassicAssert.AreEqual(3, stochast.GetXFromU(1));

            ClassicAssert.AreEqual(0, stochast.GetUFromX(3));
            ClassicAssert.AreEqual(0, stochast.GetUFromX(5));
        }

        [Test]
        public void TestNormal()
        {
            Stochast stochast = new Stochast { DistributionType = DistributionType.Normal, Mean = 3, Deviation = 1 };

            ClassicAssert.AreEqual(3, stochast.GetXFromU(0));
            ClassicAssert.AreEqual(4, stochast.GetXFromU(1));
            ClassicAssert.AreEqual(1, stochast.GetUFromX(4));

            ClassicAssert.AreEqual(0, stochast.GetUFromX(3));
            ClassicAssert.AreEqual(1, stochast.GetUFromX(4));

            TestStochast(stochast);

            // SetXAtU
            stochast.Mean = 0;
            stochast.Deviation = 1;
            stochast.SetXAtU(2, 0.05, ConstantParameterType.Deviation);
            ClassicAssert.AreEqual(stochast.GetXFromU(0.05), 2, margin);

            stochast.Mean = 2;
            stochast.Deviation = 1;
            stochast.SetXAtU(3, 0.05, ConstantParameterType.VariationCoefficient);
            ClassicAssert.AreEqual(stochast.GetXFromU(0.05), 3, margin);

            stochast.Mean = -2;
            stochast.Deviation = 1;
            stochast.SetXAtU(-1, 0.05, ConstantParameterType.VariationCoefficient);
            ClassicAssert.AreEqual(stochast.GetXFromU(0.05), -1, margin);

            stochast.Mean = 0;
            stochast.Deviation = 1;
            stochast.SetXAtU(2, 0.05, ConstantParameterType.VariationCoefficient);
            ClassicAssert.AreEqual(stochast.GetXFromU(0.05), 2, margin);

            TestFit(stochast);

            stochast.Fit(new[] { 2.5, 3.5, 4.5, 5.5, 6.5 });
            ClassicAssert.AreEqual(4.5, stochast.Mean, margin);
            ClassicAssert.AreEqual(1.581, stochast.Deviation, margin);

            Stochast prior = new Stochast { DistributionType = DistributionType.Normal, Mean = 3, Deviation = 1 };
            stochast.FitPrior(prior, new[] { 2.5, 3.5, 4.5, 5.5, 6.5 });
            ClassicAssert.AreEqual(4.0, stochast.Mean, margin);
            ClassicAssert.AreEqual(0.578, stochast.Deviation, margin);
        }

        [Test]
        public void TestNormalTruncated()
        {
            Stochast stochast = new Stochast
            {
                DistributionType = DistributionType.Normal,
                Truncated = true,
                Mean = 3,
                Deviation = 1
            };

            ClassicAssert.AreEqual(double.NegativeInfinity, stochast.Minimum);
            ClassicAssert.AreEqual(double.PositiveInfinity, stochast.Maximum);

            stochast.Minimum = 0;
            stochast.Maximum = 6;

            // Test bounded stochast
            ClassicAssert.AreEqual(3, stochast.GetXFromU(0), margin);
            ClassicAssert.AreEqual(6, stochast.GetXFromU(10000), margin);
            ClassicAssert.AreEqual(0, stochast.GetXFromU(-10000), margin);

            TestMinMax(stochast);

            // Test with no boundaries
            stochast.Minimum = Double.NegativeInfinity;
            stochast.Maximum = Double.PositiveInfinity;

            ClassicAssert.AreEqual(4, stochast.GetXFromU(1), margin);
            ClassicAssert.AreEqual(1, stochast.GetUFromX(4), margin);

            // Test one sided boundary
            stochast.Minimum = stochast.Mean;
            stochast.Maximum = Double.PositiveInfinity;

            double u = StandardNormal.GetUFromQ(0.2);
            ClassicAssert.AreEqual(4.28155, stochast.GetXFromU(u), margin);

            double u2 = stochast.GetUFromX(4.28155);
            ClassicAssert.AreEqual(0.2, StandardNormal.GetQFromU(u2), margin);

            // Test with one huge boundary
            stochast.Minimum = 0;
            stochast.Maximum = 30;

            ClassicAssert.AreEqual(0, stochast.GetXFromU(-1000), margin);
            ClassicAssert.AreEqual(30, stochast.GetXFromU(1000), margin);

            stochast.Minimum = -30;
            stochast.Maximum = 0;

            ClassicAssert.AreEqual(-30, stochast.GetXFromU(-1000), margin);
            ClassicAssert.AreEqual(0, stochast.GetXFromU(1000), margin);
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
            ClassicAssert.AreEqual(expected, stochast.GetXFromU(0), margin);

            ClassicAssert.AreEqual(0, stochast.GetUFromX(expected));

            // Test derived values
            ClassicAssert.AreEqual(0.198, stochast.Scale, 0.01);
            ClassicAssert.AreEqual(2.283, stochast.Location, 0.01);

            // modify and set back
            stochast.Scale = 2;
            stochast.Location = 2;

            stochast.Scale = 0.198;
            stochast.Location = 2.283;

            ClassicAssert.AreEqual(10, stochast.Mean, 0.01);
            ClassicAssert.AreEqual(2, stochast.Deviation, 0.01);

            TestStochast(stochast);

            // SetXAtU
            stochast.Shift = 1;
            stochast.Deviation = 1;
            stochast.Mean = 10;

            TestSetXAtU(stochast, 2, 0.05);
            TestSetXAtU(stochast, 1.1, 0.05);
            TestSetXAtU(stochast, 1, 0.05, true);

            stochast.SetXAtU(0.5, 0.05, ConstantParameterType.Deviation);
            ClassicAssert.AreEqual(stochast.GetXFromU(0.05), 1, margin); // not below shift

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
            stochast.Minimum = double.NegativeInfinity;
            stochast.Maximum = double.PositiveInfinity;
            stochast.Mean = 3;
            stochast.Deviation = 1;
            TestFit(stochast);

            Stochast prior = new Stochast { DistributionType = DistributionType.LogNormal, Mean = 3, Deviation = 1 };
            stochast.FitPrior(prior, new[] { 2.5, 3.5, 4.5, 5.5, 6.5 });
            ClassicAssert.AreEqual(3.955, stochast.Mean, margin);
            ClassicAssert.AreEqual(0.596, stochast.Deviation, margin);
        }

        [Test]
        public void TestStudentT()
        {
            var stochast = new Stochast();
            var normalStochast = new Stochast();

            ClassicAssert.AreEqual(2, stochast.Observations);

            stochast.DistributionType = DistributionType.StudentT;
            stochast.Location = 3;
            stochast.Scale = 2;
            stochast.Observations = 5;

            normalStochast.DistributionType = DistributionType.Normal;
            normalStochast.Mean = 3;
            normalStochast.Deviation = 2;

            ClassicAssert.AreEqual(3, stochast.GetXFromU(0));
            ClassicAssert.AreEqual(3 - 2 * 1.533, stochast.GetXFromP(0.1), margin);
            ClassicAssert.AreEqual(3 + 2 * 1.533, stochast.GetXFromP(0.9), margin);

            // interpolation
            stochast.Observations = 91;
            ClassicAssert.AreEqual(3 - 2 * 1.2925, stochast.GetXFromP(0.1), margin);
            ClassicAssert.AreEqual(3 + 2 * 1.2925, stochast.GetXFromP(0.9), margin);

            // interpolation
            stochast.Observations = 5;
            ClassicAssert.AreEqual(3 - 2.48, stochast.GetXFromP(0.15), margin);
            ClassicAssert.AreEqual(3 + 2.48, stochast.GetXFromP(0.85), margin);
            ClassicAssert.AreEqual(3 - 17.062, stochast.GetXFromP(0.0001), margin);
            ClassicAssert.AreEqual(3 + 17.062, stochast.GetXFromP(0.9999), margin);

            // mean adapted when x modified at p
            stochast.Observations = 10;
            double p90 = stochast.GetXFromP(0.9);
            stochast.SetXAtU(p90 + 1, StandardNormal.GetUFromP(0.9), ConstantParameterType.Deviation);
            ClassicAssert.AreEqual(4, stochast.Mean, margin);

            stochast.Observations = 1;
            ClassicAssert.IsTrue(double.IsNaN(stochast.GetXFromP(0.1)));

            stochast.Observations = 3;

            stochast.Observations = 10;
            TestFit(stochast, 1.0, stochast.Observations);

            stochast.Observations = 100;
            TestFit(stochast, 0.5, stochast.Observations);
        }

        [Test]
        public void TestUniform()
        {
            Stochast stochast = new Stochast { DistributionType = DistributionType.Uniform, Minimum = 5, Maximum = 9 };

            ClassicAssert.AreEqual(7, stochast.GetXFromU(0), margin);
            ClassicAssert.AreEqual(5.4, stochast.GetXFromP(0.1), margin);
            ClassicAssert.AreEqual(8.6, stochast.GetXFromP(0.9), margin);

            ClassicAssert.AreEqual(1.28155, stochast.GetUFromX(8.6), margin);

            TestStochast(stochast);

            TestMinMax(stochast);

            TestSetXAtU(stochast, 5.5, 0.05);

            stochast.Fit(new[] { 2.5, 3.0, 3.5, 5.5, 6.5 });
            ClassicAssert.AreEqual(1.7, stochast.Minimum, margin);
            ClassicAssert.AreEqual(7.3, stochast.Maximum, margin);

            TestFit(stochast);
        }

        [Test]
        public void TestExponential()
        {
            var stochast = new Stochast
            {
                DistributionType = DistributionType.Exponential,
                Mean = 1,
                Deviation = 0.1,
                Scale = 1.0 / 3.0
            };

            TestSettingValuesOrder(stochast);
            TestStochast(stochast);
            TestInvert(stochast, true);
            TestFit(stochast);

            stochast.Fit(new[] { 2.5, 3.0, 3.5, 5.5, 6.5 });
            ClassicAssert.AreEqual(2.5, stochast.Scale, margin);
        }

        [Test]
        public void TestTriangular()
        {
            var stochast = new Stochast
            {
                DistributionType = DistributionType.Triangular,
                Shift = 5,
                Minimum = 4,
                Maximum = 8
            };

            ClassicAssert.AreEqual(5.55, stochast.GetXFromU(0), margin);

            TestStochast(stochast);

            TestFit(stochast);

            stochast.Fit(new[] { 2.5, 3.0, 3.5, 5.5, 6.5 });
            ClassicAssert.AreEqual(1.7, stochast.Minimum, margin);
            ClassicAssert.AreEqual(7.3, stochast.Maximum, margin);
            ClassicAssert.AreEqual(3.6, stochast.Shift, margin);
        }

        [Test]
        public void TestTrapezoidal()
        {
            var stochast = new Stochast
            {
                DistributionType = DistributionType.Trapezoidal,
                Shift = 5,
                ShiftB = 6,
                Minimum = 4,
                Maximum = 8
            };

            ClassicAssert.AreEqual(5.75, stochast.GetXFromU(0), margin);

            TestStochast(stochast);
            TestFit(stochast);


            Stochast triangle = new Stochast
            {
                DistributionType = DistributionType.Trapezoidal,
                Shift = 5,
                ShiftB = 5,
                Minimum = 4,
                Maximum = 8
            };

            TestStochast(triangle);
            TestFit(triangle);

            Stochast uniform = new Stochast
            {
                DistributionType = DistributionType.Trapezoidal,
                Shift = 4,
                ShiftB = 8,
                Minimum = 4,
                Maximum = 8
            };

            TestStochast(uniform);
            TestFit(uniform);

            Stochast left = new Stochast
            {
                DistributionType = DistributionType.Trapezoidal,
                Shift = 4,
                ShiftB = 4,
                Minimum = 4,
                Maximum = 8
            };

            TestStochast(left);
            TestFit(left, 0.25, 10000);

            Stochast right = new Stochast
            {
                DistributionType = DistributionType.Trapezoidal,
                Shift = 8,
                ShiftB = 8,
                Minimum = 4,
                Maximum = 8
            };

            TestStochast(right);
            TestFit(right, 0.25, 10000);

            Stochast uneven = new Stochast
            {
                DistributionType = DistributionType.Trapezoidal,
                Shift = 7,
                ShiftB = 9,
                Minimum = 0,
                Maximum = 10
            };

            TestStochast(uneven);
            TestFit(uneven, 0.25, 10000);
        }

        [Test]
        public void TestGumbel()
        {
            Stochast stochast = new Stochast { DistributionType = DistributionType.Gumbel, Scale = 1, Shift = 0 };

            TestSettingValuesOrder(stochast);

            TestSettingPropertiesGumbel(stochast);

            stochast = new Stochast { DistributionType = DistributionType.Gumbel, Scale = 1, Shift = 0 };

            TestStochast(stochast);

            ClassicAssert.AreEqual(-1.89, stochast.GetXFromU(-3), 0.01);
            ClassicAssert.AreEqual(-1.33, stochast.GetXFromU(-2), 0.01);
            ClassicAssert.AreEqual(0.37, stochast.GetXFromU(0), 0.01);
            ClassicAssert.AreEqual(1.76, stochast.GetXFromU(1), 0.01);
            ClassicAssert.AreEqual(3.77, stochast.GetXFromU(2), 0.01);

            stochast.Scale = 2;
            stochast.Shift = 3;

            const double eulerMascheroni = 0.5772156649;
            ClassicAssert.AreEqual(3 + 2 * eulerMascheroni, stochast.Mean, 0.01);

            stochast.Mean = 5;
            ClassicAssert.AreEqual(5 - 2 * eulerMascheroni, stochast.Shift, 0.01);

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
            stochast.Minimum = double.NegativeInfinity;
            stochast.Maximum = double.PositiveInfinity;

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
        public void TestConditionalWeibull()
        {
            var stochast = new Stochast { DistributionType = DistributionType.ConditionalWeibull, Scale = 1, Shape = 1, Shift = 1 };

            var expectedValues = new List<double>
            {
                0.389679340615967,
                1.36651292058166,
                2.75588794089519,
                4.77169994533666,
                7.60705089255662,
                11.3600856506974,
                16.0649982505082,
                21.7367689494396,
                28.3843074961794
            };

            for (int i = -1; i < 8; i++)
            {
                double u = (double)i;
                double x = stochast.GetXFromU(u);
                ClassicAssert.AreEqual(x, expectedValues[i + 1], 1e-9);
            }
        }

        [Test]
        public void TestFrechet()
        {
            var stochast = new Stochast { DistributionType = DistributionType.Frechet, Scale = 1, Shape = 1 };

            ClassicAssert.AreEqual(0.54, stochast.GetPDF(0.5), 0.01);
            ClassicAssert.AreEqual(0.37, stochast.GetPDF(1), 0.01);
            ClassicAssert.AreEqual(0.16, stochast.GetPDF(2), 0.01);

            ClassicAssert.AreEqual(0.14, stochast.GetCDF(0.5), 0.01);
            ClassicAssert.AreEqual(0.37, stochast.GetCDF(1), 0.01);
            ClassicAssert.AreEqual(0.61, stochast.GetCDF(2), 0.01);

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

            TestFit(stochast);
        }

        [Test]
        public void TestPareto()
        {
            var stochast = new Stochast { DistributionType = DistributionType.Pareto, Scale = 1, Shape = 1 };

            TestStochast(stochast);

            stochast.Scale = 2;
            stochast.Shape = 3;

            TestFit(stochast);
        }

        [Test]
        public void TestGeneralizedPareto()
        {
            var stochast = new Stochast
            {
                DistributionType = DistributionType.GeneralizedPareto,
                Shape = -1,
                Scale = 1,
                Shift = 30
            };

            TestStochast(stochast);

            stochast.Shape = 1;
            stochast.Scale = 5;
            stochast.Shift = 20;

            TestStochast(stochast);
        }

        [Test]
        public void TestBeta()
        {
            var stochast = new Stochast { DistributionType = DistributionType.Beta, Shape = 1, ShapeB = 1 };

            ClassicAssert.AreEqual(0.1, stochast.GetCDF(0.1), margin);

            TestStochast(stochast);
            TestFit(stochast);

            stochast.Shape = 3;
            stochast.ShapeB = 4;

            ClassicAssert.AreEqual(0.656, stochast.GetCDF(0.5), margin);

            TestFit(stochast, 0.15);
        }

        [Test]
        public void TestGamma()
        {
            var stochast = new Stochast { DistributionType = DistributionType.Gamma, Scale = 1, Shape = 1 };

            ClassicAssert.AreEqual(0.0951, stochast.GetCDF(0.1), margin);

            TestStochast(stochast);

            stochast.Scale = 2;
            stochast.Shape = 3;

            TestInvert(stochast, false);
            TestFit(stochast, 0.15);
        }

        [Test]
        public void TestBernoulli()
        {
            var stochast = new Stochast { DistributionType = DistributionType.Bernoulli, Location = 0.8 };

            ClassicAssert.AreEqual(0, stochast.GetXFromU(StandardNormal.GetUFromP(0.1)), margin);
            ClassicAssert.AreEqual(1, stochast.GetXFromU(StandardNormal.GetUFromP(0.9)), margin);

            ClassicAssert.IsTrue(stochast.IsVarying());

            stochast.Fit(new double[] { 0, 0, 1, 0, 1 });
            ClassicAssert.AreEqual(0.4, stochast.Mean, margin);
            ClassicAssert.AreEqual(Math.Sqrt(0.4 * 0.6), stochast.Deviation, margin);

            Stochast prior = new Stochast { DistributionType = DistributionType.Bernoulli, Mean = 0.8 };
            prior.Observations = 2;

            stochast.FitPrior(prior, new double[] { 0, 0, 1, 0, 1 });
            ClassicAssert.AreEqual(0.511, stochast.Mean, margin);
            ClassicAssert.AreEqual(0.5, stochast.Deviation, margin);

            stochast.FitPrior(prior, new double[] { 1, 1 });
            ClassicAssert.AreEqual(0.766, stochast.Mean, margin);
            ClassicAssert.AreEqual(0.423, stochast.Deviation, margin);
        }

        [Test]
        public void TestPoisson()
        {
            var stochast = new Stochast { DistributionType = DistributionType.Poisson, Location = 5 };

            ClassicAssert.AreEqual(2, stochast.GetXFromU(StandardNormal.GetUFromP(0.1)), margin);
            ClassicAssert.AreEqual(5, stochast.GetXFromU(0), margin);
            ClassicAssert.AreEqual(8, stochast.GetXFromU(StandardNormal.GetUFromP(0.9)), margin);

            ClassicAssert.IsTrue(stochast.IsVarying());
        }

        [Test]
        public void TestComposite()
        {
            var stochast = new Stochast { DistributionType = DistributionType.Composite };

            var stochast1 = new Stochast { DistributionType = DistributionType.Uniform, Minimum = 0, Maximum = 8};
            var stochast2 = new Stochast { DistributionType = DistributionType.Uniform, Minimum = 6, Maximum = 10 };

            stochast.ContributingStochasts.Add(new ContributingStochast { Probability = 0.4, Stochast = stochast1 });
            stochast.ContributingStochasts.Add(new ContributingStochast { Probability = 0.6, Stochast = stochast2 });

            ClassicAssert.IsTrue(stochast.IsVarying());

            ClassicAssert.AreEqual(0.4 * 0.125 + 0.6 * 0, stochast.GetCDF(1.0), margin);
            ClassicAssert.AreEqual(0.4 * 0.875 + 0.6 * 0.25, stochast.GetCDF(7.0), margin);
            ClassicAssert.AreEqual(0.4 * 1 + 0.6 * 0.75, stochast.GetCDF(9.0), margin);

            TestStochast(stochast);
        }

        [Test]
        public void TestDiscrete()
        {
            var stochast = new Stochast { DistributionType = DistributionType.Discrete };
            stochast.DiscreteValues.Add(new DiscreteValue(1, 10));
            stochast.DiscreteValues.Add(new DiscreteValue(2, 20));
            stochast.DiscreteValues.Add(new DiscreteValue(3, 20));

            ClassicAssert.AreEqual(1, stochast.GetXFromU(StandardNormal.GetUFromP(0.1)), margin);
            ClassicAssert.AreEqual(2, stochast.GetXFromU(0), margin);
            ClassicAssert.AreEqual(3, stochast.GetXFromU(StandardNormal.GetUFromP(0.9)), margin);

            ClassicAssert.AreEqual(0.2, stochast.GetPDF(1), margin);
            ClassicAssert.AreEqual(0.4, stochast.GetPDF(2), margin);
            ClassicAssert.AreEqual(0.4, stochast.GetPDF(3), margin);
            ClassicAssert.AreEqual(0, stochast.GetPDF(1.5), margin);

            ClassicAssert.AreEqual(StandardNormal.GetUFromP(0.1), stochast.GetUFromX(1), margin);
            ClassicAssert.AreEqual(StandardNormal.GetUFromP(0.4), stochast.GetUFromX(2), margin);
            ClassicAssert.AreEqual(StandardNormal.GetUFromP(0.6), stochast.GetUFromX(2.5), margin);
            ClassicAssert.AreEqual(StandardNormal.GetUFromP(0.8), stochast.GetUFromX(3), margin);
            ClassicAssert.AreEqual(StandardNormal.BetaMax, stochast.GetUFromX(4), margin);

            ClassicAssert.AreEqual(0, stochast.GetCDF(0), margin);
            ClassicAssert.AreEqual(0.1, stochast.GetCDF(1), margin);
            ClassicAssert.AreEqual(0.4, stochast.GetCDF(2), margin);
            ClassicAssert.AreEqual(0.6, stochast.GetCDF(2.5), margin);
            ClassicAssert.AreEqual(0.8, stochast.GetCDF(3), margin);
            ClassicAssert.AreEqual(1, stochast.GetCDF(4), margin);

            ClassicAssert.IsTrue(stochast.IsVarying());

            TestFit(stochast);
        }

        [Test]
        public void TestFragilityCurve()
        {
            var stochast = new Stochast { DistributionType = DistributionType.CDFCurve };
            stochast.FragilityValues.Add(new FragilityValue { X = 1, Reliability = -1 });
            stochast.FragilityValues.Add(new FragilityValue { X = 2, Reliability = 0 });
            stochast.FragilityValues.Add(new FragilityValue { X = 3, Reliability = 1 });

            ClassicAssert.AreEqual(1, stochast.GetXFromU(-1), margin);
            ClassicAssert.AreEqual(2, stochast.GetXFromU(0), margin);
            ClassicAssert.AreEqual(3, stochast.GetXFromU(1), margin);
            ClassicAssert.AreEqual(4, stochast.GetXFromU(2), margin);

            ClassicAssert.AreEqual(-1, stochast.GetUFromX(1), margin);
            ClassicAssert.AreEqual(0, stochast.GetUFromX(2), margin);
            ClassicAssert.AreEqual(1, stochast.GetUFromX(3), margin);
            ClassicAssert.AreEqual(2, stochast.GetUFromX(4), margin);

            // compare to normal stochast
            Stochast normalStochast = new Stochast
            {
                DistributionType = DistributionType.Normal,
                Mean = 2,
                Deviation = 1
            };

            ClassicAssert.AreEqual(normalStochast.GetUFromX(1.5), stochast.GetUFromX(1.5), margin);
            ClassicAssert.AreEqual(normalStochast.GetXFromU(2.5), stochast.GetXFromU(2.5), margin);

            // not ordered
            stochast.FragilityValues.Clear();
            stochast.FragilityValues.Add(new FragilityValue { X = 2, Reliability = 0 });
            stochast.FragilityValues.Add(new FragilityValue { X = 3, Reliability = 1 });
            stochast.FragilityValues.Add(new FragilityValue { X = 1, Reliability = -1 });

            ClassicAssert.AreEqual(1, stochast.GetXFromU(-1), margin);
            ClassicAssert.AreEqual(2, stochast.GetXFromU(0), margin);
            ClassicAssert.AreEqual(3, stochast.GetXFromU(1), margin);
            ClassicAssert.AreEqual(4, stochast.GetXFromU(2), margin);

            ClassicAssert.AreEqual(-1, stochast.GetUFromX(1), margin);
            ClassicAssert.AreEqual(0, stochast.GetUFromX(2), margin);
            ClassicAssert.AreEqual(1, stochast.GetUFromX(3), margin);
            ClassicAssert.AreEqual(2, stochast.GetUFromX(4), margin);
        }

        [Test]
        public void TestTable()
        {
            var stochast = new Stochast { DistributionType = DistributionType.Histogram };
            stochast.HistogramValues.Add(new HistogramValue(1, 3, 2));
            stochast.HistogramValues.Add(new HistogramValue(5, 7, 6));
            stochast.HistogramValues.Add(new HistogramValue(7, 9, 2));

            ClassicAssert.AreEqual(2, stochast.GetXFromU(StandardNormal.GetUFromP(0.1)), margin);
            ClassicAssert.AreEqual(6, stochast.GetXFromU(0), margin);
            ClassicAssert.AreEqual(8, stochast.GetXFromU(StandardNormal.GetUFromP(0.9)), margin);

            ClassicAssert.AreEqual(StandardNormal.GetUFromP(0.1), stochast.GetUFromX(2), margin);
            ClassicAssert.AreEqual(0, stochast.GetUFromX(6), margin);
            ClassicAssert.AreEqual(StandardNormal.GetUFromP(0.9), stochast.GetUFromX(8), margin);

            ClassicAssert.AreEqual(0.1, stochast.GetPDF(2), margin);

            stochast.Fit(new double[] { 2, 3, 5 });
            ClassicAssert.AreEqual(3, stochast.HistogramValues.Count);
            ClassicAssert.AreEqual(1.5, stochast.HistogramValues[0].LowerBound);
            ClassicAssert.AreEqual(5.5, stochast.HistogramValues[stochast.HistogramValues.Count - 1].UpperBound);
            ClassicAssert.AreEqual(1, stochast.HistogramValues[1].Amount);
            ClassicAssert.AreEqual(1, stochast.HistogramValues[2].Amount);

            stochast.Fit(new double[] { 10 });
            ClassicAssert.AreEqual(1, stochast.HistogramValues.Count);
            ClassicAssert.AreEqual(9.5, stochast.HistogramValues[0].LowerBound);
            ClassicAssert.AreEqual(10.5, stochast.HistogramValues[0].UpperBound);

            stochast.Fit(new double[] { -10 });
            ClassicAssert.AreEqual(1, stochast.HistogramValues.Count);
            ClassicAssert.AreEqual(-10.5, stochast.HistogramValues[0].LowerBound);
            ClassicAssert.AreEqual(-9.5, stochast.HistogramValues[0].UpperBound);

            stochast.Fit(new double[] { 10, 10 });
            ClassicAssert.AreEqual(1, stochast.HistogramValues.Count);
            ClassicAssert.AreEqual(10, stochast.HistogramValues[0].LowerBound);
            ClassicAssert.AreEqual(10, stochast.HistogramValues[0].UpperBound);

            stochast.Fit(new double[] { 10, 10, 10 });
            ClassicAssert.AreEqual(1, stochast.HistogramValues.Count);
            ClassicAssert.AreEqual(10, stochast.HistogramValues[0].LowerBound);
            ClassicAssert.AreEqual(10, stochast.HistogramValues[0].UpperBound);

            stochast.FitWeighted(new double[] { 10, 10, 10 }, new double[] { 1, 10, 100 });
            ClassicAssert.AreEqual(1, stochast.HistogramValues.Count);
            ClassicAssert.AreEqual(10, stochast.HistogramValues[0].LowerBound);
            ClassicAssert.AreEqual(10, stochast.HistogramValues[0].UpperBound);

            stochast.FitWeighted(new double[] { 10, 10, 10 }, new double[] { 1E-6, 1, 1E-6 });
            ClassicAssert.AreEqual(1, stochast.HistogramValues.Count);
            ClassicAssert.AreEqual(10.0, stochast.HistogramValues[0].LowerBound);
            ClassicAssert.AreEqual(10.0, stochast.HistogramValues[0].UpperBound);

            stochast.Fit(new double[] { 1000000, 1000000, 1000000 });
            ClassicAssert.AreEqual(1, stochast.HistogramValues.Count);
            ClassicAssert.AreEqual(1000000, stochast.HistogramValues[0].LowerBound);
            ClassicAssert.AreEqual(1000000, stochast.HistogramValues[0].UpperBound);

            stochast.Fit(new double[] { 10, 10, 10, 20, 20, 20, 20, 20, 30, 30, 40, 40, 40, 40 });
            ClassicAssert.AreEqual(4, stochast.HistogramValues.Count);
            ClassicAssert.AreEqual(10, stochast.HistogramValues[0].LowerBound);
            ClassicAssert.AreEqual(10, stochast.HistogramValues[0].UpperBound);

            stochast.Fit(new double[] { 2, 3, 4, 4, 5, 5, 5, 5, 6, 6, 7, 8, 10000 });
            ClassicAssert.AreEqual(3, stochast.HistogramValues.Count);

            stochast.Fit(new double[] { 2, 2.0001, 2.0001, 2.0002, 2.0003, 2.0003, 2.0005, 7, 8, 10000 });
            ClassicAssert.AreEqual(3, stochast.HistogramValues.Count);

            List<double> samples = new List<double>();
            for (int i = 0; i < 1000; i++)
            {
                // intended loss of fraction for test
                samples.Add(i / 100);
            }

            stochast.Fit(samples.ToArray());

            ClassicAssert.AreEqual(10, stochast.HistogramValues.Count);
            for (int j = 1; j < stochast.HistogramValues.Count - 1; j++)
            {
                ClassicAssert.AreNotEqual(stochast.HistogramValues[j].LowerBound, stochast.HistogramValues[j].UpperBound);
            }
        }

        [Test]
        public void TestConditionalStochast()
        {
            Stochast realizedStochast = new Stochast { DistributionType = DistributionType.Uniform, Minimum = 0, Maximum = 1 };
            realizedStochast.IsVariableStochast = true;

            realizedStochast.ValueSet.StochastValues.Add(new VariableStochastValue { X = 0, Minimum = 0, Maximum = 1 });
            realizedStochast.ValueSet.StochastValues.Add(new VariableStochastValue { X = 2, Minimum = 11, Maximum = 12 });
            realizedStochast.ValueSet.StochastValues.Add(new VariableStochastValue { X = 1, Minimum = 1, Maximum = 2 });

            realizedStochast.InitializeForRun();

            // exact value
            ClassicAssert.AreEqual(0.5, realizedStochast.GetXFromUAndSource(0, 0), margin);
            ClassicAssert.AreEqual(1.5, realizedStochast.GetXFromUAndSource(1, 0), margin);
            ClassicAssert.AreEqual(11.5, realizedStochast.GetXFromUAndSource(2, 0), margin);

            // interpolated
            ClassicAssert.AreEqual(1, realizedStochast.GetXFromUAndSource(0.5, 0), margin);
            ClassicAssert.AreEqual(6.5, realizedStochast.GetXFromUAndSource(1.5, 0), margin);

            // extrapolated
            ClassicAssert.AreEqual(0.5, realizedStochast.GetXFromUAndSource(-1, 0), margin);
            ClassicAssert.AreEqual(11.5, realizedStochast.GetXFromUAndSource(3, 0), margin);
        }

        [Test]
        public void TestDesignValue()
        {
            Stochast stochast = new Stochast { DistributionType = DistributionType.Normal, Mean = 10, Deviation = 2 };

            // not modified
            ClassicAssert.AreEqual(10, stochast.DesignValue, margin);

            stochast.DesignFactor = 2;
            stochast.DesignQuantile = StandardNormal.GetPFromU(2);

            ClassicAssert.AreEqual((10 + 2*2) /2.0, stochast.DesignValue, margin);
        }

        private void TestStochast(Stochast stochast, double delta = 0.01)
        {
            foreach (var u in new[] { -7, -6, -5, -4, -3, -2, -1.5, -1, -0.5, -0.1, 0, 0.1, 0.5, 1, 1.5, 2, 3, 4, 5, 6, 7 })
            {
                double x = stochast.GetXFromU(u);
                double uu = stochast.GetUFromX(x);

                ClassicAssert.AreEqual(u, uu, delta);
            }
        }

        private static void TestSettingValuesOrder(Stochast stochast)
        {
            Stochast newStochast = new Stochast { DistributionType = stochast.DistributionType, Mean = stochast.Mean, Deviation = stochast.Deviation };
            ClassicAssert.AreEqual(stochast.Mean, newStochast.Mean, margin);
            ClassicAssert.AreEqual(stochast.Deviation, newStochast.Deviation, margin);

            newStochast = new Stochast { Mean = stochast.Mean, Deviation = stochast.Deviation };
            newStochast.SetDistributionType(stochast.DistributionType);
            ClassicAssert.AreEqual(stochast.Mean, newStochast.Mean, margin);
            ClassicAssert.AreEqual(stochast.Deviation, newStochast.Deviation, margin);
        }

        private static void TestSettingProperties(Stochast stochast)
        {
            stochast.Mean = 3;
            stochast.Deviation = 1.5;

            ClassicAssert.AreEqual(stochast.Mean, 3, margin);

            stochast.Mean = -3;
            stochast.SetShift(-5);

            ClassicAssert.AreEqual(stochast.Mean, -3, margin);

            stochast.Inverted = true;
            stochast.Mean = -1;
            stochast.SetShift(2);

            ClassicAssert.AreEqual(stochast.Mean, -1, margin);
        }

        private static void TestSettingPropertiesGumbel(Stochast stochast)
        {
            stochast.Mean = 3;
            stochast.Deviation = 1.5;

            ClassicAssert.AreEqual(stochast.Mean, 3, margin);

            stochast.Deviation = -1;
            stochast.Mean = -3;
            stochast.Deviation = 1;

            ClassicAssert.AreEqual(stochast.Mean, -3, margin);
        }


        private void TestMinMax(Stochast stochast)
        {
            ClassicAssert.AreEqual(-StandardNormal.UMax, stochast.GetUFromX(stochast.Minimum - 1), 0.01);
            ClassicAssert.AreEqual(StandardNormal.UMax, stochast.GetUFromX(stochast.Maximum + 1), 0.01);
        }

        private void TestSetXAtU(Stochast stochast, double x, double u, bool deviationZero = false)
        {
            const double delta = 0.025;

            double deviation = deviationZero ? 0 : stochast.Deviation;

            stochast.SetXAtU(x, u, ConstantParameterType.Deviation);

            ClassicAssert.AreEqual(x, stochast.GetXFromU(u), delta);
            ClassicAssert.AreEqual(deviation, stochast.Deviation, delta);

            double modifiedU = u < 0.5 ? 2 * u : 1 - 2 * (1 - u);

            double vc = stochast.Deviation / stochast.Mean;

            stochast.SetXAtU(x, modifiedU, ConstantParameterType.VariationCoefficient);

            ClassicAssert.AreEqual(x, stochast.GetXFromU(modifiedU), delta);
            ClassicAssert.AreEqual(vc, stochast.Deviation / stochast.Mean, delta);
        }

        private void TestInvert(Stochast stochast, bool useShift, double delta = 0.01)
        {
            double center = stochast.Shift;

            ClassicAssert.IsFalse(stochast.Inverted);

            // test mean inverted
            stochast.Inverted = true;
            InvertMinMax(stochast, useShift);
            double meanInverted = stochast.Mean;

            stochast.Inverted = false;
            InvertMinMax(stochast, useShift);
            double mean = stochast.Mean;

            ClassicAssert.AreEqual(mean, 2 * center - meanInverted, delta);

            foreach (var u in new[] { -7, -6, -5, -4, -3, -2, -1.5, -1, -0.5, -0.1, 0, 0.1, 0.5, 1, 1.5, 2, 3, 4, 5, 6, 7 })
            {
                stochast.Inverted = true;
                InvertMinMax(stochast, useShift);
                double xInverted = stochast.GetXFromU(-u);

                stochast.Inverted = false;
                InvertMinMax(stochast, useShift);
                double x = stochast.GetXFromU(u);

                ClassicAssert.AreEqual(x, 2 * center - xInverted, delta);

                double uu = stochast.GetUFromX(x);
                ClassicAssert.AreEqual(u, uu, delta);
            }

            // reset for further tests

            if (stochast.Inverted)
            {
                stochast.Inverted = false;
                InvertMinMax(stochast, useShift);
            }

            ClassicAssert.IsFalse(stochast.Inverted);
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

            ClassicAssert.AreEqual(stochast.Location, fittedStochast.Location, fitMargin);
            ClassicAssert.AreEqual(stochast.Scale, fittedStochast.Scale, fitMargin);
            ClassicAssert.AreEqual(stochast.Shift, fittedStochast.Shift, fitMargin);
            ClassicAssert.AreEqual(stochast.ShiftB, fittedStochast.ShiftB, fitMargin);
            ClassicAssert.AreEqual(stochast.Minimum, fittedStochast.Minimum, fitMargin);
            ClassicAssert.AreEqual(stochast.Maximum, fittedStochast.Maximum, fitMargin);
            ClassicAssert.AreEqual(stochast.Shape, fittedStochast.Shape, fitMargin);
            ClassicAssert.AreEqual(stochast.ShapeB, fittedStochast.ShapeB, fitMargin);
        }
    }
}
