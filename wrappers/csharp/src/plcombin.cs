using System;
using System.Runtime.InteropServices;

namespace Deltares.ProbabilisticCPP.Wrapper
{
    public enum CombineAndOr
    {
        CombineAnd = 0,
        CombineOr = 1
    }

    public class PLCombin
    {
        public static double Hohenbichler(double betaV, double pfU, double rhoInput)
        {
            int n = hohenbichler(ref betaV, ref pfU, ref rhoInput, out double pfVpfU);
            if (n != 0)
            {
                Console.WriteLine("No convergence in Form with Hohenbichler.");
            }
            return pfVpfU;
        }

        public static void UpscaleLength(double betaCrossSection, double[] alphaCrossSection, double[] rhoXK, double[] dXK, double sectionLength,
                out double betaSection, out double[] alphaSection, double breachLength)
        {
            int n = alphaCrossSection.Length;
            upscaleLength(ref betaCrossSection, alphaCrossSection, rhoXK, dXK, ref sectionLength,
                    out betaSection, out alphaSection, ref breachLength, n);
        }

        public static void UpscaleLength(double betaCrossSection, double[] alphaCrossSection, double[] rhoXK, double[] dXK, double sectionLength,
                out double betaSection, out double[] alphaSection)
        {
            double breachLength = -999;
            int n = alphaCrossSection.Length;
            upscaleLength(ref betaCrossSection, alphaCrossSection, rhoXK, dXK, ref sectionLength,
                    out betaSection, out alphaSection, ref breachLength, n);
        }

        public static void UpscaleInTime(double nrElements, ref double beta, double[] alpha, double[] inRhoT)
        {
            int n = alpha.Length;
            upscaleintime(ref nrElements, ref beta, alpha, inRhoT, n);
        }

        public static void CombineTwoElementsPartialCorrelation(double beta1, double[] alpha1, double beta2, double[] alpha2,
            double[] rhoP, out double beta3, double[] alpha3, CombineAndOr CombineOr, double[] alphaI, double[] alphaII)
        {
            int nStoch = alpha1.Length;
            combinetwoelementspartialcorrelationc1(ref beta1, alpha1, ref beta2, alpha2, rhoP, out beta3, alpha3,
                (int)CombineOr, nStoch, alphaI, alphaII);
        }

        public static void CombineTwoElementsPartialCorrelation(double beta1, double[] alpha1, double beta2, double[] alpha2,
            double[] rhoP, out double beta3, double[] alpha3, CombineAndOr CombineOr)
        {
            int nStoch = alpha1.Length;
            combinetwoelementspartialcorrelationc2(ref beta1, alpha1, ref beta2, alpha2, rhoP, out beta3, alpha3, (int)CombineOr, nStoch);
        }

        public static void CombineMultipleElements(double[] betaElement, double[,] alphaElement, double[] rho,
          out double beta, double[] alpha, CombineAndOr combAndOrIn)
        {
            int nrElms = betaElement.Length;
            int nrStoch = rho.Length;
            combinemultipleelements_c(betaElement, alphaElement, rho, out beta, alpha, (int)combAndOrIn, nrElms, nrStoch);
        }


        [DllImport("pb_wrapper_dll.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "hohenbichler_c")]
        private static extern int hohenbichler(ref double betaV, ref double pfU, ref double rhoInput, out double pfVpfU);

        [DllImport("pb_wrapper_dll.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "upscalelengthc")]
        private static extern int upscaleLength(ref double betaCrossSection, double[] alphaCrossSection, double[] rhoXK, double[] dXK, ref double sectionLength,
                out double betaSection, out double[] alphaSection, ref double breachLength, int n);

        [DllImport("pb_wrapper_dll.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "upscaleintimec")]
        private static extern int upscaleintime(ref double nrElements, ref double beta, double[] alpha, double[] inRhoT, int n);

        [DllImport("pb_wrapper_dll.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "combinetwoelementspartialcorrelationc1")]
        private static extern int combinetwoelementspartialcorrelationc1(ref double beta1, double[] alpha1, ref double beta2, double[] alpha2,
            double[] rhoP, out double betaC, double[] alphaC,  int combAndOr, int nStochasts, double[] alphaI, double[] alphaII);

        [DllImport("pb_wrapper_dll.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "combinetwoelementspartialcorrelationc2")]
        private static extern int combinetwoelementspartialcorrelationc2(ref double beta1, double[] alpha1, ref double beta2, double[] alpha2,
            double[] rhoP, out double betaC, double[] alphaC, int combAndOr, int nStochasts);

        [DllImport("pb_wrapper_dll.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "combinemultipleelements_c")]
        private static extern int combinemultipleelements_c(double[] betaElement, double[,] alphaElement, double[] rho,
          out double beta, double[] alpha, int combAndOrIn, int nrElms, int nrStoch);
    }
}

