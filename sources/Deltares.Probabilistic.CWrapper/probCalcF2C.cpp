#include <string>
#include <memory>

#include "basicSettings.h"
#include "createDistribution.h"
#include "../Deltares.Probabilistic/Utils/probLibException.h"
#include "../Deltares.Probabilistic/Math/vector1D.h"

#include "funcWrapper.h"
#include "progressWrapper.h"
#include "createReliabilityMethod.h"

using namespace Deltares::ProbLibCore;
using namespace Deltares::Models;
using namespace Deltares::Reliability;
using namespace Deltares::Numeric;
using namespace Deltares::Statistics;

struct fdistribs
{
    int distId;
    double params[4];
};

struct tResult
{
    double beta;
    double alpha[maxActiveStochast];
    int iPoint[maxActiveStochast];
    int stepsNeeded;
    int samplesNeeded;
    bool convergence;
};

void updateX(const std::vector<std::shared_ptr<StochastPointAlpha>> & alpha, const DPoptions option, tResult & r, const std::shared_ptr<DesignPoint> & newResult,
    double x[], funcWrapper fw)
{
    if (alpha.size() <= maxActiveStochast)
    {
        switch (option) {
        case DPoptions::None:
        {
            for (size_t i = 0; i < alpha.size(); i++)
            {
                x[i] = 0.0;
            }
        }
        break;
        case DPoptions::RMinZFunc:
        case DPoptions::RMinZFuncCompatible:
        {
            auto xSparse = std::vector<double>();
            for (size_t i = 0; i < alpha.size(); i++)
            {
                xSparse.push_back(newResult->Alphas[i]->X);
            }
            fw.updateXinDesignPoint(xSparse);
            for (size_t i = 0; i < alpha.size(); i++)
            {
                x[i] = xSparse[i];
            }
        }
        break;
        default:
        {
            for (size_t i = 0; i < alpha.size(); i++)
            {
                x[i] = newResult->Alphas[i]->X;
            }
        }
        }
    }
}

extern "C"
void probcalcf2c(const basicSettings* method, fdistribs* c, const int n, corrStruct correlations[], const int nrCorrelations,
    const double(*fx)(double[], computationSettings*, tError*),
    const bool(*pc)(ProgressType, const char*),
    const int compIds[], double x[], tResult* r, tError* ierr)
{
    try
    {
        auto nStoch = (size_t)n;
        auto fw = funcWrapper(compIds[0], fx);

        auto stochasts = std::vector<std::shared_ptr<Deltares::Statistics::Stochast>>();
        for (size_t i = 0; i < nStoch; i++)
        {
            auto distHR = (EnumDistributions)c[i].distId;
            auto s = createDistribution::createValid(distHR, c[i].params);
            stochasts.push_back(s);
        }

        auto createRelM = createReliabilityMethod();
        auto relMethod = createRelM.selectMethod(*method, nStoch, stochasts);
        auto zModel = std::make_shared<ZModel>([&fw](std::shared_ptr<ModelSample> v) { return fw.FDelegate(v); },
                                               [&fw](std::vector<std::shared_ptr<ModelSample>> v) { return fw.FDelegateParallel(v); });
        auto corr = std::make_shared<CorrelationMatrix>();
        if (nrCorrelations > 0)
        {
            corr->init(n);
            for (int i = 0; i < nrCorrelations; i++)
            {
                corr->SetCorrelation(correlations[i].idx1, correlations[i].idx2, correlations[i].correlation);
            }
        }
        auto uConverter = std::make_shared<UConverter>(stochasts, corr);
        auto pw = progressWrapper(pc, relMethod.get());
        auto progressDelegate = ProgressLambda();
        auto detailedProgressDelegate = DetailedProgressLambda();
        auto textualProgress = TextualProgressLambda([&pw](ProgressType p, std::string s) {pw.FPgDelegate(p, s); });
        auto progress = std::make_shared<ProgressIndicator>(progressDelegate, detailedProgressDelegate, textualProgress);
        auto modelRunner = std::make_shared<ModelRunner>(zModel, uConverter, progress);
        modelRunner->Settings->MaxParallelProcesses = method->numThreads;
        modelRunner->Settings->MaxChunkSize = method->chunkSize;
        modelRunner->initializeForRun();
        auto newResult = relMethod->getDesignPoint(modelRunner);

        ierr->errorCode = 0;
        r->beta = newResult->Beta;
        for (size_t i = 0; i < nStoch; i++)
        {
            r->alpha[i] = newResult->Alphas[i]->Alpha;
        }

        updateX(newResult->Alphas, method->designPointOptions, *r, newResult, x, fw);

        for (int i = 0; i < n; i++)
        {
            r->iPoint[i] = i + 1;
        }
        r->convergence = newResult->convergenceReport->IsConverged;
        r->stepsNeeded = newResult->convergenceReport->TotalIterations;
        r->samplesNeeded = newResult->convergenceReport->TotalDirections;
        if (r->samplesNeeded < 0)
        {
            r->samplesNeeded = (int)round(newResult->convergenceReport->FailedSamples / newResult->convergenceReport->FailFraction);
        }
    }
    catch (const std::exception& e)
    {
        std::string s = e.what();
        ierr->errorCode = -1;
        fillErrorMessage(*ierr, s);
    }
}
