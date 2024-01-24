using Deltares.Probabilistics.Wrappers.Test;
using System;

try
{
    var c = new TestDirectionalSampling();
    c.TestBligh();
    c.TestLinear();
    c.TestInverseLinear();
    c.TestLinearParallel();
    c.TestDoubleLinear();
    c.TestNoisy();
    c.TestNonLinear();
    c.TestBlock();
    c.TestSemiBlock();
    c.TestSaddle();
    c.TestManyVars();
    c.TestTwoBranches();
}
catch (Exception ex)
{
    Console.WriteLine(ex.ToString());
}
return 0;

