#include <memory>
#include "stringHelper.h"
#include "../Deltares.Probabilistic.CWrapper/createDistribution.h"

using namespace Deltares::Reliability;

extern "C"
void calculatedistributioninverse_c(double *u, double *y, EnumDistributions *type, double p[], tError *ierr)
{
    try
    {
        auto s = createDistribution::createValid(*type, p);
        *y = s->getXFromU(*u);
        ierr->errorCode = 0;
    }
    catch (const std::exception& e)
    {
        ierr->errorCode = -1;
        std::string s = e.what();
        fillErrorMessage(*ierr, s);
    }
}

extern "C"
void calculatedistribution_c(double* x, double* u, EnumDistributions * type, double p[], tError * ierr)
{
    try
    {
        auto s = createDistribution::createValid(*type, p);
        *u = s->getUFromX(*x);
        ierr->errorCode = 0;
    }
    catch (const std::exception& e)
    {
        ierr->errorCode = -1;
        std::string s = e.what();
        fillErrorMessage(*ierr, s);
    }
}


