#include <memory>
#include "stringHelper.h"
#include "../../../src/distributions/parseDistribution.h"
#include "../../../src/probFuncs/conversions.h"

using namespace Deltares::ProbLibCore;

extern "C"
void calculatedistributioninverse_c(double *u, double *y, EnumDistributions *type, double p[], tError *ierr)
{
    std::string s = "not implemented yet";
    ierr->errorCode = -1;
    fillErrorMessage(*ierr, s);
}

