#pragma once
#include <functional>

#include "../Deltares.Probabilistic/Model/ProgressIndicator.h"
#include "../Deltares.Probabilistic/Reliability/ReliabilityMethod.h"

typedef std::function <bool(Deltares::Models::ProgressType, const char*)> progressFuncExt;

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

