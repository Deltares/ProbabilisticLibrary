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

namespace Deltares.Probabilistic.Statistics;

public static class StandardNormal
{
    private static int id = 0;
    private static double betaMax = double.NaN;
    private static double uMax = double.NaN;

    static StandardNormal()
    {
        id = Interface.Create("standard_normal");
    }

    public static double GetUFromQ(double q)
    {
        return Interface.GetArgValue(id, "u_from_q", q);
    }

    public static double GetUFromP(double p)
    {
        return Interface.GetArgValue(id, "u_from_p", p);
    }

    public static double GetQFromU(double u)
    {
        return Interface.GetArgValue(id, "q_from_u", u);
    }

    public static double GetPFromU(double u)
    {
        return Interface.GetArgValue(id, "p_from_u", u);
    }

    public static double GetTFromU(double u)
    {
        return Interface.GetArgValue(id, "t_from_u", u);
    }

    public static double GetUFromT(double t)
    {
        return Interface.GetArgValue(id, "u_from_t", t);
    }

    public static double GetTFromP(double p)
    {
        return Interface.GetArgValue(id, "t_from_p", p);
    }

    public static double GetPFromT(double t)
    {
        return Interface.GetArgValue(id, "p_from_t", t);
    }

    public static double BetaMax
    {
        get
        {
            if (double.IsNaN(betaMax))
            {
                betaMax = Interface.GetValue(id, "beta_max");
            }

            return betaMax;
        }
    }

    public static bool IsBetaMin(double beta)
    {
        return beta <= -BetaMax;
    }

    public static bool IsBetaMax(double beta)
    {
        return beta >= BetaMax;
    }


    public static double UMax
    {
        get
        {
            if (double.IsNaN(uMax))
            {
                uMax = Interface.GetValue(id, "u_max");
            }

            return uMax;
        }
    }
}