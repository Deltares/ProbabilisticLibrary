#include "progressWrapper.h"

void progressWrapper::FPgDelegate(Deltares::Models::ProgressType pt, std::string s)
{
    auto cancel = staticPg(pt, s);
    if (cancel)
    {
        rmStatic->Stop();
    }
}

