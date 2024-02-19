#include <limits>
#include "stringHelper.h"
#include "../../../src/extern/asa063.hpp"
#include "../../../src/extern/dgammaDs.h"

using namespace Deltares::ProbLibCore;
using namespace ASA::conversions;

extern "C"
double betainc(double x, double p, double q, double beta, int* ifault)
{
    return betain(x, p, q, beta, *ifault);
}

extern "C"
double dgamln(const double x)
{
    auto d = dgammaDs(0.0);
    return d.DGamLn(x);
}

extern "C"
double dgammq_c(const double a, const double x, tError * ierr)
{
    try
    {
        ierr->errorCode = 0;
        auto gmmd = dgammaDs(a);
        return gmmd.Dgammq(a, x);
        //return 1.0 - conversions::Dgammq(x, a);
    }
    catch (const std::exception& e)
    {
        std::string s = e.what();
        ierr->errorCode = -1;
        fillErrorMessage(*ierr, s);
        return std::numeric_limits<double>::quiet_NaN();
    }
}

