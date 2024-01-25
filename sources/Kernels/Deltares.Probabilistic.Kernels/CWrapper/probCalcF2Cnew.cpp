#include <string>
#include <omp.h>
#include <memory>
#include <functional>

#include "basicSettings.h"
#include "enumDistributions.h"
#include "../Utils/probLibException.h"
#include "../Math/vector1D.h"

#include "../Reliability/CrudeMonteCarlo.h"
#include "../Reliability/DirectionalSampling.h"

using namespace Deltares::ProbLibCore;
using namespace Deltares::Models;
using namespace Deltares::Reliability;

const size_t lenSmallStr = 32;
const size_t ERRORMSGLENGTH = 256;

struct tError
{
    char errorMessage[ERRORMSGLENGTH];
    int errorCode;
};

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

std::function <bool (ProgressType, std::string)> staticPg;

ReliabilityMethod* rmStatic;
void FPgDelegate(ProgressType pt, std::string s)
{
    auto cancel = staticPg(pt, s);
    if (cancel)
    {
        rmStatic->Stop();
    }
}

std::function<double(double[], int[], tError*)> staticF;
size_t allStoch;
int* iPointer;
double* xRef;

double FDelegate (Sample* s)
{
    auto xx = new double[allStoch];
    for (size_t i = 0; i < allStoch; i++)
    {
        xx[i] = xRef[i];
    }
    for (size_t i = 0; i < s->getSize(); i++)
    {
        xx[iPointer[i]] = s->Values[i];
    }
    auto i = new int[4];
    tError e = tError();
    double result = staticF(xx, i, &e);
    delete[] xx;
    delete[] i;
    s->Z = result;
    return result;
}

void fillErrorMessage(tError& error, const std::string s)
{
    size_t length = std::min(s.length(), ERRORMSGLENGTH);
    for (size_t i = 0; i < length; i++)
    {
        error.errorMessage[i] = s[i];
    }
    size_t last = std::min(s.length(), ERRORMSGLENGTH - 1);
    error.errorMessage[last] = char(0);
}

ReliabilityMethod* selectMethod(const basicSettings & bs)
{
    auto rnd = new RandomSettings();
    switch (bs.rnd)
    {
    case rndTypes::GeorgeMarsaglia:
        rnd->RandomGeneratorType = Deltares::Numeric::RandomValueGeneratorType::GeorgeMarsaglia;
        break;
    case rndTypes::MersenneTwister:
        rnd->RandomGeneratorType = Deltares::Numeric::RandomValueGeneratorType::MersenneTwister;
        break;
    default:
        throw probLibException("ModifiedKnuthSubtractive not implemented in C wrapper");
        break;
    }
    rnd->Seed = bs.seed1;
    rnd->SeedB = bs.seed2;

    switch (bs.methodId)
    {
    case (ProbMethod::CM): {
        auto cm = new CrudeMonteCarlo();
        cm->Settings->RandomSettings = rnd;
        cm->Settings->VariationCoefficient = bs.tolB;
        cm->Settings->MinimumSamples = bs.minSamples;
        cm->Settings->MaximumSamples = bs.maxSamples;
        return cm; }
        break;
    case (ProbMethod::DS): {
        auto ds = new DirectionalSampling();
        ds->Settings->RandomSettings = rnd;
        ds->Settings->VariationCoefficient = bs.tolB;
        ds->Settings->MinimumSamples = bs.minSamples;
        ds->Settings->MaximumSamples = bs.maxSamples;
        switch (bs.iterationMethod)
        {
        case DSiterationMethods::DirSamplingIterMethodRobust:
        case DSiterationMethods::DirSamplingIterMethodRobustBisection:
            ds->Settings->DirectionSettings->Dsdu = 1.0;
            break;
        default:
            ds->Settings->DirectionSettings->Dsdu = 3.0;
            break;
        }
        ds->Settings->DirectionSettings->EpsilonUStepSize = bs.tolC;
        return ds; }
        break;
    default:
        throw probLibException("method not implemented yet:", (int)bs.methodId);
        break;
    }
}

extern "C"
void probcalcf2cnew(const basicSettings* method, const fdistribs* c, const int n, const int vectorSize,
    corrStruct correlations[], const int nrCorrelations,
    const double(*fx)(double[], int[], tError*),
    const bool(*pc)(ProgressType, std::string),
    int compIds[], int iPoint[], double x[], tResult* r, tError* ierr)
{
    auto nStoch = (size_t)n;
    allStoch = vectorSize;
    iPointer = iPoint;
    xRef = x;
    size_t numThreads = (size_t)method->numThreads;
    omp_set_num_threads(method->numThreads);

    staticF = fx;
    staticPg = pc;
    try
    {
        auto cntDeterminists = 0;
        auto stochast = std::vector<Deltares::Statistics::Stochast*>();
        for (size_t i = 0; i < nStoch; i++)
        {
            std::string name = c[i].name;
            auto distHR = (EnumDistributions)c[i].distId;
            Deltares::Statistics::DistributionType dist;
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
                default:
                    throw probLibException("Distribution not supported yet: ", c[i].distId);
            }
            auto params = new double[4];
            for (size_t j = 0; j < 4; j++)
            {
                params[j] = c[i].params[j];
            }
            auto s = new Deltares::Statistics::Stochast(dist, params);
            stochast.push_back(s);
            delete[] params;
        }

        std::unique_ptr<ReliabilityMethod> relMethod(selectMethod(*method));
        rmStatic = relMethod.get();
        std::unique_ptr<ZModel> zModel(new ZModel(FDelegate));
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
        auto progressDelegate = ProgressDelegate();
        auto detailedProgressDelegate = DetailedProgressDelegate();
        auto textualProgressDelegate = TextualProgressDelegate(FPgDelegate);
        std::unique_ptr<ProgressIndicator> progress (new ProgressIndicator(progressDelegate, detailedProgressDelegate, textualProgressDelegate));
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
        r->convergence = (newResult->ConvergenceReport->Convergence < method->tolB ? 0 : 1);
        r->stepsNeeded = -999;// result.stepsNeeded;
        r->samplesNeeded = newResult->ConvergenceReport->TotalDirections;
        if (r->samplesNeeded < 0)
        {
            r->samplesNeeded = (int)round(newResult->ConvergenceReport->FailedSamples / newResult->ConvergenceReport->FailFraction);
        }
        delete newResult->ConvergenceReport;
    }
    catch (const std::exception& e)
    {
        std::string s = e.what();
        ierr->errorCode = -1;
        fillErrorMessage(*ierr, s);
    }
}
