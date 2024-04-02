#include "funcWrapper.h"
#include "../Deltares.Probabilistic/Utils/probLibException.h"
#include <omp.h>

using namespace Deltares::Reliability;

void funcWrapper::FDelegate(std::shared_ptr<Deltares::Models::ModelSample> s)
{
    auto xx = new double[allStoch];
    copyXvector(xx, s);
    computationSettings compSetting{ designPointOptions::dpOutFALSE, compId, 0 };
    if (s->IterationIndex >= 0)
    {
        compSetting.threadId = s->IterationIndex % omp_get_max_threads();
    }
    tError e = tError();
    double result = zfunc(xx, &compSetting, &e);
    delete[] xx;
    if (e.errorCode != 0) throw probLibException(e.errorMessage);
    s->Z = result;
}

void funcWrapper::FDelegateParallel(std::vector<std::shared_ptr<Deltares::Models::ModelSample>> samples)
{
    auto buffer = new double[allStoch * omp_get_max_threads()];
    #pragma omp parallel for
    for (int i = 0; i < (int)samples.size(); i++)
    {
        double*x = &buffer[allStoch * omp_get_thread_num()];
        copyXvector(x, samples[i]);
        computationSettings compSetting{ designPointOptions::dpOutFALSE, compId, omp_get_thread_num() };
        tError e = tError();
        double result = zfunc(x, &compSetting, &e);
        samples[i]->Z = result;
    }
    delete[] buffer;
}

void funcWrapper::updateXinDesignPoint(double x[])
{
    auto xx = new double[allStoch];
    copyXvector(xx, x);
    computationSettings compSetting{ designPointOptions::dpOutTRUE, compId, 0 };
    tError e = tError();
    zfunc(xx, &compSetting, &e);
    for (size_t i = 0; i < iPointer.size(); i++)
    {
        x[i] = xx[iPointer[i]];
    }
    delete[] xx;
}

void funcWrapper::copyXvector(double x[], const std::shared_ptr<Deltares::Models::ModelSample> s) const
{
    for (size_t i = 0; i < allStoch; i++)
    {
        x[i] = xRef[i];
    }
    for (size_t i = 0; i < s->Values.size(); i++)
    {
        x[iPointer[i]] = s->Values[i];
    }
}

void funcWrapper::copyXvector(double x[], const double s[]) const
{
    for (size_t i = 0; i < allStoch; i++)
    {
        x[i] = xRef[i];
    }
    for (size_t i = 0; i < iPointer.size(); i++)
    {
        x[iPointer[i]] = s[i];
    }
}

