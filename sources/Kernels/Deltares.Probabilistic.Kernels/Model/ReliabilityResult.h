#pragma once
#include <limits>

class ReliabilityResult
{
public:
    int ProxyIteration = -1;
    int Run = 0;
    int Index = -1;
    double Reliability = std::numeric_limits<double>::quiet_NaN();
    double LowReliability = std::numeric_limits<double>::quiet_NaN();
    double HighReliability = std::numeric_limits<double>::quiet_NaN();
    double Samples = std::numeric_limits<double>::quiet_NaN();
    double ConvBeta = std::numeric_limits<double>::quiet_NaN();
    double ProxyConvergence = std::numeric_limits<double>::quiet_NaN();
    double Variation = std::numeric_limits<double>::quiet_NaN();
    double Difference = std::numeric_limits<double>::quiet_NaN();
    double Contribution = std::numeric_limits<double>::quiet_NaN();
};

