#include <string>
#include <memory>

#include "basicSettings.h"
#include "createDistribution.h"
#include "../Deltares.Probabilistic.Kernels/Utils/probLibException.h"
#include "../Deltares.Probabilistic.Kernels/Math/vector1D.h"

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
    int convergence;
    double beta;
    double alpha[maxActiveStochast];
    double x[maxActiveStochast];
    int iPoint[maxActiveStochast];
    int stepsNeeded;
    int samplesNeeded;
    double alpha2[maxActiveStochast];
};

extern "C"
void probcalcf2cnew(const basicSettings* method, fdistribs* c, const int n, const int vectorSize,
    corrStruct correlations[], const int nrCorrelations,
    const double(*fx)(double[], int[], tError*),
    const bool(*pc)(ProgressType, const char*),
    int compIds[], int iPoint[], double x[], tResult* r, tError* ierr)
{
    try
    {
        auto nStoch = (size_t)n;
        auto fw = funcWrapper(vectorSize, iPoint, x, compIds, fx);

        auto cntDeterminists = 0;
        auto stochast = std::vector<std::shared_ptr<Deltares::Statistics::Stochast>>();
        for (size_t i = 0; i < nStoch; i++)
        {
            auto distHR = (EnumDistributions)c[i].distId;
            auto s = createDistribution::create(distHR, c[i].params);
            stochast.push_back(s);
        }

        auto createRelM = createReliabilityMethod();
        std::shared_ptr<ReliabilityMethod> relMethod(createRelM.selectMethod(*method));
        std::shared_ptr<ZModel> zModel(new ZModel([&fw](std::shared_ptr<Sample> v) { return fw.FDelegate(v); }));
        std::shared_ptr<CorrelationMatrix> corr(new CorrelationMatrix());
        if (nrCorrelations > 0)
        {
            corr->init(n);
            for (size_t i = 0; i < nrCorrelations; i++)
            {
                corr->SetCorrelation(correlations[i].idx1, correlations[i].idx2, correlations[i].correlation);
            }
        }
        std::shared_ptr<UConverter> uConverter(new UConverter(stochast, corr));
        uConverter->initializeForRun();
        auto pw = progressWrapper(pc, relMethod.get());
        auto progressDelegate = ProgressLambda();
        auto detailedProgressDelegate = DetailedProgressLambda();
        auto textualProgress = TextualProgressLambda([&pw](ProgressType p, std::string s) {pw.FPgDelegate(p, s); });
        std::unique_ptr<ProgressIndicator> progress (new ProgressIndicator(progressDelegate, detailedProgressDelegate, textualProgress));
        zModel->setMaxProcesses(method->numThreads);
        std::shared_ptr<ZModelRunner> modelRunner(new ZModelRunner(zModel, uConverter, progress.get()));
        modelRunner->Settings->MaxParallelProcesses = method->numThreads;
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

        if (alpha.size() <= maxActiveStochast)
        {
            for (size_t i = 0; i < alpha.size(); i++)
            {
                r->x[i] = newResult->Alphas[i]->X;
            }
        }
        if (alpha.size() == (size_t)vectorSize)
        {
            for (size_t i = 0; i < alpha.size(); i++)
            {
                x[i] = r->x[i];
            }
        }

        for (int i = 0; i < n; i++)
        {
            r->iPoint[i] = i + 1;
        }
        for (size_t i = 0; i < alpha.size(); i++)
        {
            r->alpha2[i] = newResult->Alphas[i]->AlphaCorrelated;
        }
        r->convergence = newResult->convergenceReport->IsConverged ? 0: 1;
        r->stepsNeeded = -999;// result.stepsNeeded;
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
