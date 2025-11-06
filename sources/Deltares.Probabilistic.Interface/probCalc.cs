using System;
using System.Runtime.InteropServices;

namespace Deltares.ProbabilisticLibrary.Interface
{
    public class PbSizes
    {
        public const int maxActiveStochast = 32;
        public const int lenSmallStr = 32;
        public const int errorMsgLength = 256;
    }

    public enum ProbMethod
    {
        FORM = 1,
        CM = 3,
        DS = 4,
        NI = 5,
        IM = 6,
        AdaptiveIM = 7,
        FDIR = 11,
        DSFIHR,
        CMFORM,
        CMIMFORM,
        FORMDSFIHR,
        DSFI,
        FORMDSFI,
        LatinHyperCube = 18,
        NumericalBisection,
        CobylaReliability,
        SubSetSimulation,
    };

    public enum StartMethods
    {
        Zero = 1,
        One,
        GivenVector,
        RaySearch,
        SphereSearch,
        RaySearchVector = 8,
        RaySearchVectorScaled,
        Sensitivity = 11
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
        MultiplyInvCholesky,
        CenterOfGravity,
        CenterOfAngles
    }

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

    public struct numericalBisectionSettings
    {
        int MinimumIterations;
        int MaximumIterations;
        double EpsilonBeta;
    };

    public struct latinHypercubeSettings
    {
        int MinimumSamples;
    };

    public struct cobylaReliabilitySettings
    {
        double EpsilonBeta;
        int MaximumIterations;
    };

    public struct subSetSimulationReliabilitySettings
    {
        double VariationCoefficient;
        double MarkovChainDeviation;
        int MinimumIterations;
        int MaximumIterations;
        int SampleMethod;
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
        public int seed;
        public int isRepeatableRandom;
        public StartMethods startMethod;
        public int maxStepsSphereSearch;
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
        public double numExtraReal2;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = PbSizes.maxActiveStochast)] public double [] startVector;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = PbSizes.maxActiveStochast)] public double[] offsets;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = PbSizes.maxActiveStochast)] public double[] varianceFactors;
        public bool allQuadrants;
        public numericalBisectionSettings numerical_bisection_settings;
        public latinHypercubeSettings latin_hypercube_settings;
        public cobylaReliabilitySettings cobyla_reliability_settings;
        public subSetSimulationReliabilitySettings sub_set_simulation_reliability_settings;

    }

    public struct fdistribs
    {
        public EnumDistributions distId;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)] public double[] p4;
    }

    public struct tResult
    {
        public tError error;
        public double beta;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = PbSizes.maxActiveStochast)] public double[] alpha;
        public int stepsNeeded;
        public int samplesNeeded;
        public bool convergence;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = PbSizes.maxActiveStochast)] public double[] x;
    }

    public struct tError
    {
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = PbSizes.errorMsgLength)]
        public string message;
        public int errorCode;
    }

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
            method.seed = 2;
            method.startMethod = StartMethods.Zero;
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
            method.numExtraReal2 = 5.0;
        }

        public struct tCompIds
        {
            public int id;
            public int nrStochasts;
            public int nrCorrelations;
        };

        public static tResult Calc(basicSettings method, fdistribs[] c,
            DelegateZFunction f, DelegateProgressCancel pc)
        {
            tCompIds ids = new tCompIds();
            var correlations = new corrStruct[0];
            return Calc(method, c, correlations, f, pc, ids);
        }

        public static tResult Calc(basicSettings method, fdistribs[] c, corrStruct[] correlations,
            DelegateZFunction f, DelegateProgressCancel pc)
        {
            tCompIds ids = new tCompIds();
            return Calc(method, c, correlations, f, pc, ids);
        }

        public static tResult Calc(basicSettings method, fdistribs[] c, corrStruct[] correlations,
            DelegateZFunction f, DelegateProgressCancel pc, tCompIds ids)
        {
            int n = c.Length;
            int nrCorrelations = correlations.Length;
            ids.id = 0;
            ids.nrStochasts = n;
            ids.nrCorrelations = nrCorrelations;
            double[] x = new double[n];
            probcalcf2c(ref method, c, correlations, f, pc, ids, x, out tResult r);
            if (r.error.errorCode != 0)
            {
                var s = r.error.message;
                throw new Exception(s);
            }
            r.x = x;
            return r;
        }

        public static void DumpResults(tResult result)
        {
            Console.WriteLine("beta = " + result.beta);
            Console.Write("alpha = ");
            for (int i = 0; i < result.x.Length; i++)
            {
                Console.Write(result.alpha[i]);
                Console.Write(",");
            }
            Console.WriteLine();

            Console.Write("x = ");
            foreach (var x in result.x)
            {
                Console.Write(x);
                Console.Write(",");
            }
            Console.WriteLine();
            Console.WriteLine("convergence: " + result.convergence);
            Console.WriteLine("samplesNeeded: " + result.samplesNeeded);
            Console.WriteLine("stepsNeeded: " + result.stepsNeeded);
            Console.WriteLine();
        }

        [DllImport("Deltares.Probabilistic.CWrapper.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "probcalcf2c")]
        private static extern void probcalcf2c(ref basicSettings method, fdistribs[] c, corrStruct[] correlations,
            [MarshalAs(UnmanagedType.FunctionPtr)] DelegateZFunction fx,
            [MarshalAs(UnmanagedType.FunctionPtr)] DelegateProgressCancel pc,
            tCompIds compIds, double[] x, out tResult r);
    }
}

