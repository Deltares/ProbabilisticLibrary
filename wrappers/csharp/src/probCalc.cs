using System;
using System.Runtime.InteropServices;

namespace Deltares.ProbabilisticCPP.Wrapper
{
    public class PbSizes
    {
        public const int maxActiveStochast = 32;
        public const int lenSmallStr = 32;
        public const int errorMsgLength = 256;
    }

    public enum rndTypes
    {
        GeorgeMarsaglia = 1,
        MersenneTwister
    };

    public enum ProbMethod
    {
        FORM = 1,
        CM = 3,
        DS = 4,
        NI = 5,
        IMHR = 6,
        FDIR = 11,
        DSFIHR,
        CMFORM,
        CMIMFORM,
        FORMDSFIHR,
        DSFI,
        FORMDSFI,
        IM,
    };

    public enum StartMethods
    {
        Zero = 1,
        One,
        GivenVector,
        RaySearch,
        SphereSearch,
        RaySearchVector = 8,
        RaySearchVectorScaled
    };

    public enum DPoptions
    {
        None,
        XfromU,
        XCorrelatedFromU,
        RMin,
        RMinZFunc,
        XCorrelatedFromUCompatible,
        RMinZFuncCompatible,
        MultiplyInvCholesky
    };

    public enum DSiterationMethods
    {
        DirSamplingIterMethodRobust = 1,
        DirSamplingIterMethodFast,
        DirSamplingIterMethodFaster,
        DirSamplingIterMethodRobustBisection,
        DirSamplingIterMethodFastBisection,
        DirSamplingIterMethodFasterBisection,
        DirSamplingIterMethodFastAuto = 8,
        DirSamplingIterMethodFasterAuto
    };

    public struct corrStruct
    {
        public int idx1;
        public int idx2;
        public double correlation;
    };

    [StructLayout(LayoutKind.Sequential)]
    public struct basicSettings
    {
        public ProbMethod methodId;
        public double tolA;
        public double tolB;
        public double tolC;
        public int numThreads;
        public int chunkSize;
        public int minSamples;
        public int maxSamples;
        public int seed1;
        public int seed2;
        public rndTypes rnd;
        public StartMethods startMethod;
        public int progressInterval;
        public double varianceFactor;
        public double epsilonDu;
        public DPoptions designPointOptions;
        public double du;
        public double du1;
        public double relaxationFactor;
        public int trialLoops;
        public DSiterationMethods iterationMethod;
        public int numExtraInt;
        public int numExtraInt2;
        public double numExtraReal1;
        public double maximumUvalue;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = PbSizes.maxActiveStochast)] public double [] startVector;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = PbSizes.maxActiveStochast)] public double[] offsets;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = PbSizes.maxActiveStochast)] public double[] varianceFactors;
    }

    public struct fdistribs
    {
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = PbSizes.lenSmallStr)] public char [] name;
        public EnumDistributions distId;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)] public double[] p4;
    };

    public struct tResult
    {
        public int convergence;
        public double beta;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = PbSizes.maxActiveStochast)] public double[] alpha;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = PbSizes.maxActiveStochast)] public double[] x;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = PbSizes.maxActiveStochast)] public int[] iPoint;
        public int stepsNeeded;
        public int samplesNeeded;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = PbSizes.maxActiveStochast)] public double[] alpha2;
    };

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate double DelegateZFunction(IntPtr x, int[] i, ref tError error);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate bool DelegateProgressCancel(int iter, double beta, double convergenceIndicator);

    public class ProbCalc
    {
        public static void SetDefaults(ref basicSettings method)
        {
            method.tolA = 0.001;
            method.tolB = 0.001;
            method.tolC = 0.001;
            method.chunkSize = 200;
            method.minSamples = 10000;
            method.maxSamples = 99000;
            method.seed1 = 2;
            method.seed2 = 1;
            method.rnd = rndTypes.GeorgeMarsaglia;
            method.startMethod = 0;
            method.progressInterval = -1;
            method.varianceFactor = 1.5;
            method.epsilonDu = 0.001;
            method.designPointOptions = DPoptions.RMin;
            method.du = 0.01;
            method.du1 = 3.0;
            method.relaxationFactor = 0.7;
            method.trialLoops = 3;
            method.iterationMethod = DSiterationMethods.DirSamplingIterMethodFast;
            method.numExtraInt = 100;
            method.numExtraReal1 = -5.0;
            method.maximumUvalue = 5.0;
        }

        public static tResult Calc(basicSettings method, fdistribs[] c,
            DelegateZFunction f, DelegateProgressCancel pc)
        {
            var ids = new int[1];
            var correlations = new corrStruct[0];
            return Calc(method, c, correlations, f, pc, ids);
        }

        public static tResult Calc(basicSettings method, fdistribs[] c, corrStruct[] correlations,
            DelegateZFunction f, DelegateProgressCancel pc)
        {
            var ids = new int[1];
            return Calc(method, c, correlations, f, pc, ids);
        }

        public static tResult Calc(basicSettings method, fdistribs[] c, corrStruct[] correlations,
            DelegateZFunction f, DelegateProgressCancel pc, int[] ids)
        {
            int n = c.Length;
            int nrCorrelations = correlations.Length;
            double[] x = new double[n];
            int[] iPoint = new int[n];
            for (int i = 0; i < n; i++)
            {
                iPoint[i] = i;
            }
            probcalcf2c(ref method, c, n, n, correlations, nrCorrelations, f, pc, ids, iPoint, x, out tResult r, out tError ierr);
            if (ierr.errorCode != 0)
            {
                var s = ierr.message;
                throw new Exception(s);
            }
            return r;
        }

        [DllImport("pb_wrapper_dll.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "probcalcf2c")]
        private static extern void probcalcf2c(ref basicSettings method, fdistribs[] c, int n, int vectorLength,
            corrStruct[] correlations, int nrCorrelations,
            [MarshalAs(UnmanagedType.FunctionPtr)] DelegateZFunction fx,
            [MarshalAs(UnmanagedType.FunctionPtr)] DelegateProgressCancel pc,
            int[] compIds, int[] iPoint, double[] x, out tResult r, out tError ierr);
    }
}

