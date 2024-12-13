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
﻿using System;
using Deltares.Statistics.Wrappers;
using NUnit.Framework;

namespace Deltares.Probabilistic.Wrapper.Test
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
