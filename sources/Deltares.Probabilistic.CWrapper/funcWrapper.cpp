#include "funcWrapper.h"

#include "../Deltares.Probabilistic/Utils/probLibException.h"
#include <omp.h>

using namespace Deltares::Reliability;
using namespace Deltares::Models;

void funcWrapper::FDelegate(std::shared_ptr<ModelSample> s)
{
    designPointOptions dp = s->ExtendedLogging ? designPointOptions::dpOutTRUE : designPointOptions::dpOutFALSE;

    computationSettings compSetting{ dp, compId, s->threadId, s->LoggingCounter };
    tError e = tError();
    double result = zfunc(s->Values.data(), &compSetting, &e);
    if (e.errorCode != 0)
    {
        error_messages.push_back((e.errorMessage));
        s->Z = std::nan("");
    }
    else
    {
        s->Z = result;
    }
}

void funcWrapper::FDelegateParallel(std::vector<std::shared_ptr<ModelSample>> samples)
{
    auto errorMessagePerThread = std::vector<std::string>(omp_get_num_threads());

#pragma omp parallel for
    for (int i = 0; i < (int)samples.size(); i++)
    {
        computationSettings compSetting{ designPointOptions::dpOutFALSE, compId, omp_get_thread_num(), 1 };
        tError e = tError();
        double result = zfunc(samples[i]->Values.data(), &compSetting, &e);
        if (e.errorCode != 0)
        {
            errorMessagePerThread[omp_get_thread_num()] = e.errorMessage;
            samples[i]->Z = std::nan("");
        }
        else
        {
            samples[i]->Z = result;
        }
    }

    for (const auto& s : errorMessagePerThread)
    {
        if (s.length() > 0)
        {
            error_messages.push_back(s);
        }
    }
}

