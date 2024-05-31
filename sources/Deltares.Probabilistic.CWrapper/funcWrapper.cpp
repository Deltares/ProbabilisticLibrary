#include "funcWrapper.h"
#include "../Deltares.Probabilistic/Utils/probLibException.h"
#include <omp.h>

using namespace Deltares::Reliability;
using namespace Deltares::Models;

void funcWrapper::FDelegate(std::shared_ptr<Deltares::Models::ModelSample> s)
{
    auto dp = (designPointOptions)s->loggingOption;
    computationSettings compSetting{ dp, compId, s->threadId };
    tError e = tError();
    double result = zfunc(s->Values.data(), &compSetting, &e);
    if (e.errorCode != 0) throw probLibException(e.errorMessage);
    s->Z = result;
}

void funcWrapper::FDelegateParallel(std::vector<std::shared_ptr<Deltares::Models::ModelSample>> samples)
{
    #pragma omp parallel for
    for (int i = 0; i < (int)samples.size(); i++)
    {
        computationSettings compSetting{ designPointOptions::dpOutFALSE, compId, omp_get_thread_num() };
        tError e = tError();
        double result = zfunc(samples[i]->Values.data(), &compSetting, &e);
        samples[i]->Z = result;
    }
}

