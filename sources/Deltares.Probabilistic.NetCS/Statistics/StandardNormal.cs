using System;
using Deltares.Probabilistic.Utils;

namespace Deltares.Probabilistic.Statistics;

public static class StandardNormal
{
    private static int id = 0;

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
}