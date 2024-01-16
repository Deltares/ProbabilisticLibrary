#include "ReliabilityResult.h"
#include "../Math/NumericSupport.h"

bool ReliabilityResult::IsMeaningful(ReliabilityResult* resultBefore, ReliabilityResult* resultAfter)
{
    if (resultBefore == nullptr || resultAfter == nullptr)
    {
        return true;
    }

    return !NumericSupport::areEqual(Reliability, resultBefore->Reliability, 0.001) ||
	       !NumericSupport::areEqual(Reliability, resultAfter->Reliability, 0.001);
}
