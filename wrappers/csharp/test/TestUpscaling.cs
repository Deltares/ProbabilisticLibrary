using System;
using Deltares.ProbabilisticCPP.Wrapper;

namespace Probabilistic.Tests
{
    /// <summary>
    /// Class for testing class Upscaling
    /// </summary>
    public class TestUpscaling
    {
        public static void allUpscalingTests()
        {
            TestUpscaleInTime();
            TestUpscaleInTime2();
            TestUpscaleLength1();
            TestUpscaleLength2();
        }

        private static void TestUpscaleLength1()
        {
            const double betaCrossSection = 5.0;
            var alphaCrossSection = new [] {0.0, 0.6, 0.0, -0.8, 0.0};
            var rhoXK = new [] { 0.8, 0.8 , 0.8 , 0.8 , 0.8 };
            var dXK = new [] { 200.0, 200.0 , 200.0 , 200.0 , 200.0 };
            const double sectionLength = 250.0;
            const double breachLength = 0.0;
            double betaSection;
            var alphaSection = new double[5];
            PLCombin.UpscaleLength(betaCrossSection, alphaCrossSection, rhoXK, dXK, sectionLength,
                out betaSection, out alphaSection, breachLength);
            const double expectedBetaSection = 4.81288713415857;
            Comparer.AreEqual(expectedBetaSection, betaSection, 1e-6);
        }

        private static void TestUpscaleLength2()
        {
            const double betaCrossSection = 5.0;
            var alphaCrossSection = new [] { 0.0, 0.6, 0.0, -0.8, 0.0 };
            var rhoXK = new [] { 0.8, 0.8, 0.8, 0.8, 0.8 };
            var dXK = new [] { 200.0, 200.0, 200.0, 200.0, 200.0 };
            const double sectionLength = 250.0;
            double betaSection;
            var alphaSection = new double[5];
            PLCombin.UpscaleLength(betaCrossSection, alphaCrossSection, rhoXK, dXK, sectionLength, out betaSection, out alphaSection);
            const double expectedBetaSection = 4.9094794340831234;
            Comparer.AreEqual(expectedBetaSection, betaSection, 1e-6);
        }

        private static void TestUpscaleInTime()
        {
            // test based on Fortran test upscaleInTimeTests1
            const int nStochasts = 5;
            const double nrElements = 10.0;
            var beta = 3.5;
            var alpha1 = Math.Sqrt(1.0 / nStochasts);
            var alpha = new [] {alpha1, alpha1, alpha1, alpha1, alpha1}; // equal alpha values
            var inRhoT = new [] {1.0, 1.0, 1.0, 1.0, 1.0};               // all rho values set to one

            PLCombin.UpscaleInTime(nrElements, ref beta, alpha, inRhoT);
            Comparer.AreEqual(beta, 3.5, 0.001);
        }

        private static void TestUpscaleInTime2()
        {
            // test based on Fortran test upscaleInTimeTests6
            const double nrElements = 10.0;
            var beta = 3.5;
            var alpha = new[] { 0.0, 0.6, 0.0, -0.8, 0.0 };
            var alphaOriginal = alpha;
            var inRhoT = new[] { 0.999999, 0.999999, 0.999999, 0.999999, 0.999999 };

            const double margin = 1.0e-6;

            const double betaExpectedResult = 3.49846239808633;

            PLCombin.UpscaleInTime(nrElements, ref beta, alpha, inRhoT);
            Comparer.AreEqual(beta, betaExpectedResult, margin);
            for (var i = 0; i < alpha.Length; i++)
            {
                Comparer.AreEqual(alpha[i], alphaOriginal[i], margin);
            }

        }
    }
}
