#include <memory>
#include "stringHelper.h"
#include "../../../src/distributions/parseDistribution.h"
#include "../../../src/probFuncs/conversions.h"

using namespace Deltares::ProbLibCore;

extern "C"
void calculatedistributioninverse_c(double *u, double *y, EnumDistributions *type, double p[], tError *ierr)
{
    try
    {
        std::string name = "name";
        auto pd = parseDistribution();
        auto nrActive = pd.activeParameters(*type);
        std::vector<double> p4;
        for (size_t i = 0; i < nrActive; i++)
        {
            p4.push_back(p[i]);
        }
    
        std::unique_ptr<distrib> d (pd.parse(*type, name, p4));
        *y = d->getValue(*u);
        ierr->errorCode = 0;
    }
    catch(const std::exception& e)
    {
        std::string s = e.what();
        ierr->errorCode = -1;
        fillErrorMessage(*ierr, s);
    }
}

