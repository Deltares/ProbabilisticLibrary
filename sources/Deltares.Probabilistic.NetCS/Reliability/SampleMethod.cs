using System;

namespace Deltares.Probabilistic.Reliability;

public enum SampleMethod
{
    MarkovChain,
    AdaptiveConditional
};

public static class SampleMethodConverter
{
    public static string ConvertToString(SampleMethod method)
    {
        return method switch
        {
            SampleMethod.MarkovChain => "markov_chain",
            SampleMethod.AdaptiveConditional => "adaptive_conditional",
            _ => throw new ArgumentOutOfRangeException(nameof(method), method, null)
        };
    }

    public static SampleMethod ConvertFromString(string value)
    {
        return value switch
        {
            "markov_chain" => SampleMethod.MarkovChain,
            "adaptive_conditional" => SampleMethod.AdaptiveConditional,
            _ => throw new ArgumentException($"Unknown sample method: '{value}'", nameof(value))
        };
    }
}
