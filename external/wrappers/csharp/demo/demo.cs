using System;
using Deltares.ProbabilisticCPP.Wrapper;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using Probabilistic.Tests;

namespace Deltares.ProbabilisticCPP.Main
{
    public class PBprogram
    {
        /// <summary>
        /// Test (cmake setup)
        /// </summary>
        static void Main(string[] args)
        {
            double beta = 0.0;
            if (args.Length > 0)
            {
                if (!double.TryParse(args[0], out beta))
                {
                    beta = 0.0;
                }
            }
            testPQfromBeta(beta);
            testHohenblichler();
            testDistributions();
            testForm();
            testImpSampling();

            TestUpscaling.allUpscalingTests();

            ProbLibWrapperTests.runAllProbLibWrapperTests();

            TestCorrelation.runAllCorrelationTests();

            TestCombineElements.runAllCombinTests();
        }

        internal static void testPQfromBeta(double beta)
        {
            const double eps = 1e-12;
            double p1 = PLConversions.PFromBeta(beta);
            double q1 = PLConversions.QFromBeta(beta);
            PLConversions.PQFromBeta(beta, out double p, out double q);
            Comparer.AreEqual(p1, p, eps, "p are equal");
            Comparer.AreEqual(q1, q, eps, "q are equal");
            Comparer.AreEqual(1.0, p + q, 1e-12, "p+q=1");
            double y = PLConversions.BetaFromQ(2.0);
            Comparer.IsTrue(y < -37.0, "beta at invalid q(2.0)");
        }

        internal static void testHohenblichler()
        {
            const double expectedpf2pf1 = 0.11323892;
            var p2 = PLCombin.Hohenbichler(3.0, 1e-4, 0.5);
            Comparer.AreEqual(p2, expectedpf2pf1, 1e-6, "test Hohenbichler");
        }

        internal static void testDistributions()
        {
            const double eps = 1e-12;
            var p4 = new List<double> { 1, 2, 0, 0 };
            var type = EnumDistributions.normal;
            var y = PLDistributions.CalculateDistributionInverse(1.2, type, p4.ToArray());
            Comparer.AreEqual(y, 3.4, eps, "test normal distribution");

            type = EnumDistributions.lognormal2;
            y = PLDistributions.CalculateDistributionInverse(1.2, type, p4.ToArray());
            Comparer.AreEqual(y, 2.0495997561755828, eps, "test lognormal2 distribution");
        }

        internal static bool progressForm(int i, double x, double y)
        {
            if (i % 2 == 0)
            {
                Console.WriteLine(string.Format("iter = {0}, beta = {1}, conv = {2}", i, x, y));
            }
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

        internal static void testForm()
        {
            const double eps = 1e-12;
            var m = new basicSettings();
            ProbCalc.SetDefaults(ref m);
            m.methodId = ProbMethod.FORM;
            m.numThreads = 1;
            m.du = 0.3;
            var d = new List<fdistribs>();
            var a = new fdistribs{distId = EnumDistributions.uspace};
            d.Add(a);
            d.Add(a);
            var r = ProbCalc.Calc(m, d.ToArray(), SimpleZ, progressForm);
            Comparer.AreEqual(r.convergence, 0, "testForm: conv. flag");
            Comparer.AreEqual(r.beta, Math.Sqrt(2), eps, "testForm: beta");
            Comparer.AreEqual(r.alpha[0],  0.5 * Math.Sqrt(2), eps, "testForm: alpha[0]");
            Comparer.AreEqual(r.alpha[1], -0.5 * Math.Sqrt(2), eps, "testForm: alpha[1]");
            Comparer.AreEqual(r.x[0], -1.0, eps, "testForm: x[0]");
            Comparer.AreEqual(r.x[1],  1.0, eps, "testForm: x[1]");
        }

        internal static void testImpSampling()
        {
            const double eps = 1e-2;
            var m = new basicSettings();
            ProbCalc.SetDefaults(ref m);
            m.methodId = ProbMethod.IMHR;
            m.numThreads = 1;
            m.tolA = 0.03;
            m.tolB = 0.03;
            m.tolC = 0.03;
            m.progressInterval = 2500;
            var d = new List<fdistribs>();
            var a = new fdistribs { distId = EnumDistributions.uspace };
            d.Add(a);
            d.Add(a);
            var r = ProbCalc.Calc(m, d.ToArray(), SimpleZ, progressSampling);
            Comparer.AreEqual(r.convergence, 0, "testImpSampling: conv. flag");
            Comparer.AreEqual(r.beta, Math.Sqrt(2), eps, "testImpSampling: beta");
            Comparer.AreEqual(r.alpha[0],  0.5 * Math.Sqrt(2), 0.1, "testImpSampling: alpha[0]");
            Comparer.AreEqual(r.alpha[1], -0.5 * Math.Sqrt(2), 0.1, "testImpSampling: alpha[1]");
            Comparer.AreEqual(r.x[0], -1.0, 0.2, "testImpSampling: x[0]");
            Comparer.AreEqual(r.x[1],  1.0, 0.2, "testImpSampling: x[1]");
        }
    }
}
