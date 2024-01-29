#include <string>
#include <omp.h>
#include <memory>

#include "basicSettings.h"
#include "enumDistributions.h"
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
void probcalcf2cnew(const basicSettings* method, const fdistribs* c, const int n, const int vectorSize,
    corrStruct correlations[], const int nrCorrelations,
    const double(*fx)(double[], int[], tError*),
    const bool(*pc)(ProgressType, const char*),
    int compIds[], int iPoint[], double x[], tResult* r, tError* ierr)
{
    try
    {
        auto nStoch = (size_t)n;
        auto fw = funcWrapper(vectorSize, iPoint, x, fx);
        omp_set_num_threads(method->numThreads);

        auto cntDeterminists = 0;
        auto stochast = std::vector<Deltares::Statistics::Stochast*>();
        for (size_t i = 0; i < nStoch; i++)
        {
            auto distHR = (EnumDistributions)c[i].distId;
            Deltares::Statistics::DistributionType dist;
            bool truncated = false;
            double truncatedMin; double truncatedMax;
            switch (distHR)
            {
                case EnumDistributions::normal:
                    dist = Deltares::Statistics::DistributionType::Normal;
                    break;
                case EnumDistributions::deterministic:
                    dist = Deltares::Statistics::DistributionType::Deterministic;
                    break;
                case EnumDistributions::lognormal2:
                    dist = Deltares::Statistics::DistributionType::LogNormal;
                    break;
                case EnumDistributions::uniform:
                    dist = Deltares::Statistics::DistributionType::Uniform;
                    break;
                case EnumDistributions::gumbel2:
                    dist = Deltares::Statistics::DistributionType::Gumbel;
                    break;
                case EnumDistributions::truncatedNormal: {
                    dist = Deltares::Statistics::DistributionType::Normal;
                    truncated = true;
                    truncatedMin = c[i].params[2];
                    truncatedMax = c[i].params[3]; }
                    break;
                case EnumDistributions::uspace:
                    dist = Deltares::Statistics::DistributionType::Normal;
                    break;
                default:
                    throw probLibException("Distribution not supported yet: ", c[i].distId);
            }
            auto params = new double[4];
            for (size_t j = 0; j < 4; j++)
            {
                params[j] = c[i].params[j];
            }
            if (distHR == EnumDistributions::uspace)
            {
                params[0] = 1.0; params[1] = 0.0;
            }
            auto s = new Deltares::Statistics::Stochast(dist, params);
            if (truncated)
            {
                s->setTruncated(true);
                s->Minimum = truncatedMin;
                s->Maximum = truncatedMax;
            }
            stochast.push_back(s);
            delete[] params;
        }

        auto createRelM = createReliabilityMethod();
        std::unique_ptr<ReliabilityMethod> relMethod(createRelM.selectMethod(*method));
        std::unique_ptr<ZModel> zModel(new ZModel([&fw](Sample* v) { return fw.FDelegate(v); }));
        std::unique_ptr<CorrelationMatrix> corr(new CorrelationMatrix());
        if (nrCorrelations > 0)
        {
            corr->init(n);
            for (size_t i = 0; i < nrCorrelations; i++)
            {
                corr->SetCorrelation(correlations[i].idx1, correlations[i].idx2, correlations[i].correlation);
            }
        }
        std::unique_ptr<UConverter> uConverter(new UConverter(stochast, corr.get()));
        uConverter->initializeForRun();
        auto pw = progressWrapper(pc, relMethod.get());
        auto progressDelegate = ProgressDelegate();
        auto detailedProgressDelegate = DetailedProgressDelegate();
        auto textualProgress = TextualProgressLambda([&pw](ProgressType p, std::string s) {pw.FPgDelegate(p, s); });
        std::unique_ptr<ProgressIndicator> progress (new ProgressIndicator(progressDelegate, detailedProgressDelegate, textualProgress));
        std::unique_ptr<ZModelRunner> modelRunner(new ZModelRunner(zModel.get(), uConverter.get(), progress.get()));
        std::unique_ptr<DesignPoint> newResult( relMethod->getDesignPoint(modelRunner.get()));

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
        r->convergence = (newResult->convergenceReport->Convergence < method->tolB ? 0 : 1);
        r->stepsNeeded = -999;// result.stepsNeeded;
        r->samplesNeeded = newResult->convergenceReport->TotalDirections;
        if (r->samplesNeeded < 0)
        {
            r->samplesNeeded = (int)round(newResult->convergenceReport->FailedSamples / newResult->convergenceReport->FailFraction);
        }

        // clean up
        for (size_t i = 0; i < stochast.size(); i++)
        {
            delete stochast[i];
        }
    }
    catch (const std::exception& e)
    {
        std::string s = e.what();
        ierr->errorCode = -1;
        fillErrorMessage(*ierr, s);
    }
}
