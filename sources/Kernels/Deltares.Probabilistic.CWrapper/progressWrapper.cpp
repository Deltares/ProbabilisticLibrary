#include "progressWrapper.h"

void progressWrapper::FPgDelegate(Deltares::Models::ProgressType pt, std::string s)
{
    auto cancel = externalProgressFunc(pt, s);
    if (cancel)
    {
        pntrToReliabilityMethod->Stop();
    }
}

