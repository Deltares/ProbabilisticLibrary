using System;
using System.Runtime.InteropServices;

namespace Deltares.ProbabilisticCPP.Wrapper
{
    public enum EnumDistributions
    {
        deterministic,
        uniform,
        normal,
        lognormal,
        lognormal2,
        shiftedExponential,
        gumbel,
        gumbel2,
        weibull,
        rayleigh,
        pareto,
        triangular,
        table,
        tableT,
        conditionalWeibull,
        modifiedGumbel,
        truncatedModifiedGumbel,
        RayleighN = 18,
        truncatedNormal,
        beta,
        uspace
    };

    public class PLDistributions
    {
        public static double CalculateDistributionInverse(double u, EnumDistributions type, double[] p4)
        {
            var ierr = new tError();
            calculatedistributioninverse_c(ref u, out double y, ref type, p4, ref ierr);
            if (ierr.errorCode != 0)
            {
                var s = ierr.message;
                throw new Exception(s);
            }
            return y;
        }

        [DllImport("pb_wrapper_dll.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "calculatedistributioninverse_c")]
        private static extern void calculatedistributioninverse_c(ref double u, out double y, ref EnumDistributions type, double[] p, ref tError ierr);
    }
}


