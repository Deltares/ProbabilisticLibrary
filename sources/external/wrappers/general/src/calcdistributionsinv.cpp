#include <memory>
#include "stringHelper.h"
#include "../../../../Kernels/Deltares.Probabilistic.CWrapper/createDistribution.h"

using namespace Deltares::ProbLibCore;

extern "C"
void calculatedistributioninverse_c(double *u, double *y, EnumDistributions *type, double p[], tError *ierr)
{
    try
    {
        auto s = createDistribution::create(*type, p);
        *y = s->getXFromU(*u);
    }
    catch (const std::exception& e)
    {
        ierr->errorCode = -1;
        std::string s = e.what();
        fillErrorMessage(*ierr, s);
    }
}

