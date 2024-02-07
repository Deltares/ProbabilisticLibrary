#include "ReliabilityResult.h"
#include "../Math/NumericSupport.h"

namespace Deltares
{
    namespace Reliability
    {
        bool ReliabilityResult::IsMeaningful(std::shared_ptr<ReliabilityResult> resultBefore, std::shared_ptr<ReliabilityResult> resultAfter)
        {
            if (resultBefore == nullptr || resultAfter == nullptr)
            {
                return true;
            }

            return !NumericSupport::areEqual(Reliability, resultBefore->Reliability, 0.001) ||
                !NumericSupport::areEqual(Reliability, resultAfter->Reliability, 0.001);
        }
    }
}
