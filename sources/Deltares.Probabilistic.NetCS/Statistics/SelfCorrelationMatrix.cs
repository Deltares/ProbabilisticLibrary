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
using System.Collections.Generic;
using Deltares.Probabilistic.Reliability;
using Deltares.Probabilistic.Utils;

namespace Deltares.Probabilistic.Statistics;

public class SelfCorrelationMatrix
{
    private int id = 0;
    private List<Stochast> stochasts = null;

    public SelfCorrelationMatrix()
    {
        this.id = Interface.Create("self_correlation_matrix");
    }

    internal SelfCorrelationMatrix(int id)
    {
        this.id = id;
    }

    ~SelfCorrelationMatrix()
    {
        Interface.Destroy(id);
    }

    internal int GetId()
    {
        return id;
    }

    public double GetCorrelation(Stochast stochast, DesignPoint designPoint1, DesignPoint designPoint2)
    {
        Interface.SetIntValue(id, "correlation_stochast", stochast.GetId());
        return Interface.GetIndexedIndexedValue(id, "correlation", designPoint1.GetId(), designPoint2.GetId());
    }

    public void SetCorrelation(Stochast stochast, DesignPoint designPoint1, DesignPoint designPoint2, double value)
    {
        Interface.SetIntValue(id, "correlation_stochast", stochast.GetId());
        Interface.SetIndexedIndexedValue(id, "correlation", designPoint1.GetId(), designPoint2.GetId(), value);
    }

    public double this[Stochast stochast, DesignPoint designPoint1, DesignPoint designPoint2]
    {
        get { return GetCorrelation(stochast, designPoint1, designPoint2); }
        set { SetCorrelation(stochast, designPoint1, designPoint2, value); }
    }

    public bool IsValid()
    {
        return Interface.GetBoolValue(id, "is_valid");
    }

    public double GetCorrelationValue(Stochast stochast)
    {
        return Interface.GetIntArgValue(id, stochast.GetId(), "rho");
    }

    public void SetCorrelationValue(Stochast stochast, double correlationValue)
    {
        Interface.SetIntArgValue(id, stochast.GetId(), "rho", correlationValue);
    }
}