#pragma once
#include <cmath>

class ReliabilityResult
{
public:
    int ProxyIteration = -1;
    int Run = 0;
    int Index = -1;
    double Reliability = nan("");
    double LowReliability = nan("");
    double HighReliability = nan("");
    double Samples = nan("");
    double ConvBeta = nan("");
    double ProxyConvergence = nan("");
    double Variation = nan("");
    double Difference = nan("");
    double Contribution = nan("");

    bool IsMeaningful(ReliabilityResult* resultBefore, ReliabilityResult* resultAfter);

};

