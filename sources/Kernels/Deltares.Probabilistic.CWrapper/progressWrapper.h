#pragma once
#include <functional>

#include "../Deltares.Probabilistic.Kernels/Model/ProgressIndicator.h"
#include "../Deltares.Probabilistic.Kernels/Reliability/ReliabilityMethod.h"

typedef std::function <bool(Deltares::Models::ProgressType, std::string)> progressFuncExt;

class progressWrapper
{
public:
    progressWrapper(progressFuncExt func, Deltares::Reliability::ReliabilityMethod* relMethod) :
        externalProgressFunc(func), pntrToReliabilityMethod(relMethod) {;}
    void FPgDelegate(Deltares::Models::ProgressType pt, std::string s);
private:
    progressFuncExt externalProgressFunc;
    Deltares::Reliability::ReliabilityMethod* pntrToReliabilityMethod;
};

