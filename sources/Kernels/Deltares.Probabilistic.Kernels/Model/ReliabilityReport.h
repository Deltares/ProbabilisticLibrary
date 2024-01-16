#pragma once
#include <limits>

class ReliabilityReport
{
public:
    int Index = -1;
    double Reliability = std::numeric_limits<double>::quiet_NaN();
    double ConvBeta = std::numeric_limits<double>::quiet_NaN();
    double Variation = std::numeric_limits<double>::quiet_NaN();
    int Step= 0;
    int Loop = 0;
    int MaxSteps = 0;
    double Contribution = std::numeric_limits<double>::quiet_NaN();
    bool ReportMatchesEvaluation = true;
};

