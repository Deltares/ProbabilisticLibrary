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

const size_t lenSmallStr = 32;

struct fdistribs
{
    char name[lenSmallStr];
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

void updateX(const vector1D & alpha, const DPoptions option, tResult & r, const std::shared_ptr<DesignPoint> & newResult,
    const size_t vectorSize, double x[], funcWrapper fw)
{
    if (alpha.size() <= maxActiveStochast)
    {
        switch (option) {
        case DPoptions::None:
        {
            for (size_t i = 0; i < vectorSize; i++)
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
void probcalcf2c(const basicSettings* method, fdistribs* c, const int n, const int vectorSize,
    corrStruct correlations[], const int nrCorrelations,
    const double(*fx)(double[], computationSettings*, tError*),
    const bool(*pc)(ProgressType, const char*),
    const int compIds[], double x[], tResult* r, tError* ierr)
{
    try
    {
        auto nStoch = (size_t)n;
        auto fw = funcWrapper(compIds[0], fx);

        auto stochast = std::vector<std::shared_ptr<Deltares::Statistics::Stochast>>();
        for (size_t i = 0; i < nStoch; i++)
        {
            auto distHR = (EnumDistributions)c[i].distId;
            auto s = createDistribution::createValid(distHR, c[i].params);
            stochast.push_back(s);
        }

        auto createRelM = createReliabilityMethod();
        std::shared_ptr<ReliabilityMethod> relMethod(createRelM.selectMethod(*method, nStoch));
        std::shared_ptr<ZModel> zModel(new ZModel([&fw](std::shared_ptr<ModelSample> v) { return fw.FDelegate(v); },
                                                  [&fw](std::vector<std::shared_ptr<ModelSample>> v) { return fw.FDelegateParallel(v); }));
        std::shared_ptr<Deltares::Statistics::CorrelationMatrix> corr(new Deltares::Statistics::CorrelationMatrix());
        if (nrCorrelations > 0)
        {
            corr->init(n);
            for (int i = 0; i < nrCorrelations; i++)
            {
                corr->SetCorrelation(correlations[i].idx1, correlations[i].idx2, correlations[i].correlation);
            }
        }
        std::shared_ptr<UConverter> uConverter(new UConverter(stochast, corr));
        auto pw = progressWrapper(pc, relMethod.get());
        auto progressDelegate = ProgressLambda();
        auto detailedProgressDelegate = DetailedProgressLambda();
        auto textualProgress = TextualProgressLambda([&pw](ProgressType p, std::string s) {pw.FPgDelegate(p, s); });
        std::shared_ptr<ProgressIndicator> progress (new ProgressIndicator(progressDelegate, detailedProgressDelegate, textualProgress));
        std::shared_ptr<ModelRunner> modelRunner(new ModelRunner(zModel, uConverter, progress));
        modelRunner->Settings->MaxParallelProcesses = method->numThreads;
        modelRunner->Settings->MaxChunkSize = method->chunkSize;
        modelRunner->initializeForRun();
        std::shared_ptr<DesignPoint> newResult ( relMethod->getDesignPoint(modelRunner));

        auto alpha = vector1D(newResult->Alphas.size());
        for (size_t i = 0; i < alpha.size(); i++)
        {
            alpha(i) = newResult->Alphas[i]->Alpha;
        }

        ierr->errorCode = 0;
        r->beta = newResult->Beta;
        for (size_t i = 0; i < alpha.size(); i++)
        {
            r->alpha[i] = alpha(i);
        }

        updateX(alpha, method->designPointOptions, *r, newResult, vectorSize, x, fw);

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
