using System;
using Deltares.Probabilistic.Interface;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;

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
            testAdaptiveImportanceSampling();
        }

        internal static bool progressForm(ProgressType progress, StringBuilder s)
        {
            // print everything
            Console.WriteLine(s);
            return false;
        }

        internal static bool progressSampling(ProgressType progress, StringBuilder s)
        {
            if (progress == ProgressType.Global)
            {
                Console.WriteLine(s);
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
            var x = new XandAlpha[2];
            var r = ProbCalc.Calc(m, d.ToArray(), SimpleZ, progressForm, x);
            ProbCalc.DumpResults(r, x);
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
            var x = new XandAlpha[2];
            var r = ProbCalc.Calc(m, d.ToArray(), SimpleZ, progressSampling, x);
            ProbCalc.DumpResults(r, x);
        }

        internal static void testAdaptiveImportanceSampling()
        {
            var m = new basicSettings();
            ProbCalc.SetDefaults(ref m);
            m.methodId = ProbMethod.AdaptiveIM;
            m.numThreads = 1;
            m.tolA = 0.03;
            m.tolB = 0.03;
            m.tolC = 0.03;
            m.progressInterval = 2500;
            var d = new List<fdistribs>();
            var p4 = new double[4];
            p4[0] = 0.0;
            p4[1] = 1.0;
            var a = new fdistribs { distId = EnumDistributions.normal, p4 = p4 };
            d.Add(a);
            d.Add(a);
            var x = new XandAlpha[2];
            var r = ProbCalc.Calc(m, d.ToArray(), SimpleZ, progressSampling, x);
            ProbCalc.DumpResults(r, x);
        }
    }
}
