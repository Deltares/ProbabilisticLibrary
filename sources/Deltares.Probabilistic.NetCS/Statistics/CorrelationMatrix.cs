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
using System.Linq;
using Deltares.Probabilistic.Utils;

namespace Deltares.Probabilistic.Statistics;

public class CorrelationMatrix : IDisposable
{
    private int id = 0;

    public CorrelationMatrix()
    {
        this.id = Interface.Create("correlation_matrix");
    }

    internal CorrelationMatrix(int id)
    {
        this.id = id;
    }

    public void Dispose()
    {
        Interface.Destroy(id);
    }

    internal int GetId()
    {
        return id;
    }

    public void Initialize(IList<Stochast> stochasts)
    {
        Interface.SetArrayIntValue(id, "variables", stochasts.Select(p => p.GetId()).ToArray());
    }

    public double GetCorrelation(Stochast stochast1, Stochast stochast2)
    {
        return Interface.GetIndexedIndexedValue(id, "correlation", stochast1.GetId(), stochast2.GetId());
    }

    public void SetCorrelation(Stochast stochast1, Stochast stochast2, double value)
    {
        Interface.SetIndexedIndexedValue(id, "correlation", stochast1.GetId(), stochast2.GetId(), value);
    }

    public double this[Stochast stochast1, Stochast stochast2]
    {
        get { return GetCorrelation(stochast1, stochast2); }
        set { SetCorrelation(stochast1, stochast2, value); }
    }

    public void ResolveConflictingCorrelations()
    {
        Interface.Execute(id, "resolve_conflicting_correlations"); 
    }

    public bool IsIdentity()
    {
        return Interface.GetBoolValue(id, "is_identity");
    }

    public int CountCorrelations()
    {
        return Interface.GetIntValue(id, "count_correlations");
    }

    public bool HasConflictingCorrelations()
    {
        return Interface.GetBoolValue(id, "has_conflicting_correlations");
    }
}