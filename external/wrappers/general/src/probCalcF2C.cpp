#include <string>
#include <omp.h>
#include <memory>
#include "fortranZfunc.h"
#include "stringHelper.h"
#include "../../../src/probMethods/parseAndRunMethod.h"
#include "../../../src/correlation/identity.h"
#include "../../../src/correlation/gaussianCorrelationRobust.h"
#include "../../../src/distributions/parseDistribution.h"
#include "../../../src/utils/basic_math.h"
#include "../../../src/probMethods/dsSettings.h"

using namespace Deltares::ProbLibCore;

const size_t lenSmallStr = 32;

struct fdistribs
{
    char name[lenSmallStr];
    int distId;
    double params[4];
};

struct tResult
{
    int convergence;
    double beta;
    double alpha[maxActiveStochast];
    double x[maxActiveStochast];
    int iPoint[maxActiveStochast];
    int stepsNeeded;
    int samplesNeeded;
    double alpha2[maxActiveStochast];
};

// helper for probcalcf2c
correlation* getCorrelation(corrStruct correlations[], const int nrCorrelations, const int n)
{
    if (nrCorrelations > 0)
    {
        auto pairs = std::vector<corrStruct>();
        for (size_t i = 0; i < (size_t)nrCorrelations; i++)
        {
            pairs.push_back(correlations[i]);
        }
        return new gaussianCorrelationRobust(n, pairs);
    }
    else
    {
        return new identity(n);
    }
}

// another helper for probcalcf2c
progress* get_progress_ptr(const int interval, const bool(*pc)(int, double, double))
{
    if (interval > 0)
    {
        return new externalProgressCancel(pc);
    }
    else
    {
        return new progress();
    }
}

extern "C"
void probcalcf2c(const basicSettings* method, const fdistribs* c, const int n, const int vectorSize,
    corrStruct correlations[], const int nrCorrelations,
    const double(*fx)(double[], int[], tError*),
    const bool(*pc)(int, double, double),
    int compIds[], int iPoint[], double x[], tResult* r, tError* ierr)
{
    auto nStoch = (size_t)n;
    size_t numThreads = (size_t)method->numThreads;
    omp_set_num_threads(method->numThreads);
    auto zf = fortranZfunc(fx, nStoch, (size_t)vectorSize, compIds, iPoint, x, numThreads);

    std::unique_ptr<progress> pg (get_progress_ptr(method->progressInterval, pc));

    auto pr = parseAndRunMethod(*method, *pg);

    try
    {
        auto ld = listDistribs(vectorSize);
        auto parser = parseDistribution();
        auto cntDeterminists = 0;
        for (size_t i = 0; i < nStoch; i++)
        {
            std::string name = c[i].name;
            auto dist = (EnumDistributions) c[i].distId;
            if (dist == EnumDistributions::deterministic) cntDeterminists++;
            std::vector<double> params = {c[i].params[0], c[i].params[1], c[i].params[2], c[i].params[3]};
            ld.addDistrib(parser.parse(dist, name, params));
        }

        std::unique_ptr<correlation> corr (getCorrelation(correlations, nrCorrelations, n));
        auto useIp = (nrCorrelations > 0 || cntDeterminists > 0);
        auto stoch = stochSettings(*corr, ld, useIp);
        auto result = pr.parseAndRun(method->methodId, zf, stoch, numThreads);
        ierr->errorCode = 0;
        r->beta = result.result.getBeta();
        for (size_t i = 0; i < result.result.size(); i++)
        {
            r->alpha[i] = result.result.getAlphaI(i);
        }

        if (result.x.size() <= maxActiveStochast)
        {
            for (size_t i = 0; i < result.x.size(); i++)
            {
                r->x[i] = result.x(i);
            }
        }
        if (result.x.size() == (size_t)vectorSize)
        {
            for (size_t i = 0; i < result.x.size(); i++)
            {
                x[i] = result.x(i);
            }
        }

        for (int i = 0; i < n; i++)
        {
            r->iPoint[i] = i + 1;
        }
        for (size_t i = 0; i < result.alpha_u.size(); i++)
        {
            r->alpha2[i] = result.alpha_u(i);
        }
        r->convergence = (int)result.convergence;
        r->stepsNeeded = result.stepsNeeded;
        r->samplesNeeded = result.samplesNeeded;
    }
    catch (const std::exception& e)
    {
        std::string s = e.what();
        ierr->errorCode = -1;
        fillErrorMessage(*ierr, s);
    }
}
