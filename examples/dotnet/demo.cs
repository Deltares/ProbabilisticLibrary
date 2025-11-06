using System;
using Deltares.ProbabilisticLibrary.Interface;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace Deltares.Examples.dotnet
{
    public class Demo
    {
        /// <summary>
        /// Test (cmake setup)
        /// </summary>
        static void Main(string[] args)
        {
            testForm();
            testImpSampling();
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
            var m = new basicSettings();
            ProbCalc.SetDefaults(ref m);
            m.methodId = ProbMethod.FORM;
            m.numThreads = 1;
            m.du = 0.3;
            var d = new List<fdistribs>();
            var p4 = new double[4];
            p4[0] = 0.0;
            p4[1] = 1.0;
            var a = new fdistribs{distId = EnumDistributions.normal, p4=p4};
            d.Add(a);
            d.Add(a);
            var r = ProbCalc.Calc(m, d.ToArray(), SimpleZ, progressForm);
        }

        internal static void testImpSampling()
        {
            var m = new basicSettings();
            ProbCalc.SetDefaults(ref m);
            m.methodId = ProbMethod.IM;
            m.numThreads = 1;
            m.tolA = 0.03;
            m.tolB = 0.03;
            m.tolC = 0.03;
            m.progressInterval = 2500;
            var d = new List<fdistribs>();
            var p4 = new double[4];
            p4[0] = 0.0;
            p4[1] = 1.0;
            var a = new fdistribs { distId = EnumDistributions.normal, p4=p4 };
            d.Add(a);
            d.Add(a);
            var r = ProbCalc.Calc(m, d.ToArray(), SimpleZ, progressSampling);
        }
    }
}
