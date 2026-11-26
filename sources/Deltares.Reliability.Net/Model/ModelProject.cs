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

using Deltares.Probabilistic.Statistics;
using Deltares.Probabilistic.Utils;
using System.Collections.Generic;
using System.Linq;

namespace Deltares.Probabilistic.Model
{
    /// <summary>
    /// Project containing a model and stochastic variables
    /// </summary>
    public class ModelProject : BaseModelProject
    {
        private CallBackList<Stochast> stochasts = null;
        private CorrelationMatrix correlationMatrix = null;

        public ModelProject()
        {
        }

        internal ModelProject(int id) : base(id)
        {
        }

        public IList<Stochast> Stochasts
        {
            get
            {
                if (stochasts == null)
                {
                    stochasts = new CallBackList<Stochast>(StochastValuesChanged);

                    int[] stochastIds = Interface.GetArrayIdValue(GetId(), "variables");
                    foreach (int stochastId in stochastIds)
                    {
                        stochasts.AddWithoutCallBack(ObjectFactory.GetObject<Stochast>(stochastId));
                    }
                }

                return stochasts;
            }
        }

        private void StochastValuesChanged(ListOperationType listOperation, Stochast item)
        {
            Interface.SetArrayIntValue(GetId(), "variables", this.stochasts.Select(p => p.GetId()).ToArray());
        }

        public CorrelationMatrix CorrelationMatrix
        {
            get
            {
                if (correlationMatrix == null)
                {
                    int matrixId = Interface.GetIdValue(GetId(), "correlation_matrix");
                    if (matrixId == 0)
                    {
                        correlationMatrix = new CorrelationMatrix();
                        Interface.SetIntValue(GetId(), "correlation_matrix", correlationMatrix.GetId());
                    }
                    else
                    {
                        correlationMatrix = new CorrelationMatrix(matrixId);
                    }
                }

                return correlationMatrix;
            }
            set
            {
                Interface.SetIntValue(GetId(), "correlation_matrix", value.GetId());
                correlationMatrix = value;
            }
        }
    }
}