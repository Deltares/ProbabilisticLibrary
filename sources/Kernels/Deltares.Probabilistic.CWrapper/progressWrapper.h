#pragma once
#include <functional>

#include "../Deltares.Probabilistic.Kernels/Model/ProgressIndicator.h"
#include "../Deltares.Probabilistic.Kernels/Reliability/ReliabilityMethod.h"

class progressWrapper
{
public:
    std::function <bool(Deltares::Models::ProgressType, std::string)> externalProgressFunc;

    Deltares::Reliability::ReliabilityMethod* pntrToReliabilityMethod;
    void FPgDelegate(Deltares::Models::ProgressType pt, std::string s);
};

