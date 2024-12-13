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

            Assert.IsTrue(matrix.IsIdentity());

            matrix.SetCorrelation(a, b, 0.5);
            matrix.SetCorrelation(b, c, 0.1);

            Assert.AreEqual(0.5, matrix.GetCorrelation(a, b));
            Assert.AreEqual(0.5, matrix.GetCorrelation(b, a));
            Assert.AreEqual(0.1, matrix.GetCorrelation(b, c));
            Assert.AreEqual(0.1, matrix.GetCorrelation(c, b));
            Assert.AreEqual(0, matrix.GetCorrelation(a, c));
            Assert.AreEqual(1, matrix.GetCorrelation(b, b));
            Assert.AreEqual(1, matrix.GetCorrelation(e, e));

            Assert.IsFalse(matrix.IsIdentity());

            Assert.AreEqual(2, matrix.CountCorrelations());

            /*
            matrix.Stochasts.Remove(c);

            Assert.AreEqual(0.1, matrix.GetCorrelation(b, c));
            Assert.AreEqual(1, matrix.GetCorrelation(c, c));

            Assert.AreEqual(2, matrix.Correlations.Count);

            SparseMatrix corr = matrix.GetCorrelationMatrix(new List<IHasStochast> {b, c});

            Assert.AreEqual(2, corr.RowCount);
            Assert.AreEqual(2, corr.ColumnCount);
            Assert.AreEqual(1, corr[0, 0]);
            Assert.AreEqual(0.1, corr[0, 1]);
            Assert.AreEqual(0.1, corr[1, 0]);
            Assert.AreEqual(1, corr[0, 0]);

            matrix.Stochasts.Add(c);

            Assert.AreEqual(0.1, matrix.GetCorrelation(b, c));
            Assert.AreEqual(1, matrix.GetCorrelation(c, c));

            Assert.AreEqual(2, matrix.Correlations.Count);
            */
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

            Assert.IsTrue(matrix.IsIdentity());
            Assert.IsFalse(matrix.HasConflictingCorrelations());

            matrix.SetCorrelation(a, b, 1);
            matrix.SetCorrelation(b, c, 1);

            Assert.IsTrue(matrix.HasConflictingCorrelations());

            matrix.ResolveConflictingCorrelations();

            Assert.IsFalse(matrix.HasConflictingCorrelations());
            Assert.AreEqual(1, matrix.GetCorrelation(a, c));

            matrix.SetCorrelation(d, e, -1);

            Assert.IsFalse(matrix.HasConflictingCorrelations());
            
            matrix.SetCorrelation(e, a,-1);

            Assert.IsTrue(matrix.HasConflictingCorrelations());

            matrix.ResolveConflictingCorrelations();

            Assert.IsFalse(matrix.HasConflictingCorrelations());

            Assert.AreEqual(-1, matrix.GetCorrelation(c, e));
            Assert.AreEqual(1, matrix.GetCorrelation(c, d));
        }
    }
}
