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
using Deltares.Probabilistic.Logging;
using Deltares.Probabilistic.Statistics;
using Deltares.Probabilistic.Utils;
using System.Collections.Generic;
using System.Linq;

namespace Deltares.Probabilistic.Reliability;

public class LengthEffectProject
{
    private readonly int id = 0;
    private CorrelationMatrix correlationMatrix = null;
    private SelfCorrelationMatrix selfCorrelationMatrix = null;
    private DesignPoint crossSectionDesignPoint = null;
    private DesignPoint designPoint = null;

    public LengthEffectProject()
    {
        this.id = Interface.Create("length_effect_project");
    }

    internal LengthEffectProject(int id)
    {
        this.id = id;
    }

    ~LengthEffectProject()
    {
        Interface.Destroy(id);
    }

    internal int GetId()
    {
        return id;
    }

    public double Length
    {
        get { return Interface.GetValue(id, "length"); }
        set { Interface.SetValue(id, "length", value); }
    }

    public double[] CorrelationLengths
    {
        get { return Interface.GetArrayValue(id, "correlation_lengths"); }
        set { Interface.SetArrayValue(id, "correlation_lengths", value); }
    }

    public SelfCorrelationMatrix SelfCorrelationMatrix
    {
        get
        {
            if (selfCorrelationMatrix == null)
            {
                int selfCorrelationMatrixId = Interface.GetIdValue(id, "correlation_matrix");
                if (selfCorrelationMatrixId == 0)
                {
                    selfCorrelationMatrix = new SelfCorrelationMatrix();
                    Interface.SetIntValue(id, "correlation_matrix", correlationMatrix.GetId());
                }
                else
                {
                    selfCorrelationMatrix = new SelfCorrelationMatrix(selfCorrelationMatrixId);
                }
            }
            return selfCorrelationMatrix;
        }
        set
        {
            Interface.SetIntValue(id, "correlation_matrix", value.GetId());
            selfCorrelationMatrix = value;
        }
    }

    public DesignPoint CrossSectionDesignPoint
    {
        get
        {
            if (crossSectionDesignPoint == null)
            {
                int designPointId = Interface.GetIdValue(id, "design_point_cross_section");
                crossSectionDesignPoint = ObjectFactory.GetObject<DesignPoint>(designPointId);
            }

            return crossSectionDesignPoint;
        }
        set
        {
            Interface.SetIntValue(id, "design_point_cross_section", value.GetId());
            crossSectionDesignPoint = value;
        }
    }

    public void Run()
    {
        designPoint = null;
        Interface.Execute(id, "run");
    }

    public DesignPoint DesignPoint
    {
        get
        {
            if (designPoint == null)
            {
                int designPointId = Interface.GetIdValue(id, "design_point");
                designPoint = ObjectFactory.GetObject<DesignPoint>(designPointId);
            }

            return designPoint;
        }
        set
        {
            Interface.SetIntValue(id, "design_point", value.GetId());
            designPoint = value;
        }
    }
}

 