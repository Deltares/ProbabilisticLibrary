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
﻿using System.Linq;
using Deltares.Statistics.Wrappers;
using NUnit.Framework;
using NUnit.Framework.Legacy;

namespace Deltares.Probabilistic.Wrapper.Test
{
    [TestFixture]
    public class TestCorrelationMatrix
    {
        [Test]
        public void TestMatrix()
        {
            CorrelationMatrix matrix = new CorrelationMatrix();

            Stochast a = new Stochast();
            Stochast b = new Stochast();
            Stochast c = new Stochast();
            Stochast d = new Stochast();
            Stochast e = new Stochast();

            var s = new[] { a, b, c, d, e };

            matrix.Initialize(s.ToList());

            ClassicAssert.IsTrue(matrix.IsIdentity());

            matrix.SetCorrelation(a, b, 0.5);
            matrix.SetCorrelation(b, c, 0.1);

            ClassicAssert.AreEqual(0.5, matrix.GetCorrelation(a, b));
            ClassicAssert.AreEqual(0.5, matrix.GetCorrelation(b, a));
            ClassicAssert.AreEqual(0.1, matrix.GetCorrelation(b, c));
            ClassicAssert.AreEqual(0.1, matrix.GetCorrelation(c, b));
            ClassicAssert.AreEqual(0, matrix.GetCorrelation(a, c));
            ClassicAssert.AreEqual(1, matrix.GetCorrelation(b, b));
            ClassicAssert.AreEqual(1, matrix.GetCorrelation(e, e));

            ClassicAssert.IsFalse(matrix.IsIdentity());

            ClassicAssert.AreEqual(2, matrix.CountCorrelations());
        }

        [Test]
        public void TestFullCorrelations()
        {
            CorrelationMatrix matrix = new CorrelationMatrix();

            Stochast a = new Stochast();
            Stochast b = new Stochast();
            Stochast c = new Stochast();
            Stochast d = new Stochast();
            Stochast e = new Stochast();

            var s = new[] { a, b, c, d, e };
            matrix.Initialize(s.ToList());

            ClassicAssert.IsTrue(matrix.IsIdentity());
            ClassicAssert.IsFalse(matrix.HasConflictingCorrelations());

            matrix.SetCorrelation(a, b, 1);
            matrix.SetCorrelation(b, c, 1);

            ClassicAssert.IsTrue(matrix.HasConflictingCorrelations());

            matrix.ResolveConflictingCorrelations();

            ClassicAssert.IsFalse(matrix.HasConflictingCorrelations());
            ClassicAssert.AreEqual(1, matrix.GetCorrelation(a, c));

            matrix.SetCorrelation(d, e, -1);

            ClassicAssert.IsFalse(matrix.HasConflictingCorrelations());
            
            matrix.SetCorrelation(e, a,-1);

            ClassicAssert.IsTrue(matrix.HasConflictingCorrelations());

            matrix.ResolveConflictingCorrelations();

            ClassicAssert.IsFalse(matrix.HasConflictingCorrelations());

            ClassicAssert.AreEqual(-1, matrix.GetCorrelation(c, e));
            ClassicAssert.AreEqual(1, matrix.GetCorrelation(c, d));
        }
    }
}
