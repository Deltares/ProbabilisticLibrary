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
using Deltares.Probabilistic.Model;
using Deltares.Probabilistic.Reliability;
using Deltares.Probabilistic.Sensitivity;
using Deltares.Probabilistic.Uncertainty;
using Deltares.Probabilistic.Statistics;
using NUnit.Framework;

namespace Deltares.Probabilistic.Test;

[TestFixture]
public class TestObjectProperties
{
    [Test]
    [TestCase(typeof(Stochast), "Mean", "Deviation", "Variation", "DesignValue")]
    [TestCase(typeof(CorrelationMatrix), "Item")]
    [TestCase(typeof(HistogramValue))]
    [TestCase(typeof(DiscreteValue))]
    [TestCase(typeof(FragilityValue))]
    [TestCase(typeof(ContributingStochast))]
    [TestCase(typeof(ProbabilityValue))]
    [TestCase(typeof(ConditionalValue))]
    [TestCase(typeof(RunProject))]
    [TestCase(typeof(StochastPointAlpha), "Identifier")]
    [TestCase(typeof(ReliabilityProject))]
    [TestCase(typeof(ReliabilitySettings))]
    [TestCase(typeof(LimitStateFunction))]
    [TestCase(typeof(StochastSettings))]
    [TestCase(typeof(DesignPoint))]
    [TestCase(typeof(ConvergenceReport))]
    [TestCase(typeof(ReliabilityResult))]
    [TestCase(typeof(ConvergenceReport))]
    [TestCase(typeof(FragilityCurve), "DistributionType", "ConstantParameterType", "Truncated", "Mean", "Deviation", "Variation", "DesignValue")]
    [TestCase(typeof(CombineProject))]
    [TestCase(typeof(CombineSettings))]
    [TestCase(typeof(ExcludingCombineProject))]
    [TestCase(typeof(ExcludingCombineSettings))]
    [TestCase(typeof(SensitivityProject))]
    [TestCase(typeof(SensitivitySettings))]
    [TestCase(typeof(SensitivityResult))]
    [TestCase(typeof(SensitivityValue))]
    [TestCase(typeof(UncertaintyProject))]
    [TestCase(typeof(UncertaintySettings))]
    [TestCase(typeof(UncertaintyResult), "Identifier")]
    public void TestProperties(Type type, params string[] excluded)
    {
        ObjectPropertiesTester.TestProperties(type, excluded);
    }
}
