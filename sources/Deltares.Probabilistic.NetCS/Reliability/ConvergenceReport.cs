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
using Deltares.Probabilistic.Logging;
using Deltares.Probabilistic.Model;
using Deltares.Probabilistic.Utils;

namespace Deltares.Probabilistic.Reliability;

public class ConvergenceReport
{
    private int id = 0;

    public ConvergenceReport()
    {
        id = Interface.Create("convergence_report");
    }

    internal ConvergenceReport(int id)
    {
        this.id = id;
    }

    ~ConvergenceReport()
    {
        Interface.Destroy(id);
    }

    internal int GetId()
    {
        return id;
    }

    public double Convergence
    {
        get { return Interface.GetValue(id, "convergence"); }
        set { Interface.SetValue(id, "convergence", value); }
    }

    public bool IsConverged
    {
        get { return Interface.GetBoolValue(id, "is_converged"); }
        set { Interface.SetBoolValue(id, "is_converged", value); }
    }

    public double VarianceFactor
    {
        get { return Interface.GetValue(id, "variance_factor"); }
        set { Interface.SetValue(id, "variance_factor", value); }
    }

    public double RelaxationFactor
    {
        get { return Interface.GetValue(id, "relaxation_factor"); }
        set { Interface.SetValue(id, "relaxation_factor", value); }
    }

    public double FailFraction
    {
        get { return Interface.GetValue(id, "fail_fraction"); }
        set { Interface.SetValue(id, "fail_fraction", value); }
    }

    public double FailWeight
    {
        get { return Interface.GetValue(id, "fail_weight"); }
        set { Interface.SetValue(id, "fail_weight", value); }
    }

    public double MaxWeight
    {
        get { return Interface.GetValue(id, "max_weight"); }
        set { Interface.SetValue(id, "max_weight", value); }
    }

    public int FailedSamples
    {
        get { return Interface.GetIntValue(id, "failed_samples"); }
        set { Interface.SetIntValue(id, "failed_samples", value); }
    }

    public double ZMargin { get; set; }
}