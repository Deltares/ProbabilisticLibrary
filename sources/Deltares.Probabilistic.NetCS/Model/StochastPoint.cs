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
using System.Linq;
using Deltares.Probabilistic.Utils;

namespace Deltares.Probabilistic.Model;

public class StochastPoint
{
    private int id = 0;
    private CallBackList<StochastPointAlpha> alphas = null;

    public StochastPoint()
    {
        this.id = Interface.Create("stochast_point");
    }

    internal StochastPoint(int id)
    {
        if (id > 0)
        {
            this.id = id;
        }
    }

    ~StochastPoint()
    {
        ObjectFactory.Destroy(id);
    }

    internal int GetId()
    {
        return id;
    }

    internal void SetId(int id)
    {
        this.id = id;
    }

    public double Beta
    {
        get { return Interface.GetValue(id, "beta"); }
        set { Interface.SetValue(id, "beta", value); }
    }

    public IList<StochastPointAlpha> Alphas
    {
        get
        {
            if (alphas == null)
            {
                alphas = new CallBackList<StochastPointAlpha>(AlphasChanged);

                int[] alphaIds = Interface.GetArrayIdValue(id, "alphas");
                foreach (int alphaId in alphaIds)
                {
                    alphas.AddWithoutCallBack(new StochastPointAlpha(alphaId));
                }
            }

            return alphas;
        }
    }

    private void AlphasChanged(ListOperationType listOperation, StochastPointAlpha item)
    {
        Interface.SetArrayIntValue(id, "alphas", this.alphas.Select(p => p.GetId()).ToArray());
    }

    public ModelSample GetModelSample()
    {
        return new ModelSample(this.Alphas.Select(p => p.X).ToArray());
    }
}