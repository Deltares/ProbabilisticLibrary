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
using System.Linq;
using System.Reflection;
using Deltares.Probabilistic.Logging;
using Deltares.Probabilistic.Model;
using Deltares.Probabilistic.Reliability;
using Deltares.Probabilistic.Sensitivity;
using Deltares.Probabilistic.Uncertainty;
using Deltares.Probabilistic.Statistics;
using NUnit.Framework;
using NUnit.Framework.Legacy;

namespace Deltares.Probabilistic.Test;

[TestFixture]
public class TestObjectProperties
{
    [Test]
    [TestCase(typeof(Stochast), "Deviation", "Variation")]
    [TestCase(typeof(CorrelationMatrix), "Item")]
    [TestCase(typeof(HistogramValue))]
    [TestCase(typeof(DiscreteValue))]
    [TestCase(typeof(FragilityValue))]
    [TestCase(typeof(ProbabilityValue))]
    [TestCase(typeof(ConditionalValue))]
    [TestCase(typeof(RunProject))]
    [TestCase(typeof(RunSettings))]
    [TestCase(typeof(StochastPointAlpha), "Identifier")]
    [TestCase(typeof(Evaluation))]
    [TestCase(typeof(Message))]
    [TestCase(typeof(ReliabilityProject))]
    [TestCase(typeof(ReliabilitySettings))]
    [TestCase(typeof(StochastSettings))]
    [TestCase(typeof(DesignPoint))]
    [TestCase(typeof(ReliabilityResult))]
    [TestCase(typeof(CombineProject))]
    [TestCase(typeof(CombineSettings))]
    [TestCase(typeof(SensitivityProject))]
    [TestCase(typeof(SensitivitySettings))]
    [TestCase(typeof(SensitivityResult))]
    [TestCase(typeof(SensitivityValue))]
    [TestCase(typeof(UncertaintyProject))]
    [TestCase(typeof(UncertaintySettings))]
    [TestCase(typeof(UncertaintyResult), "Identifier")]
    public void TestProperties(Type type, params string[] excluded)
    {
        object obj = Activator.CreateInstance(type);

        foreach (PropertyInfo property in type.GetProperties())
        {
            if (property.CanRead && property.CanWrite && !excluded.Any(p => p == property.Name))
            {
                if (property.PropertyType == typeof(int))
                {
                    TestIntProperty(obj, property);
                }
                else if (property.PropertyType == typeof(double))
                {
                    TestDoubleProperty(obj, property);
                }
                else if (property.PropertyType == typeof(bool))
                {
                    TestBoolProperty(obj, property);
                }
                else if (property.PropertyType == typeof(string))
                {
                    TestStringProperty(obj, property);
                }
                else if (property.PropertyType.IsEnum)
                {
                    TestEnumProperty(obj, property);
                }
            }
        }
    }

    private void TestIntProperty(object obj, PropertyInfo property)
    {
        int current = (int)property.GetValue(obj);
        int modified = current + 1;
        property.SetValue(obj, modified);
        int modifiedRetrieved = (int)property.GetValue(obj);

        ClassicAssert.AreEqual(modified, modifiedRetrieved, property.Name);
    }

    private void TestDoubleProperty(object obj, PropertyInfo property)
    {
        const double margin = 1E-10;

        double current = (double)property.GetValue(obj);
        double modified = current + 0.5;
        property.SetValue(obj, modified);
        double modifiedRetrieved = (double)property.GetValue(obj);

        ClassicAssert.AreEqual(modified, modifiedRetrieved, margin, property.Name);
    }

    private void TestBoolProperty(object obj, PropertyInfo property)
    {
        bool current = (bool)property.GetValue(obj);
        bool modified = !current;
        property.SetValue(obj, modified);
        bool modifiedRetrieved = (bool)property.GetValue(obj);

        ClassicAssert.AreEqual(modified, modifiedRetrieved, property.Name);
    }

    private void TestStringProperty(object obj, PropertyInfo property)
    {
        string current = (string)property.GetValue(obj);
        string modified = current + "-modified";
        property.SetValue(obj, modified);
        string modifiedRetrieved = (string)property.GetValue(obj);

        ClassicAssert.AreEqual(modified, modifiedRetrieved, property.Name);
    }

    private void TestEnumProperty(object obj, PropertyInfo property)
    {
        foreach (var enumValue in Enum.GetValues(property.PropertyType))
        {
            property.SetValue(obj, enumValue);
            var enumValueRetrieved = property.GetValue(obj);

            ClassicAssert.AreEqual(enumValue, enumValueRetrieved, $"{property.Name} : {enumValue.ToString()}");
        }
    }
}