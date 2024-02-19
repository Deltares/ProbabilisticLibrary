using Deltares.ProbLib;
using System;
using System.Runtime.InteropServices;

var q = conversions2.betaFromQ(0.5);
Console.WriteLine(string.Format("{0,12:F4}", q));

var z = intElements.integrateEqualElements(3.0, 0.0, 365);
Console.WriteLine(string.Format("{0,12:F4}", z));

ab x = new(3, 4);
var y = x.sum();
Console.WriteLine(string.Format("{0,12:F4}", y));
Console.WriteLine(string.Format("{0,12:F4}", x.a));

x.a = -999;

var a= new funcDel();

MyDel b = a.func1;
MyDel c = MyClass.a;
MyZdel d = MyClass.b;

var qq = b(9);
var ww = c(12);
var yy = a.func3(c, 1234);
var xx = new double[2];
var qq2 = GCHandle.Alloc(xx);
IntPtr qq3 = (IntPtr) qq2;
var zz = a.func4(d, (long)qq3);
Console.WriteLine(qq.ToString());
Console.WriteLine(ww.ToString());

Console.WriteLine("ready");

//var z = zm.zmodel();
//var c = cm.calc();

public class MyClass
{
    //public delegate double MyZdelegate(long a);
    public static int a(int b)
    {
        return 2 * b;
    }

    public static double b(long  a)
    {
        IntPtr ptr = (IntPtr) a;
        double[] theArray = new double[2];
        Marshal.Copy(ptr, theArray, 0, 2);
        //return theArray[1] - theArray[0];
        return 9.0;// a[1] - a[0];
    }
}
