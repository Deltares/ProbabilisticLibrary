using Deltares.Probabilistics.Wrappers.Test;
using System;

// See https://aka.ms/new-console-template for more information
try
{
    //Stopwatch stopWatch = System.Diagnostics.Stopwatch.StartNew();
    var c = new TestCrudeMonteCarlo();
    c.TestBligh();
    c.TestConvex();
    c.TestLinear();
    c.TestLinearLimited();
    c.TestLinearNegativeFullyCorrelated();
    c.TestLinearManySamples();
    c.TestLinearParallel();
    c.TestLinearMessages();
    c.TestLinearPartialCorrelated();
    c.TestLinearProgress();
    c.TestLinearPartialCorrelated();
    c.TestLoadStrength();
    c.TestLoadStrengthSurvived();
    c.TestNoisy();
    //c.TestNoisyInheritedModelRunner();
    c.TestNoisyMessages();
    c.TestNoisyStochastSettings();
    c.TestNonLinear();
    //stopWatch.Stop();
    //Console.WriteLine(stopWatch.Elapsed.ToString());
}
catch (Exception ex)
{
    Console.WriteLine(ex.ToString());
}
return 0;

