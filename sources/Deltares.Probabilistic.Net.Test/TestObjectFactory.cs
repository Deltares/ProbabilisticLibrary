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
using System.Reflection;
using System.Threading;
using Deltares.Probabilistic.Reliability;
using Deltares.Probabilistic.Statistics;
using Deltares.Probabilistic.Utils;
using NUnit.Framework;
using NUnit.Framework.Legacy;

namespace Deltares.Probabilistic.Test;

[TestFixture]
public class TestObjectFactory
{
    [SetUp]
    public void Setup()
    {
        GC.Collect();
        GC.WaitForPendingFinalizers();
        GC.Collect();
    }

    [Test]
    public void TestStochast()
    {
        int count = ObjectFactory.GetCount();

        // use an internal method so that objects are garbage collected
        TestStochastInternal(count);

        GC.Collect();
        GC.WaitForPendingFinalizers();
        GC.Collect();

        ClassicAssert.AreEqual(count, ObjectFactory.GetCount()); // no new instances
    }

    [Test]
    public void TestDesignPoint()
    {
        int count = ObjectFactory.GetCount();

        // use an internal method so that objects are garbage collected
        TestDesignPointInternal(count);

        GC.Collect();
        GC.WaitForPendingFinalizers();
        GC.Collect();

        ClassicAssert.AreEqual(count, ObjectFactory.GetCount()); // no new instances
    }

    private static void TestStochastInternal(int count)
    {
        Stochast newStochast = new Stochast();

        ClassicAssert.AreEqual(count + 1, ObjectFactory.GetCount());

        Stochast copyStochast = ObjectFactory.GetObject<Stochast>(ObjectFactory.GetLastId());

        ClassicAssert.AreEqual(count + 1, ObjectFactory.GetCount());
        ClassicAssert.AreSame(newStochast, copyStochast);
    }

    private static void TestDesignPointInternal(int count)
    {
        DesignPoint newDesignPoint = new DesignPoint();

        ClassicAssert.AreEqual(count + 1, ObjectFactory.GetCount());

        DesignPoint copyDesignPoint = ObjectFactory.GetObject<DesignPoint>(ObjectFactory.GetLastId());

        ClassicAssert.AreEqual(count + 1, ObjectFactory.GetCount());
        ClassicAssert.AreSame(newDesignPoint, copyDesignPoint);
    }
}