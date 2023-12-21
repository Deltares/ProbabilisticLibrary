using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Threading;
using Deltares.ProbabilisticCPP.Wrapper;

namespace Probabilistic.Tests
{
    public class ProbLibWrapperTests
    {

        private static List<double> betas = new List<double>();
        private static List<double> convIndicators = new List<double>();

        public static void runAllProbLibWrapperTests()
        {
            BasicTestIS();
            TestErrorHandling();
            TestErrorHandlingDS();
            TestDelegateFORM();
            TestDelegateFORMCorrelated();
            TestDelegateFullyCorrelatedFORM();
            TestConvergenceDataDS();
            TestFORM_StartVector();
            TestTwoThreads();
            TestBm3_17();
            TestErrorHandlingZFunc();
            TestErrorHandlingZFuncDS();
            TestErrorHandlingZFuncNumInt();
        }

        internal static double SimpleZFunc(IntPtr x, int[] i, ref tError ierr)
        {
            var xx = new double[2];
            Marshal.Copy(x, xx, 0, xx.Length);
            ierr.errorCode = 0;
            return  xx[1] - xx[0];
        }

        internal static double AnotherSimpleZFunc(IntPtr x, int[] i, ref tError ierr)
        {
            var xx = new double[2];
            Marshal.Copy(x, xx, 0, xx.Length);
            ierr.errorCode = 0;
            return 1.0 + xx[1] - 0.5 * xx[0];
        }

        internal static bool progressForm(int i, double x, double y)
        {
            if (i % 2 == 0)
            {
                Console.WriteLine(string.Format("iter = {0}, beta = {1}, conv = {2}", i, x, y));
            }
            return false;
        }

        internal static bool progressFormList(int i, double x, double y)
        {
            betas.Add(x);
            convIndicators.Add(y);
            return false;
        }

        internal static bool progressSampling(int i, double x, double y)
        {
            if (i % 1000 == 0)
            {
                Console.WriteLine(string.Format("samples = {0}, Pf = {1}, conv = {2}", i, x, y));
            }
            return false;
        }

        internal static double SimpleZ(IntPtr x, int[] i, ref tError ierr)
        {
            var xx = new double[2];
            Marshal.Copy(x, xx, 0, xx.Length);
            ierr.errorCode = 0;
            return 2.0 - (xx[1] - xx[0]);
        }

        private static void TestErrorHandling()
        {
            var f = new basicSettings();
            ProbCalc.SetDefaults(ref f);
            f.startMethod = StartMethods.Zero;
            f.methodId = ProbMethod.FORM;
            f.numThreads = 1;
            f.numExtraInt = 50;

            var s = new List<fdistribs>();
            var a = new fdistribs { distId = EnumDistributions.deterministic };
            s.Add(a);
            s.Add(a);

            try
            {
                var r = ProbCalc.Calc(f, s.ToArray(), SimpleZ, progressForm);
            }
            catch (Exception e)
            {
                Comparer.AreEqual(e.Message, "No stochastic parameters found.", "diff in error message.");
            }
        }

        private static void TestErrorHandlingDS()
        {
            var plData = new basicSettings();
            ProbCalc.SetDefaults(ref plData);
            plData.methodId = ProbMethod.DS;

            var s = new List<fdistribs>();
            var a = new fdistribs { distId = EnumDistributions.deterministic };
            s.Add(a);
            s.Add(a);

            try
            {
                var r = ProbCalc.Calc(plData, s.ToArray(), SimpleZ, progressForm);
            }
            catch (Exception e)
            {
                Comparer.AreEqual(e.Message, "No stochastic parameters found.", "diff in error message.");
            }
        }

        private static fdistribs[] getTwoStochasts(bool same)
        {
            var s = new List<fdistribs>();
            var a1 = new fdistribs { distId = EnumDistributions.normal, p4 = new double[] { 1.0, 1.0, 0.0, 0.0 } };
            s.Add(a1);
            if (same)
            {
                s.Add(a1);
            }
            else
            {
                var a2 = new fdistribs { distId = EnumDistributions.deterministic, p4 = new double[] { 3.0, 0.0, 0.0, 0.0 } };
                s.Add(a2);
            }
            return s.ToArray();
        }

        private static void TestDelegateFORM()
        {
            var f = new basicSettings();
            ProbCalc.SetDefaults(ref f);
            f.startMethod = StartMethods.Zero;
            f.methodId = ProbMethod.FORM;
            f.numThreads = 1;
            f.numExtraInt = 50;

            var s = getTwoStochasts(false);

            var i = new List<int>() { 1, 2, 3, 4 };
            var r = ProbCalc.Calc(f, s, new corrStruct[0], SimpleZFunc, progressForm, i.ToArray());
            Comparer.AreEqual(r.beta, 2.0, 1e-6, "TestDelegateFORM: beta");
            Comparer.AreEqual(r.x[0], 3.0, 1e-6, "TestDelegateFORM: x[0]");
            Comparer.AreEqual(r.x[1], 3.0, 1e-6, "TestDelegateFORM: x[1]");
            Comparer.AreEqual(r.convergence, 0, "TestDelegateFORM: conv");
        }

        private static void TestDelegateFORMCorrelated()
        {
            var f = new basicSettings();
            ProbCalc.SetDefaults(ref f);
            f.startMethod = StartMethods.Zero;
            f.methodId = ProbMethod.FORM;
            f.numThreads = 1;
            f.numExtraInt = 50;

            var s = getTwoStochasts(true);

            var correlations = new corrStruct[1];
            correlations[0] = new corrStruct { idx1 = 0, idx2 = 1, correlation = 0.1 };

            var r = ProbCalc.Calc(f, s, correlations, SimpleZFunc, progressForm);
            Comparer.AreEqual(r.beta, 0.0, 1e-6, "TestDelegateFORMCorrelated: beta");
            Comparer.AreEqual(r.x[0], 1.0, 1e-6, "TestDelegateFORMCorrelated: x[0]");
            Comparer.AreEqual(r.x[1], 1.0, 1e-6, "TestDelegateFORMCorrelated: x[1]");
            Comparer.AreEqual(r.convergence, 0, "TestDelegateFORMCorrelated: conv");
        }

        private static void TestDelegateFullyCorrelatedFORM()
        {
            const double betaExpected = 3.0;
            const double eps = 1e-6;

            var f = new basicSettings();
            ProbCalc.SetDefaults(ref f);
            f.startMethod = StartMethods.Zero;
            f.methodId = ProbMethod.FORM;
            f.numThreads = 1;
            f.relaxationFactor = 0.9;
            f.numExtraInt = 50;

            var s = getTwoStochasts(true);
            var correlations = new corrStruct[1];
            correlations[0] = new corrStruct { idx1 = 0, idx2 = 1, correlation = 1.0 };
            f.progressInterval = 1;

            var r = ProbCalc.Calc(f, s, correlations, AnotherSimpleZFunc, progressFormList);

            Comparer.AreEqual(r.stepsNeeded, 5, "number of iterations");
            Comparer.AreEqual(r.beta, betaExpected, eps, "diff in beta");
            Comparer.AreEqual(betas[4], betaExpected, eps, "diff in last Beta");
            Comparer.AreEqual(convIndicators[4], 0.0003, eps, "diff in conv indicator");
        }

        private static void TestConvergenceDataDS()
        {
            const double eps = 1e-6;

            var f = new basicSettings();
            ProbCalc.SetDefaults(ref f);
            f.methodId = ProbMethod.DS;
            f.numThreads = 1;
            f.tolA = 0.1;
            f.tolB = 0.1;
            f.maxSamples = 40000;
            f.du1 = 1;
            f.seed1 = 12345;
            f.seed2 = 12345;

            f.progressInterval = 1;
            var s = getTwoStochasts(true);

            betas.Clear();
            convIndicators.Clear();
            var r = ProbCalc.Calc(f, s, AnotherSimpleZFunc, progressFormList);

            Comparer.AreEqual(10000, r.samplesNeeded, "number of iterations");
            Comparer.AreEqual(0, r.convergence, "Difference in convergence type");
            Comparer.AreEqual(0.089560285, betas[r.samplesNeeded - 1], eps, "diff in last beta");
            Comparer.AreEqual(0.000258902, convIndicators[r.samplesNeeded - 1], eps, "diff in last Z");
        }

        private static void TestFORM_StartVector()
        {
            const double eps = 1e-6;
            var f = new basicSettings();
            ProbCalc.SetDefaults(ref f);
            f.startMethod = StartMethods.GivenVector;
            f.startVector = new double[PbSizes.maxActiveStochast];
            f.startVector[0] = 1.0;
            f.startVector[1] = 2.0;
            f.methodId = ProbMethod.FORM;
            f.numThreads = 1;
            f.relaxationFactor = 0.9;
            f.numExtraInt = 50;

            var s = getTwoStochasts(false);
            f.progressInterval = 1;

            betas.Clear();
            convIndicators.Clear();
            var r = ProbCalc.Calc(f, s, SimpleZFunc, progressFormList);
            Comparer.AreEqual(r.beta, 2.0, eps, "diff in beta");
            Comparer.AreEqual(r.x[0], 3.0, eps, "diff in x[0]");
            Comparer.AreEqual(r.x[1], 3.0, eps, "diff in x[1]");
            Comparer.AreEqual(r.alpha[0], -1.0, eps, "diff in alpha[0]");
            Comparer.AreEqual(r.alpha[1], -0.0, eps, "diff in alpha[1]");
        }

        private static void TestTwoThreads()
        {
            var t1 = new Thread(TestDelegateFORMCorrelated);
            var t2 = new Thread(TestConvergenceDataDS);
            t1.Start();
            t2.Start();
            t1.Join();
            t2.Join();
        }

        private static double MacroZFunc(IntPtr x, int[] i, ref tError ierr)
        {
            var xx = new double[7];
            Marshal.Copy(x, xx, 0, xx.Length);
            ierr.errorCode = 0;
            return xx[0] - xx[6] - 32;
        }

        internal static double ErrorZFunc(IntPtr x, int[] i, ref tError ierr)
        {
            var xx = new double[2];
            Marshal.Copy(x, xx, 0, xx.Length);
            if (xx[0] + xx[1] != 4.0)
            {
                ierr.errorCode = 123;
                ierr.message = "my returned error message";
                return double.NaN;
            }
            else
            {
                return xx[0] - xx[1];
            }
        }

        private static void TestBm3_17()
        {
            const int nStochasts = 7;

            var f = new basicSettings();
            ProbCalc.SetDefaults(ref f);
            f.methodId = ProbMethod.FORM;
            f.numThreads = 1;
            f.numExtraInt = 50;

            var probStochasts = new List<fdistribs>();
            for (int k = 0; k < nStochasts; k++)
            {
                var s = new fdistribs();
                switch (k)
                {
                    case 0:
                        s.distId = EnumDistributions.normal;
                        s.p4 = new double[4] { 40, 4, 0, 0 };
                        break;
                    case 6:
                        s.distId = EnumDistributions.deterministic;
                        s.p4 = new double[4] { 1, 0, 0, 0 };
                        break;
                }
                probStochasts.Add(s);
            }

            var r = ProbCalc.Calc(f, probStochasts.ToArray(), MacroZFunc, progressFormList);
            Comparer.AreEqual(r.beta, 1.75, 1e-6, "diff in beta");
            Comparer.AreEqual(r.convergence, 0, "diff in conv");
        }

        private static void TestErrorHandlingZFunc()
        {
            var f = new basicSettings();
            ProbCalc.SetDefaults(ref f);
            f.methodId = ProbMethod.FORM;
            f.numExtraInt = 50;
            for (int i = 1; i <= 4; i++)
            {
                f.numThreads = i;
                var stoch = getTwoStochasts(false);
                var r = ProbCalc.Calc(f, stoch, ErrorZFunc, progressFormList);
                Comparer.AreEqual(r.convergence, 1, "TestErrorHandlingZFunc: conv. flag");
            }
        }

        private static void TestErrorHandlingZFuncDS()
        {
            var f = new basicSettings();
            ProbCalc.SetDefaults(ref f);
            f.methodId = ProbMethod.DS;
            for (int i = 1; i <= 4; i++)
            {
                f.numThreads = i;
                var stoch = getTwoStochasts(false);
                try
                {
                    var r = ProbCalc.Calc(f, stoch, ErrorZFunc, progressFormList);
                    Comparer.AreEqual(r.convergence, 1, "TestErrorHandlingZFunc: conv. flag");
                }
                catch (Exception e)
                {
                    Console.WriteLine(e.Message);
                }
            }
        }
        private static void TestErrorHandlingZFuncNumInt()
        {
            var f = new basicSettings();
            ProbCalc.SetDefaults(ref f);
            f.methodId = ProbMethod.NI;
            for (int i = 1; i <= 4; i++)
            {
                f.numThreads = i;
                var stoch = getTwoStochasts(false);
                try
                {
                    var r = ProbCalc.Calc(f, stoch, ErrorZFunc, progressFormList);
                    Comparer.AreEqual(r.convergence, 1, "TestErrorHandlingZFunc: conv. flag");
                }
                catch (Exception e)
                {
                    Console.WriteLine(e.Message);
                }
            }
        }

        private static void BasicTestIS()
        {
            var f = new basicSettings();
            ProbCalc.SetDefaults(ref f);
            f.methodId = ProbMethod.IMHR;
            f.maxSamples = 40000;
            f.seed1 = 1234;
            f.seed2 = 1234;
            f.tolA = 0.01;
            f.tolB = f.tolA;
            f.varianceFactor = 1.1;
            f.numThreads = 1;

            var s1 = new fdistribs { distId = EnumDistributions.normal, p4 = new double[4] { 1.0, 1.0, 0, 0 } };
            var s2 = new fdistribs { distId = EnumDistributions.normal, p4 = new double[4] { 2.0, 0.5, 0, 0 } };
            var s = new List<fdistribs> { s1, s2 };

            var x = new double[2];
            var r = ProbCalc.Calc(f, s.ToArray(), SimpleZFunc, progressFormList);
            Comparer.AreEqual(1, r.convergence, "diff in convergence flag");
            //Comparer.AreEqual(1.118570, r.accuracy1, 1e-4, "diff in convergence type 1");
            //Comparer.AreEqual(0.312271, r.accuracy2, 1e-4, "diff in convergence type 2");
            Comparer.AreEqual(40000, r.samplesNeeded, "diff in number of steps");
            Comparer.AreEqual(r.beta, 0.885380, 1e-5);
            Comparer.AreEqual(r.alpha[0], -0.89126, 1e-5);
            Comparer.AreEqual(r.alpha[1], 0.453487, 1e-5);
            Comparer.AreEqual(r.x[0], 1.79764, 1e-5);
            Comparer.AreEqual(r.x[1], 1.79707, 1e-5);
        }
    }
}
