#include <string>
#include <omp.h>
#include <memory>
#include <functional>
#include "fortranZfunc.h"
#include "stringHelper.h"
#include "../../../src/probMethods/parseAndRunMethod.h"
#include "../../../src/correlation/identity.h"
#include "../../../src/correlation/gaussianCorrelationRobust.h"
#include "../../../src/distributions/parseDistribution.h"
#include "../../../src/utils/basic_math.h"
#include "../../../src/probMethods/dsSettings.h"
#include "../../../../Kernels/Deltares.Probabilistic.Kernels/Reliability/CrudeMonteCarlo.h"

using namespace Deltares::ProbLibCore;
using namespace Deltares::Models;

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

std::function<double(double[], int[], tError*)> staticF;

double FDelegate (Sample* s)
{
    auto xx = new double[s->getSize()];
    for (size_t i = 0; i < s->getSize(); i++)
    {
        xx[i] = s->Values[i];
    }
    auto i = new int[4];
    tError e = tError();
    double result = staticF(xx, i, &e);
    delete[] xx;
    s->Z = result;
    return result;
}

extern "C"
void probcalcf2cnew(const basicSettings* method, const fdistribs* c, const int n, const int vectorSize,
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

    staticF = fx;
    try
    {
        auto cntDeterminists = 0;
        auto stochast = std::vector<Stochast*>();
        for (size_t i = 0; i < nStoch; i++)
        {
            std::string name = c[i].name;
            auto dist = (DistributionType) c[i].distId;
            auto params = new double[4];
            for (size_t j = 0; j < 4; j++)
            {
                params[j] = c[i].params[j];
            }
            auto s = new Stochast(dist, params);
            stochast.push_back(s);
        }

        auto mc = CrudeMonteCarlo();
        switch (method->rnd)
        {
            case rndTypes::GeorgeMarsaglia:
                mc.Settings->RandomSettings->RandomGeneratorType = Deltares::Numeric::RandomValueGeneratorType::GeorgeMarsaglia;
                break;
            case rndTypes::MersenneTwister:
                mc.Settings->RandomSettings->RandomGeneratorType = Deltares::Numeric::RandomValueGeneratorType::MersenneTwister;
                break;
            default:
                throw probLibException("ModifiedKnuthSubtractive not implemented in C wrapper");
                break;
        }
        mc.Settings->RandomSettings->Seed = method->seed1;
        mc.Settings->RandomSettings->SeedB = method->seed2;
        auto zModel = new ZModel(FDelegate);
        auto corr2 = new CorrelationMatrix();
        auto uConverter = new UConverter(stochast, corr2);
        uConverter->initializeForRun();
        auto progressDelegate = ProgressDelegate();
        auto detailedProgressDelegate = DetailedProgressDelegate();
        auto textualProgressDelegate = TextualProgressDelegate();
        auto progress = new ProgressIndicator(progressDelegate, detailedProgressDelegate, textualProgressDelegate);
        auto modelRunner = new ZModelRunner(zModel, uConverter, progress);
        auto newResult = mc.getDesignPoint(modelRunner);
        lsfResult result;

        result.result.setBeta(newResult->Beta);
        auto alpha = vector1D(newResult->Alphas.size());
        for (size_t i = 0; i < alpha.size(); i++)
        {
            alpha(i) = newResult->Alphas[i]->Alpha;
        }

        result.result.setAlpha(alpha);
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
