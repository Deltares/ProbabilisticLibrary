using System.Runtime.InteropServices;

namespace Deltares.ProbabilisticCPP.Wrapper
{
    public struct tError
    {
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = PbSizes.errorMsgLength)]
        public string message;
        public int errorCode;
    };

    public class PLConversions
    {
        public static double PFromBeta(double beta)
        {
            return pFromBeta(ref beta);
        }

        public static double QFromBeta(double beta)
        {
            return qFromBeta(ref beta);
        }

        public static void PQFromBeta(double beta, out double p, out double q)
        {
            pqFromBeta(ref beta, out p, out q);
        }

        public static double BetaFromQ(double q)
        {
            tError error = new tError();
            betaFromQ(ref q, out double beta, ref error);
            return beta;
        }

        [DllImport("pb_wrapper_dll.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "pfrombeta")]
        private static extern double pFromBeta(ref double beta);

        [DllImport("pb_wrapper_dll.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "qfrombeta")]
        private static extern double qFromBeta(ref double beta);

        [DllImport("pb_wrapper_dll.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "pqfrombeta")]
        private static extern double pqFromBeta(ref double beta, out double p, out double q);

        [DllImport("pb_wrapper_dll.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "betafromqc")]
        private static extern double betaFromQ(ref double q, out double beta, ref tError e);
    }
}
