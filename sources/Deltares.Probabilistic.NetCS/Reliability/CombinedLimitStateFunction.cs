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
using Deltares.Probabilistic.Utils;
using System.Collections.Generic;
using System.Linq;

namespace Deltares.Probabilistic.Reliability;

public class CombinedLimitStateFunction : LimitStateFunction
{
    private int id = 0;
    private CallBackList<LimitStateFunction> limitStateFunctions = null;

    public CombinedLimitStateFunction() : base(-1)
    {
        this.id = Interface.Create("combined_limit_state_function");
        base.SetId(id);
    }

    internal CombinedLimitStateFunction(int id) : base(id)
    {
        this.id = id;
    }

    public CombineType CombineType
    {
        get { return CombineTypeConverter.ConvertFromString(Interface.GetStringValue(id, "combine_type")); }
        set { Interface.SetStringValue(id, "combine_type", CombineTypeConverter.ConvertToString(value)); }
    }

    public IList<LimitStateFunction> LimitStateFunctions
    {
        get
        {
            if (limitStateFunctions == null)
            {
                limitStateFunctions = new CallBackList<LimitStateFunction>(LimitStateFunctionsChanged);

                int[] parameterIds = Interface.GetArrayIdValue(id, "limit_state_functions");
                foreach (int parameterId in parameterIds)
                {
                    limitStateFunctions.AddWithoutCallBack(new LimitStateFunction(parameterId));
                }
            }

            return limitStateFunctions;
        }
    }

    private void LimitStateFunctionsChanged(ListOperationType listOperation, LimitStateFunction item)
    {
        Interface.SetArrayIntValue(id, "limit_state_functions", this.limitStateFunctions.Select(p => p.GetId()).ToArray());
    }
}