using NUnit.Framework;
using Deltares.Statistics.Wrappers;

namespace Deltares.Probabilistic.Nuget.Test
{
    [TestFixture]
    public class TestStatistics
    {
        private const double margin = 0.001;

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

            // SetXAtU
            stochast.Mean = 0;
            stochast.Deviation = 1;
            stochast.SetXAtU(2, 0.05, ConstantParameterType.Deviation);
            Assert.AreEqual(stochast.GetXFromU(0.05), 2, margin);

            stochast.Mean = 2;
            stochast.Deviation = 1;
            stochast.SetXAtU(3, 0.05, ConstantParameterType.VariationCoefficient);
            Assert.AreEqual(stochast.GetXFromU(0.05), 3, margin);

            stochast.Mean = -2;
            stochast.Deviation = 1;
            stochast.SetXAtU(-1, 0.05, ConstantParameterType.VariationCoefficient);
            Assert.AreEqual(stochast.GetXFromU(0.05), -1, margin);

            stochast.Mean = 0;
            stochast.Deviation = 1;
            stochast.SetXAtU(2, 0.05, ConstantParameterType.VariationCoefficient);
            Assert.AreEqual(stochast.GetXFromU(0.05), 2, margin);

            stochast.Fit(new[] { 2.5, 3.5, 4.5, 5.5, 6.5 });
            Assert.AreEqual(4.5, stochast.Mean, margin);
            Assert.AreEqual(1.581, stochast.Deviation, margin);
        }
    }
}
