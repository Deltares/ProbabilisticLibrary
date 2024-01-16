#pragma once
#include <cmath>

class ReliabilityReport
{
public:
    int Index = -1;
    double Reliability = nan("");
    double ConvBeta = nan("");
    double Variation = nan("");
    int Step= 0;
    int Loop = 0;
    int MaxSteps = 0;
    double Contribution = nan("");
    bool ReportMatchesEvaluation = true;
};

