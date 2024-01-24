using Deltares.Probabilistics.Wrappers.Test;
using System;

try
{
    var c = new TestDistributions();
    c.TestDeterministic();
    c.TestGumbel();
    c.TestLogNormal();
    c.TestNormal();
    c.TestNormalTruncated();
    c.TestUniform();

    var d = new TestStandardNormal();
    d.GetQFromUTest();
    d.GetUFromQTest();
}
catch (Exception ex)
{
    Console.WriteLine(ex.ToString());
}
return 0;

