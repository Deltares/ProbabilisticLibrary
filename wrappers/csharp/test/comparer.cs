using System;
using System.Collections.Generic;

namespace Probabilistic.Tests
{
    public class Comparer
    {
        public static void AreEqual(double a, double b, double tol, string msg)
        {
            IsTrue(Math.Abs(a - b) < tol, msg);
        }

        public static void AreEqual(int a, int b, string msg)
        {
            IsTrue(a == b, msg);
        }

        public static void AreEqual(string a, string b, string msg)
        {
            IsTrue(a == b, msg);
        }

        public static void AreEqual(double a, double b, double tol)
        {
            if (Math.Abs(a - b) > tol)
            {
                throw new Exception("diff > tol");
            }
        }

        public static void AreEqual(List<double> a, List<double> b, double tol, string msg)
        {
            if (a.Count != b.Count)
            {
                throw new Exception("a and b must have the same size: " + msg);
            }

            for(var i=0; i<a.Count; i++)
            {
                if (Math.Abs(a[i] - b[i]) > tol)
                {
                    throw new Exception(msg);
                }
            }
            Console.WriteLine(msg + " ok!");
        }

        public static void IsTrue(bool a, string msg)
        {
            if ( ! a)
            {
                throw new Exception(msg);
            }
            else
            {
                Console.WriteLine(msg + " ok!");
            }
        }
    }
}
