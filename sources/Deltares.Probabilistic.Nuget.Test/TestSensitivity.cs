// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of Streams.
//
// Streams is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// All names, logos, and references to "Deltares" are registered trademarks of
// Stichting Deltares and remain full property of Stichting Deltares at all times.
// All rights reserved.
//
﻿using System;
using System.Threading;
using NUnit.Framework;
using Deltares.Models.Wrappers;
using Deltares.Statistics.Wrappers;
using Deltares.Sensitivity.Wrappers;
using System.Linq;

namespace Deltares.Probabilistic.Nuget.Test
{
    [TestFixture]
    public class TestSensitivity
    {
        private const double margin = 0.02;

        [Test]
        public void TestLinear()
        {
            var project = GetLinearProject();

            project.SensitivityMethod = new CrudeMonteCarloS();
            ((CrudeMonteCarloS)project.SensitivityMethod).Settings.RandomSettings.RandomGeneratorType = RandomGeneratorType.MersenneTwister;

            Stochast stochast = project.GetStochast();

            Assert.AreEqual(DistributionType.Table, stochast.DistributionType);
            Assert.AreEqual(84, stochast.HistogramValues.Count);

            Assert.AreEqual(1.8, stochast.Mean, margin);
            Assert.AreEqual(0.82, stochast.Deviation, margin);
        }

        private static SensitivityProject GetLinearProject()
        {
            var project = new SensitivityProject();

            project.Stochasts.Add(new Stochast { DistributionType = DistributionType.Uniform, Minimum = -1, Maximum = 1 });
            project.Stochasts.Add(new Stochast { DistributionType = DistributionType.Uniform, Minimum = -1, Maximum = 1 });

            project.CorrelationMatrix.Initialize(project.Stochasts);

            project.ZFunction = LinearZFunction;

            return project;
        }

        private static void LinearZFunction(ModelSample sample)
        {
            sample.Z = 1.8 - sample.Values.Sum();
        }
    }
}
