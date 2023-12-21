using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using Deltares.ProbabilisticCPP.Wrapper;

namespace Probabilistic.Tests
{
    public class TestCorrelation
    {
        public static void runAllCorrelationTests()
        {
            TestDelegateFullyCorrelatedFORM2();
            TestCorrelatedFractionsCase1();
            TestCorrelatedFractionsCase2();
            TestCorrelatedFractionsCase3();
            TestCorrelatedFractionsCase4();
            TestCorrelatedFractionsCase5();
            TestProbCompFullyCorrelatedCase1();
            TestProbCompFullyCorrelatedCase2();
            TestProbCompFullyCorrelatedCase3();
            TestProbCompFullyCorrelatedCase4();
            TestProbCompFullyCorrelatedCase5();
            TestProbCompFullyCorrelatedCase6();
            TestProbCompFullyCorrelatedCase7();
            TestProbCompFullyCorrelatedCase8();
            TestProbCompFullyCorrelatedCase9();
            TestProbCompFullyCorrelatedCase10();
            TestProbCompMixedCorrelatedCase1();
            TestProbCompMixedCorrelatedCase2();
            TestProbCompMixedCorrelatedCase3();
            TestProbCompMixedCorrelatedCase4();
            TestProbCompMixedCorrelatedCase5();
        }

        internal static double AnotherSimpleZFunc_3(IntPtr x, int[] i, ref tError ierr)
        {
            var xx = new double[6];
            Marshal.Copy(x, xx, 0, xx.Length);
            ierr.errorCode = 0;
            var z = 1.0 + xx[1] - 0.5 * xx[0] + 0.75 * xx[2] - xx[3] + xx[4] - xx[5];
            return z;
        }

        private static basicSettings GetBasicSettings(DPoptions dp=DPoptions.RMin)
        {
            var f = new basicSettings();
            ProbCalc.SetDefaults(ref f);
            f.startMethod = StartMethods.Zero;
            f.methodId = ProbMethod.FORM;
            f.relaxationFactor = 0.9;
            f.numThreads = 1;
            f.designPointOptions = dp;
            return f;
        }

        private static void TestDelegateFullyCorrelatedFORM2()
        {   //
            // This test case shows the effect of the order in which the correlations are given.
            // It does not happen with a smaller number of stochasts.
            // NB: behaviour is not fully understood, and may change in the future.
            //
            const double betaExpected = 2.14662525839980;

            for (int i = 0; i < 2; i++)
            {
                var w = new List<corrStruct>();
                if (i == 0)
                {
                    w.Add(new corrStruct() { idx1=2, idx2= 1, correlation = -1.0 });
                }
                else
                {
                    w.Add(new corrStruct() { idx1 = 1, idx2 = 2, correlation = -1.0 });
                }
                w.Add(new corrStruct() { idx1 = 1, idx2 = 0, correlation = 1.0 });
                w.Add(new corrStruct() { idx1 = 3, idx2 = 2, correlation = -1.0 });
                w.Add(new corrStruct() { idx1 = 3, idx2 = 4, correlation = 1.0 });
                var c = new Correlations();
                c.ExtendFullyCorrelated(ref w);
                var sds = GetStochasts();

                var f = GetBasicSettings();
                var ii = new List<int>() { 1, 2, 3, 4 };
                var r = ProbCalc.Calc(f, sds, w.ToArray(), AnotherSimpleZFunc_3, ProbLibWrapperTests.progressForm, ii.ToArray());

                Comparer.AreEqual(r.beta, betaExpected, 1e-7);
            }
        }

        private static fdistribs[] GetStochasts()
        {
            var s1 = new fdistribs { distId = EnumDistributions.normal, p4 = new double[] { 1.0, 1.0, 0.0, 0.0 } };
            var s2 = new fdistribs { distId = EnumDistributions.normal, p4 = new double[] { 0.5, 2.0, 0.0, 0.0 } };
            var s3 = new fdistribs { distId = EnumDistributions.normal, p4 = new double[] { 2.0, 0.5, 0.0, 0.0 } };
            var s4 = new fdistribs { distId = EnumDistributions.normal, p4 = new double[] { 1.1, 1.5, 0.0, 0.0 } };
            var s5 = new fdistribs { distId = EnumDistributions.normal, p4 = new double[] { 0.7, 2.5, 0.0, 0.0 } };
            var s6 = new fdistribs { distId = EnumDistributions.normal, p4 = new double[] { 0.3, 0.75, 0.0, 0.0 } };
            var sds = new fdistribs[] { s1, s2, s3, s4, s5, s6 };
            return sds;
        }

        private static List<corrStruct> FillCorrelationPairs(double[,] rho, bool extend=false)
        {
            var w = new List<corrStruct>();
            var dim = rho.GetUpperBound(1) + 1;
            for (int kk = 0; kk < dim; kk++)

            for (int ll = 0; ll < kk; ll++)
            {
                var correlation = rho[kk, ll];
                if (Math.Abs(correlation) > double.Epsilon)
                    w.Add(new corrStruct(){idx1 = kk, idx2 = ll, correlation = correlation });
            }

            if (extend)
            {
                var c = new Correlations();
                c.ExtendFullyCorrelated(ref w);
            }

            return w;
        }

        private static void TestCorrelatedFractionsCase1()
        {
            double[,] rho =
            {
                {1, 0, 0, 0, 0, 0},
                {0, 1, 0, 0, 0, 0},
                {0, 0, 1, 0, 0, 0},
                {0, 0, 0, 1, 0, 0},
                {0, 0, 0, 0, 1, 0},
                {0, 0, 0, 0, 0, 1}
            };

            var w = FillCorrelationPairs(rho);
            var sds = GetStochasts();
            var f = GetBasicSettings();
            var ii = new List<int>() { -999 };
            var r = ProbCalc.Calc(f, sds, w.ToArray(), AnotherSimpleZFunc_3, ProbLibWrapperTests.progressForm, ii.ToArray());

            Comparer.AreEqual(r.beta, 0.491, 1e-3);
            var a = (new List<double>(r.alpha)).GetRange(0,6);
            var refAlpha = new List<double>{ -0.136, 0.545, 0.102, -0.409, 0.682, -0.204 };
            Comparer.AreEqual(a, refAlpha, 1e-3, "TestCorrelatedFractionsCase1: diff in alpha");
            // equal to results from Probabilistic Toolkit
        }

        private static void TestCorrelatedFractionsCase2()
        {
            double[,] rho =
            {
                {1,  -0.85,   -0.78,   0.68,    -0.87,   0.42,},
                {-0.85,   1,   0.79,    -0.71,   0.89,    -0.43},
                {-0.78,   0.79,    1,   -0.45,   0.66,    -0.71},
                {0.68,    -0.71,   -0.45,   1,   -0.71 ,  0.09},
                {-0.87,   0.89,    0.66,    -0.71,   1,   -0.17},
                {0.42,    -0.43,   -0.71,   0.09,    -0.17,   1}
            };

            var w = FillCorrelationPairs(rho);
            var sds = GetStochasts();
            var f = GetBasicSettings(DPoptions.MultiplyInvCholesky);
            var ii = new List<int>() { -999 };
            var r = ProbCalc.Calc(f, sds, w.ToArray(), AnotherSimpleZFunc_3, ProbLibWrapperTests.progressForm, ii.ToArray());

            Comparer.AreEqual(r.beta, 0.275, 1e-3);
            var a = (new List<double>(r.alpha)).GetRange(0, 6);
            var refAlpha = new List<double> { -0.916, 0.967, 0.794, -0.805, 0.940, -0.404 };
            Comparer.AreEqual(a, refAlpha, 1e-3, "TestCorrelatedFractionsCase2: diff in alpha");

            var a2 = (new List<double>(r.alpha2)).GetRange(0, 6);
            var refAlpha2 = new List<double> { -0.916, 0.357, -0.011, -0.139, 0.098, -0.060 };
            Comparer.AreEqual(a2, refAlpha2, 1e-3, "TestCorrelatedFractionsCase2: diff in alpha2");
            // equal to results from Probabilistic Toolkit
        }

        private static void TestCorrelatedFractionsCase3()
        {
            double[,] rho =
            {
                {1,  0.1, -0.2608, 0.7539,  0.014,   0.2256},
                {0.1, 1,   0.0572,  0.7196,  -0.2814, 0.2407},
                {-0.2608, 0.0572,  1,   -0.1409, 0.3473,  0.0741},
                {0.7539,  0.7196,  -0.1409, 1,   -0.1729, 0.3714},
                {0.014,   -0.2814, 0.3473,  -0.1729, 1,   -0.0581},
                {0.2256,  0.2407,  0.0741,  0.3714,  -0.0581, 1}
            };

            var w = FillCorrelationPairs(rho);
            var sds = GetStochasts();
            var f = GetBasicSettings(DPoptions.MultiplyInvCholesky);
            var ii = new List<int>() { -999 };
            var r = ProbCalc.Calc(f, sds, w.ToArray(), AnotherSimpleZFunc_3, ProbLibWrapperTests.progressForm, ii.ToArray());

            Comparer.AreEqual(r.beta, 0.57, 1e-2);
            var a = (new List<double>(r.alpha)).GetRange(0, 6);
            var refAlpha = new List<double> { -0.527, 0.003, 0.521, -0.381, 0.749, -0.334 };
            Comparer.AreEqual(a, refAlpha, 1e-3, "TestCorrelatedFractionsCase3: diff in alpha");

            var a2 = (new List<double>(r.alpha2)).GetRange(0, 6);
            var refAlpha2 = new List<double> { -0.527, 0.055, 0.394, -0.179, 0.706, -0.184 };
            Comparer.AreEqual(a2, refAlpha2, 1e-3, "TestCorrelatedFractionsCase3: diff in alpha2");

            // equal to results from Probabilistic Toolkit
        }

        private static void TestCorrelatedFractionsCase4()
        {
            double[,] rho =
            {
                {1,  0.0098,  -0.3539, 0.6517,  0,   0.2202},
                {0.0098,  1,   0.043,   0.6971,  -0.3118, 0.2303},
                {-0.3539, 0.043,   1,   -0.2143, 0.3982,  0.1238},
                {0.6517,  0.6971,  -0.2143, 1,   -0.1577, 0.3772},
                {0,   -0.3118, 0.3982,  -0.1577, 1,   -0.0125},
                {0.2202,  0.2303,  0.1238,  0.3772,  -0.0125, 1}
            };

            var w = FillCorrelationPairs(rho);
            var sds = GetStochasts();
            var f = GetBasicSettings(DPoptions.MultiplyInvCholesky);
            var ii = new List<int>() { -999 };
            var r = ProbCalc.Calc(f, sds, w.ToArray(), AnotherSimpleZFunc_3, ProbLibWrapperTests.progressForm, ii.ToArray());

            Comparer.AreEqual(r.beta, 0.576, 1e-3);
            var a = (new List<double>(r.alpha)).GetRange(0, 6);
            var refAlpha = new List<double> { -0.561, 0.004, 0.595, -0.380, 0.726, -0.304 };
            Comparer.AreEqual(a, refAlpha, 1e-3, "TestCorrelatedFractionsCase4: diff in alpha");

            var a2 = (new List<double>(r.alpha2)).GetRange(0, 6);
            var refAlpha2 = new List<double> { -0.561, 0.010, 0.424, -0.045, 0.677, -0.212 };
            Comparer.AreEqual(a2, refAlpha2, 1e-3, "TestCorrelatedFractionsCase4: diff in alpha2");
            // equal to results from Probabilistic Toolkit
        }

        private static void TestCorrelatedFractionsCase5()
        {
            double[,] rho =
            {
                {1, 0.8, 0.7, 0.7, 0, 0.8},
                {0.8, 1, 0.8, 0.8, 0, 0.8},
                {0.7, 0.8, 1, 0.7, 0, 0.7},
                {0.7, 0.8, 0.7, 1, 0, 0.7},
                {0, 0, 0, 0, 1, 0.3},
                {0.8, 0.8, 0.7, 0.7, 0.3, 1}
            };

            var w = FillCorrelationPairs(rho);
            var sds = GetStochasts();
            var f = GetBasicSettings(DPoptions.MultiplyInvCholesky);
            var ii = new List<int>() { -999 };
            var r = ProbCalc.Calc(f, sds, w.ToArray(), AnotherSimpleZFunc_3, ProbLibWrapperTests.progressForm, ii.ToArray());

            Comparer.AreEqual(r.beta, 0.706, 1e-3);
            var a = (new List<double>(r.alpha)).GetRange(0, 6);
            var refAlpha = new List<double> { -0.113, 0.039, 0.020, -0.201, 0.892, 0.162 };
            Comparer.AreEqual(a, refAlpha, 1e-3, "TestCorrelatedFractionsCase5: diff in alpha");

            var a2 = (new List<double>(r.alpha2)).GetRange(0, 6);
            var refAlpha2 = new List<double> { -0.113, 0.216, 0.021, -0.359, 0.892, -0.129 };
            Comparer.AreEqual(a2, refAlpha2, 1e-3, "TestCorrelatedFractionsCase5: diff in alpha2");
            // equal to results from Probabilistic Toolkit
        }

        private static void TestProbCompFullyCorrelatedCase1()
        {
            double[,] rho =
            {
                { 1,   1,   0,   0,   0,   0},
                { 1,   1,   0,   0,   0,   0},
                { 0,   0,   1,   0,   0,   0},
                { 0,   0,   0,   1,   0,   0},
                { 0,   0,   0,   0,   1,   0},
                { 0,   0,   0,   0,   0,   1}
            };

            var w = FillCorrelationPairs(rho, true);
            var sds = GetStochasts();
            var f = GetBasicSettings(DPoptions.MultiplyInvCholesky);
            var ii = new List<int>() { -999 };
            var r = ProbCalc.Calc(f, sds, w.ToArray(), AnotherSimpleZFunc_3, ProbLibWrapperTests.progressForm, ii.ToArray());

            Comparer.AreEqual(r.beta, 0.532, 1e-3);
            var a = (new List<double>(r.alpha)).GetRange(0, 6);
            var refAlpha = new List<double> { 0.443, 0.443, 0.111, -0.443, 0.739, -0.222 };
            Comparer.AreEqual(a, refAlpha, 1e-3, "TestProbCompFullyCorrelatedCase1: diff in alpha");

            var a2 = (new List<double>(r.alpha2)).GetRange(0, 6);
            var refAlpha2 = new List<double> { 0.443, 0.0, 0.111, -0.443, 0.739, -0.222 };
            Comparer.AreEqual(a2, refAlpha2, 1e-3, "TestProbCompFullyCorrelatedCase1: diff in alpha2");
            // equal to results from Probabilistic Toolkit
        }

        private static void TestProbCompFullyCorrelatedCase2()
        {
            double[,] rho =
            {
                { 1,   1,   1,   0,   0,   0},
                { 1,   1,   1,   0,   0,   0},
                { 1,   1,   1,   0,   0,   0},
                { 0,   0,   0,   1,   0,   0},
                { 0,   0,   0,   0,   1,   0},
                { 0,   0,   0,   0,   0,   1}
            };

            var w = FillCorrelationPairs(rho, true);
            var sds = GetStochasts();
            var f = GetBasicSettings(DPoptions.MultiplyInvCholesky);
            var ii = new List<int>() { -999 };
            var r = ProbCalc.Calc(f, sds, w.ToArray(), AnotherSimpleZFunc_3, ProbLibWrapperTests.progressForm, ii.ToArray());

            Comparer.AreEqual(r.beta, 0.508, 1e-3);
            var a = (new List<double>(r.alpha)).GetRange(0, 6);
            var refAlpha = new List<double> { 0.529, 0.529, 0.529, -0.423, 0.705, -0.211 };
            Comparer.AreEqual(a, refAlpha, 1e-3, "TestProbCompFullyCorrelatedCase2: diff in alpha");

            var a2 = (new List<double>(r.alpha2)).GetRange(0, 6);
            var refAlpha2 = new List<double> { 0.529, 0.0, 0.0, -0.423, 0.705, -0.211 };
            Comparer.AreEqual(a2, refAlpha2, 1e-3, "TestProbCompFullyCorrelatedCase2: diff in alpha2");
            // equal to results from Probabilistic Toolkit
        }

        private static void TestProbCompFullyCorrelatedCase3()
        {
            double[,] rho =
            {
                { 1,   1,   1,   1,   0,   0},
                { 1,   1,   1,   1,   0,   0},
                { 1,   1,   1,   1,   0,   0},
                { 1,   1,   1,   1,   0,   0},
                { 0,   0,   0,   0,   1,   0},
                { 0,   0,   0,   0,   0,   1}
            };

            var w = FillCorrelationPairs(rho, true);
            var sds = GetStochasts();
            var f = GetBasicSettings(DPoptions.MultiplyInvCholesky);
            var ii = new List<int>() { -999 };
            var r = ProbCalc.Calc(f, sds, w.ToArray(), AnotherSimpleZFunc_3, ProbLibWrapperTests.progressForm, ii.ToArray());

            Comparer.AreEqual(r.beta, 0.683, 1e-3);
            var a = (new List<double>(r.alpha)).GetRange(0, 6);
            var refAlpha = new List<double> { 0.142, 0.142, 0.142, 0.142, 0.948, -0.284 };
            Comparer.AreEqual(a, refAlpha, 1e-3, "TestProbCompFullyCorrelatedCase3: diff in alpha");
            var a2 = (new List<double>(r.alpha2)).GetRange(0, 6);
            var refAlpha2 = new List<double> { 0.142, 0.0, 0.0, 0.0, 0.948, -0.284 };
            Comparer.AreEqual(a2, refAlpha2, 1e-3, "TestProbCompFullyCorrelatedCase3: diff in alpha2");
            // equal to results from Probabilistic Toolkit
        }

        private static void TestProbCompFullyCorrelatedCase4()
        {
            double[,] rho =
            {
                { 1,   1,   1,   1,   1,   0},
                { 1,   1,   1,   1,   1,   0},
                { 1,   1,   1,   1,   1,   0},
                { 1,   1,   1,   1,   1,   0},
                { 1,   1,   1,   1,   1,   0},
                { 0,   0,   0,   0,   0,   1}
            };

            var w = FillCorrelationPairs(rho, true);
            var sds = GetStochasts();
            var f = GetBasicSettings(DPoptions.MultiplyInvCholesky);
            var ii = new List<int>() { -999 };
            var r = ProbCalc.Calc(f, sds, w.ToArray(), AnotherSimpleZFunc_3, ProbLibWrapperTests.progressForm, ii.ToArray());

            Comparer.AreEqual(r.beta, 0.606, 1e-3);
            Comparer.AreEqual(r.alpha[0], 0.968, 1e-3);
            Comparer.AreEqual(r.alpha[5], -0.252, 1e-3);
            Comparer.AreEqual(r.alpha2[0], 0.968, 1e-3);
            Comparer.AreEqual(r.alpha2[5], -0.252, 1e-3);
            // equal to results from Probabilistic Toolkit
        }

        private static void TestProbCompFullyCorrelatedCase5()
        {
            double[,] rho =
            {
                { 1,   1,   1,   1,   1,   1},
                { 1,   1,   1,   1,   1,   1},
                { 1,   1,   1,   1,   1,   1},
                { 1,   1,   1,   1,   1,   1},
                { 1,   1,   1,   1,   1,   1},
                { 1,   1,   1,   1,   1,   1}
            };

            var w = FillCorrelationPairs(rho, true);
            var sds = GetStochasts();
            var f = GetBasicSettings(DPoptions.MultiplyInvCholesky);
            var ii = new List<int>() { -999 };
            var r = ProbCalc.Calc(f, sds, w.ToArray(), AnotherSimpleZFunc_3, ProbLibWrapperTests.progressForm, ii.ToArray());

            Comparer.AreEqual(r.beta, 0.847, 1e-3);
            Comparer.AreEqual(r.alpha[0], 1.0, 1e-9);
            Comparer.AreEqual(r.alpha2[0], 1.0, 1e-9);
            // equal to results from Probabilistic Toolkit
        }

        private static void TestProbCompFullyCorrelatedCase6()
        {
            double[,] rho =
            {
                { 1,   0,   0,   0,   0,   0},
                { 0,   1,   0,   0,   0,   0},
                { 0,   0,   1,   1,   1,   0},
                { 0,   0,   1,   1,   1,   0},
                { 0,   0,   1,   1,   1,   0},
                { 0,   0,   0,   0,   0,   1}
            };

            var w = FillCorrelationPairs(rho, true);
            var sds = GetStochasts();
            var f = GetBasicSettings(DPoptions.MultiplyInvCholesky);
            var ii = new List<int>() { -999 };
            var r = ProbCalc.Calc(f, sds, w.ToArray(), AnotherSimpleZFunc_3, ProbLibWrapperTests.progressForm, ii.ToArray());

            Comparer.AreEqual(r.beta, 0.695, 1e-3);
            Comparer.AreEqual(r.alpha[0], -0.193, 1e-3);
            Comparer.AreEqual(r.alpha[1], 0.772, 1e-3);
            Comparer.AreEqual(r.alpha[2], 0.531, 1e-3);
            Comparer.AreEqual(r.alpha[5], -0.290, 1e-3);
            Comparer.AreEqual(r.alpha2[0], -0.193, 1e-3);
            Comparer.AreEqual(r.alpha2[1], 0.772, 1e-3);
            Comparer.AreEqual(r.alpha2[2], 0.531, 1e-3);
            Comparer.AreEqual(r.alpha2[5], -0.29, 1e-3);
            // equal to results from Probabilistic Toolkit
        }

        private static void TestProbCompFullyCorrelatedCase7()
        {
            double[,] rho =
            {
                { 1,  -1,   0,   0,   0,   0},
                {-1,   1,   0,   0,   0,   0},
                { 0,   0,   1,   0,   0,   0},
                { 0,   0,   0,   1,   0,   0},
                { 0,   0,   0,   0,   1,   0},
                { 0,   0,   0,   0,   0,   1}
            };

            var w = FillCorrelationPairs(rho, true);
            var sds = GetStochasts();
            var f = GetBasicSettings(DPoptions.MultiplyInvCholesky);
            var ii = new List<int>() { -999 };
            var r = ProbCalc.Calc(f, sds, w.ToArray(), AnotherSimpleZFunc_3, ProbLibWrapperTests.progressForm, ii.ToArray());

            Comparer.AreEqual(r.beta, 0.458, 1e-3);
            Comparer.AreEqual(r.alpha[0], -0.636, 1e-3);
            Comparer.AreEqual(r.alpha[2], 0.095, 1e-3);
            Comparer.AreEqual(r.alpha[3], -0.382, 1e-3);
            Comparer.AreEqual(r.alpha[4], 0.636, 1e-3);
            Comparer.AreEqual(r.alpha[5], -0.191, 1e-3);
            Comparer.AreEqual(r.alpha2[0], -0.636, 1e-3);
            Comparer.AreEqual(r.alpha2[2], 0.095, 1e-3);
            Comparer.AreEqual(r.alpha2[3], -0.382, 1e-3);
            Comparer.AreEqual(r.alpha2[4], 0.636, 1e-3);
            Comparer.AreEqual(r.alpha2[5], -0.191, 1e-3);
            // equal to results from Probabilistic Toolkit
        }

        private static void TestProbCompFullyCorrelatedCase8()
        {
            double[,] rho =
            {
                { 1,  -1,  -1,   0,   0,   0},
                {-1,   1,   1,   0,   0,   0},
                {-1,   1,   1,   0,   0,   0},
                { 0,   0,   0,   1,   0,   0},
                { 0,   0,   0,   0,   1,   0},
                { 0,   0,   0,   0,   0,   1}
            };

            var w = FillCorrelationPairs(rho, true);
            var sds = GetStochasts();
            var f = GetBasicSettings(DPoptions.MultiplyInvCholesky);
            var ii = new List<int>() { -999 };
            var r = ProbCalc.Calc(f, sds, w.ToArray(), AnotherSimpleZFunc_3, ProbLibWrapperTests.progressForm, ii.ToArray());

            Comparer.AreEqual(r.beta, 0.432, 1e-3);
            Comparer.AreEqual(r.alpha[0], -0.691, 1e-3);
            Comparer.AreEqual(r.alpha[3], -0.360, 1e-3);
            Comparer.AreEqual(r.alpha[4], 0.601, 1e-3);
            Comparer.AreEqual(r.alpha[5], -0.180, 1e-3);
            Comparer.AreEqual(r.alpha2[0], -0.691, 1e-3);
            Comparer.AreEqual(r.alpha2[3], -0.36, 1e-3);
            Comparer.AreEqual(r.alpha2[4], 0.601, 1e-3);
            Comparer.AreEqual(r.alpha2[5], -0.18, 1e-3);
            // equal to results from Probabilistic Toolkit
        }

        private static void TestProbCompFullyCorrelatedCase9()
        {
            double[,] rho =
            {
                { 1,   0,   0,   0,   0,   0},
                { 0,   1,   0,   0,   0,   0},
                { 0,   0,   1,  -1,  -1,   0},
                { 0,   0,  -1,   1,   1,   0},
                { 0,   0,  -1,   1,   1,   0},
                { 0,   0,   0,   0,   0,   1}
            };

            var w = FillCorrelationPairs(rho, true);
            var sds = GetStochasts();
            var f = GetBasicSettings(DPoptions.MultiplyInvCholesky);
            var ii = new List<int>() { -999 };
            var r = ProbCalc.Calc(f, sds, w.ToArray(), AnotherSimpleZFunc_3, ProbLibWrapperTests.progressForm, ii.ToArray());

            Comparer.AreEqual(r.beta, 0.789, 1e-3);
            var a = (new List<double>(r.alpha)).GetRange(0, 6);
            var refAlpha = new List<double> { -0.219, 0.877, -0.274, 0.274, 0.274, -0.329 };
            Comparer.AreEqual(a, refAlpha, 1e-3, "TestProbCompFullyCorrelatedCase9: diff in alpha");
            var a2 = (new List<double>(r.alpha2)).GetRange(0, 6);
            var refAlpha2 = new List<double> { -0.219, 0.877, -0.274, 0.0, 0.0, -0.329 };
            Comparer.AreEqual(a2, refAlpha2, 1e-3, "TestProbCompFullyCorrelatedCase9: diff in alpha2");
            // equal to results from Probabilistic Toolkit
        }

        private static void TestProbCompFullyCorrelatedCase10()
        {
            double[,] rho =
            {
                { 1,  -1,  -1,   1,   0,   0},
                {-1,   1,   1,  -1,   0,   0},
                {-1,   1,   1,  -1,   0,   0},
                { 1,  -1,  -1,   1,   0,   0},
                { 0,   0,   0,   0,   1,   0},
                { 0,   0,   0,   0,   0,   1}
            };

            var w = FillCorrelationPairs(rho, true);
            var sds = GetStochasts();
            var f = GetBasicSettings(DPoptions.MultiplyInvCholesky);
            var ii = new List<int>() { -999 };
            var r = ProbCalc.Calc(f, sds, w.ToArray(), AnotherSimpleZFunc_3, ProbLibWrapperTests.progressForm, ii.ToArray());

            Comparer.AreEqual(r.beta, 0.353, 1e-3);
            var a = (new List<double>(r.alpha)).GetRange(0, 6);
            var refAlpha = new List<double> { -0.859, 0.859, 0.859, -0.859, 0.491, -0.147 };
            Comparer.AreEqual(a, refAlpha, 1e-3, "TestProbCompFullyCorrelatedCase10: diff in alpha");

            var a2 = (new List<double>(r.alpha2)).GetRange(0, 6);
            var refAlpha2 = new List<double> { -0.859, 0.0, 0.0, 0.0, 0.491, -0.147 };
            Comparer.AreEqual(a2, refAlpha2, 1e-3, "TestProbCompFullyCorrelatedCase10: diff in alpha2");
            // equal to results from Probabilistic Toolkit
        }

        private static void TestProbCompMixedCorrelatedCase1()
        {
            double[,] rho =
            {
                { 1,   1,   1,   0.6517,  0,   0.2202 },
                { 1,   1,   1,   0.6971,  -0.3118, 0.2303 },
                { 1,   1,   1,   -0.2143, 0.3982,  0.1238 },
                { 0.6517,  0.6971,  -0.2143, 1,   -0.1577, 0.3772 },
                { 0,   -0.3118, 0.3982,  -0.1577, 1,   -0.0125 },
                { 0.2202,  0.2303,  0.1238,  0.3772,  -0.0125, 1 }
            };

            var w = FillCorrelationPairs(rho, true);
            var sds = GetStochasts();
            var f = GetBasicSettings(DPoptions.MultiplyInvCholesky);
            var ii = new List<int>() { -999 };
            var r = ProbCalc.Calc(f, sds, w.ToArray(), AnotherSimpleZFunc_3, ProbLibWrapperTests.progressForm, ii.ToArray());

            Comparer.AreEqual(r.beta, 0.559, 1e-3);
            var a = (new List<double>(r.alpha)).GetRange(0, 6);
            var refAlpha = new List<double> { 0.227, 0.227, 0.227, -0.297, 0.853, -0.290 };
            Comparer.AreEqual(a, refAlpha, 1e-3, "TestProbCompMixedCorrelatedCase1: diff in alpha");

            var a2 = (new List<double>(r.alpha2)).GetRange(0, 6);
            var refAlpha2 = new List<double> { 0.227, 0.0, 0.0, -0.586, 0.747, -0.215 };
            Comparer.AreEqual(a2, refAlpha2, 1e-3, "TestProbCompMixedCorrelatedCase1: diff in alpha2");
            // equal to results from Probabilistic Toolkit
        }

        private static void TestProbCompMixedCorrelatedCase2()
        {
            double[,] rho =
            {
                { 1,   0.8, 0.7, 0.7, 0,  0.8 },
                { 0.8, 1,   0.8, 0.8, 0,   0.8 },
                { 0.7, 0.8, 1,   0.7, 0,   0.7 },
                { 0.7, 0.8, 0.7, 1,   1,   0.7 },
                { 0,   0,   0,   1,   1,   0.3 },
                { 0.8, 0.8, 0.7, 0.7, 0.3, 1 }
            };

            var w = FillCorrelationPairs(rho, true);
            var sds = GetStochasts();
            var f = GetBasicSettings(DPoptions.MultiplyInvCholesky);
            var ii = new List<int>() { -999 };
            var r = ProbCalc.Calc(f, sds, w.ToArray(), AnotherSimpleZFunc_3, ProbLibWrapperTests.progressForm, ii.ToArray());

            Comparer.AreEqual(r.beta, 0.799, 1e-3);
            var a = (new List<double>(r.alpha)).GetRange(0, 6);
            var refAlpha = new List<double> { 0.649, 0.932, 0.799, 0.883, 0.883, 0.627 };
            Comparer.AreEqual(a, refAlpha, 1e-3, "TestProbCompMixedCorrelatedCase2: diff in alpha");
            var a2 = (new List<double>(r.alpha2)).GetRange(0, 6);
            var refAlpha2 = new List<double> { 0.649, 0.688, 0.117, 0.244, 0.0, -0.177 };
            Comparer.AreEqual(a, refAlpha, 1e-3, "TestProbCompMixedCorrelatedCase2: diff in alpha2");
            // equal to results from Probabilistic Toolkit
        }

        private static void TestProbCompMixedCorrelatedCase3()
        {
            double[,] rho =
            {
                { 1,   0.8, 0.7, 0.7, 0,   0.8 },
                { 0.8, 1,   0.8, 0.8, 0,   0.8 },
                { 0.7, 0.8, 1,   0.7, 0,   0.7 },
                { 0.7, 0.8, 0.7, 1,   1,   1 },
                { 0,   0,   0,   1,   1,   1 },
                { 0.8, 0.8, 0.7, 1,   1,   1 }
            };

            var w = FillCorrelationPairs(rho, true);
            var sds = GetStochasts();
            var f = GetBasicSettings(DPoptions.MultiplyInvCholesky);
            var ii = new List<int>() { -999 };
            var r = ProbCalc.Calc(f, sds, w.ToArray(), AnotherSimpleZFunc_3, ProbLibWrapperTests.progressForm, ii.ToArray());

            Comparer.AreEqual(r.beta, 0.844, 1e-3);
            var a = (new List<double>(r.alpha)).GetRange(0, 6);
            var refAlpha = new List<double> { 0.721, 0.985, 0.844, 0.827, 0.827, 0.827 };
            Comparer.AreEqual(a, refAlpha, 1e-3, "TestProbCompMixedCorrelatedCase3: diff in alpha");

            var a2 = (new List<double>(r.alpha2)).GetRange(0, 6);
            var refAlpha2 = new List<double> { 0.721, 0.680, 0.114, 0.069, 0.0, 0.0 };
            Comparer.AreEqual(a2, refAlpha2, 1e-3, "TestProbCompMixedCorrelatedCase3: diff in alpha");
            // equal to results from Probabilistic Toolkit
        }

        private static void TestProbCompMixedCorrelatedCase4()
        {
            double[,] rho =
            {
                { 1,   0.8, 0.7, 0.7, 0,   0.8 },
                { 0.8, 1,   0.8, 0.8, 0,   0.8 },
                { 0.7, 0.8, 1,   0.7, 0,   0.7 },
                { 0.7, 0.8, 0.7, 1,   -1,  -1 },
                { 0,   0,   0,   -1,  1,   1 },
                { 0.8, 0.8, 0.7, -1,  1,   1 }
            };

            var w = FillCorrelationPairs(rho, true);
            var sds = GetStochasts();
            var f = GetBasicSettings(DPoptions.MultiplyInvCholesky);
            var ii = new List<int>() { -999 };
            var r = ProbCalc.Calc(f, sds, w.ToArray(), AnotherSimpleZFunc_3, ProbLibWrapperTests.progressForm, ii.ToArray());

            Comparer.AreEqual(r.beta, 0.852, 1e-3);
            var a = (new List<double>(r.alpha)).GetRange(0, 6);
            var refAlpha = new List<double> { -0.432, -0.331, -0.308, -0.823, 0.823, 0.823 };
            Comparer.AreEqual(a, refAlpha, 1e-3, "TestProbCompMixedCorrelatedCase4: diff in alpha");

            var a2 = (new List<double>(r.alpha2)).GetRange(0, 6);
            var refAlpha2 = new List<double> { -0.432, 0.024, -0.025, -0.901, 0.0, 0.0 };
            Comparer.AreEqual(a2, refAlpha2, 1e-3, "TestProbCompMixedCorrelatedCase4: diff in alpha2");
            // equal to results from Probabilistic Toolkit
        }

        private static void TestProbCompMixedCorrelatedCase5()
        {
            double[,] rho =
            {
                { 1,   -1,  0.7, 0.7, 0,   0.8 },
                { -1,  1,   0.8, 0.8, 0,   0.8 },
                { 0.7, 0.8, 1,   0.7, 0,   0.7 },
                { 0.7, 0.8, 0.7, 1,   1,   0.7 },
                { 0,   0,   0,   1,   1,   0.3 },
                { 0.8, 0.8, 0.7, 0.7, 0.3, 1 }
            };

            var w = FillCorrelationPairs(rho, true);
            var sds = GetStochasts();
            var f = GetBasicSettings(DPoptions.MultiplyInvCholesky);
            var ii = new List<int>() { -999 };
            var r = ProbCalc.Calc(f, sds, w.ToArray(), AnotherSimpleZFunc_3, ProbLibWrapperTests.progressForm, ii.ToArray());

            Comparer.AreEqual(r.beta, 0.788, 1e-3);
            var a = (new List<double>(r.alpha)).GetRange(0, 6);
            var refAlpha = new List<double> { -0.935, 0.935, -0.525, -0.443, -0.443, -0.782 };
            Comparer.AreEqual(a, refAlpha, 1e-3, "TestProbCompMixedCorrelatedCase5: diff in alpha");

            var a2 = (new List<double>(r.alpha2)).GetRange(0, 6);
            var refAlpha2 = new List<double> { -0.935, 0.0, 0.182, 0.243, 0.0, -0.181 };
            Comparer.AreEqual(a2, refAlpha2, 1e-3, "TestProbCompMixedCorrelatedCase5: diff in alpha2");
            // equal to results from Probabilistic Toolkit
        }
    }
}
