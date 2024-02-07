using System;
using Deltares.Statistics.Wrappers;
using NUnit.Framework;

namespace Deltares.Probabilistics.Wrappers.Test
{
    [TestFixture]
    public class TestStandardNormal
    {
        private const double margin = 1E-6;

        [Test]
        public void GetUFromQTest()
        {
            Assert.AreEqual(-1, StandardNormal.GetUFromQ(0.841344746068543), margin);
            Assert.AreEqual(-0.5, StandardNormal.GetUFromQ(0.691462461274013), margin);
            Assert.AreEqual(-0.1, StandardNormal.GetUFromQ(0.539827837277029), margin);
            Assert.AreEqual(-0.02, StandardNormal.GetUFromQ(0.507978313716902), margin);
            Assert.AreEqual(-0.006, StandardNormal.GetUFromQ(0.502393639320564), margin);
            Assert.AreEqual(0, StandardNormal.GetUFromQ(0.5), margin);
            Assert.AreEqual(0.006, StandardNormal.GetUFromQ(0.497606360679436), margin);
            Assert.AreEqual(0.02, StandardNormal.GetUFromQ(0.492021686283098), margin);
            Assert.AreEqual(0.1, StandardNormal.GetUFromQ(0.460172162722971), margin);
            Assert.AreEqual(0.5, StandardNormal.GetUFromQ(0.308537538725987), margin);
            Assert.AreEqual(1, StandardNormal.GetUFromQ(0.158655253931457), margin);
            Assert.AreEqual(1.2815519386788521, StandardNormal.GetUFromQ(0.1), margin);
            Assert.AreEqual(-1.2815519386788521, StandardNormal.GetUFromQ(0.9), margin);

            Assert.AreEqual(double.NaN, StandardNormal.GetUFromQ(Double.NaN));
        }

        /// <summary>
        /// GetBaseValue uses the reliability method GetPQFromU which is similar to the standard normal distribution Excel function NORM.S.DIST with cumulative distribution.
        /// </summary>
        [Test]
        public void GetQFromUTest()
        {
            Assert.AreEqual(1, StandardNormal.GetQFromU(-100), margin);
            Assert.AreEqual(1, StandardNormal.GetQFromU(-10), margin);
            Assert.AreEqual(0.841345, StandardNormal.GetQFromU(-1), margin);
            Assert.AreEqual(0.691462, StandardNormal.GetQFromU(-0.5), margin);
            Assert.AreEqual(0.539828, StandardNormal.GetQFromU(-0.1), margin);
            Assert.AreEqual(0.507978, StandardNormal.GetQFromU(-0.02), margin);
            Assert.AreEqual(0.502394, StandardNormal.GetQFromU(-0.006), margin);
            Assert.AreEqual(0.5, StandardNormal.GetQFromU(0), margin);
            Assert.AreEqual(0.497606, StandardNormal.GetQFromU(0.006), margin);
            Assert.AreEqual(0.492022, StandardNormal.GetQFromU(0.02), margin);
            Assert.AreEqual(0.460172, StandardNormal.GetQFromU(0.1), margin);
            Assert.AreEqual(0.308538, StandardNormal.GetQFromU(0.5), margin);
            Assert.AreEqual(0.158655, StandardNormal.GetQFromU(1), margin);
            Assert.AreEqual(0, StandardNormal.GetQFromU(10), margin);
            Assert.AreEqual(0, StandardNormal.GetQFromU(100), margin);
            Assert.AreEqual(0.1, StandardNormal.GetQFromU(1.28155), margin);
            Assert.AreEqual(0.9, StandardNormal.GetQFromU(-1.28155), margin);

            Assert.AreEqual(double.NaN, StandardNormal.GetQFromU(Double.NaN));
        }

    }
}