#include "createReliabilityMethod.h"
#include "../Deltares.Probabilistic/Reliability/CrudeMonteCarlo.h"
#include "../Deltares.Probabilistic/Reliability/DirectionalSampling.h"
#include "../Deltares.Probabilistic/Reliability/FORM.h"
#include "../Deltares.Probabilistic/Reliability/FDIR.h"
#include "../Deltares.Probabilistic/Reliability/DSFI.h"

using namespace Deltares::ProbLibCore;
using namespace Deltares::Models;
using namespace Deltares::Reliability;

Deltares::Models::RandomSettings* createReliabilityMethod::getRnd(const basicSettings& bs)
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

    return rnd;
}

ReliabilityMethod* createReliabilityMethod::selectMethod(const basicSettings& bs, const size_t nStoch)
{
    switch (bs.methodId)
    {
    case (ProbMethod::CM): {
        auto cm = new CrudeMonteCarlo();
        std::shared_ptr<RandomSettings> r (getRnd(bs));
        cm->Settings->randomSettings.swap(r);
        cm->Settings->VariationCoefficient = bs.tolB;
        cm->Settings->MinimumSamples = bs.minSamples;
        cm->Settings->MaximumSamples = bs.maxSamples;
        return cm; }
        break;
    case (ProbMethod::DS): {
        auto ds = new DirectionalSampling();
        std::shared_ptr<RandomSettings> r(getRnd(bs));
        ds->Settings->RandomSettings.swap(r);
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
    case (ProbMethod::FORM): {
        auto form = new FORM();
        fillFormSettings(form->Settings, bs, nStoch);
        return form; }
        break;
    case (ProbMethod::FDIR): {
        auto fdir = new FDIR();
        std::shared_ptr<RandomSettings> r(getRnd(bs));
        fdir->DsSettings->RandomSettings.swap(r);
        fdir->DsSettings->VariationCoefficient = bs.tolB;
        fdir->DsSettings->MinimumSamples = bs.minSamples;
        fdir->DsSettings->MaximumSamples = bs.maxSamples;
        fillFormSettings(fdir->formSettings, bs, nStoch);
        return fdir; }
        break;
    case (ProbMethod::DSFIHR):
    case (ProbMethod::DSFI): {
        auto dsfi = new DSFI();
        std::shared_ptr<RandomSettings> r(getRnd(bs));
        dsfi->DsSettings->RandomSettings.swap(r);
        dsfi->DsSettings->VariationCoefficient = bs.tolB;
        dsfi->DsSettings->MinimumSamples = bs.minSamples;
        dsfi->DsSettings->MaximumSamples = bs.maxSamples;
        fillFormSettings(dsfi->formSettings, bs, nStoch);
        return dsfi; }
        break;
    default:
        throw probLibException("method not implemented yet: ", (int)bs.methodId);
        break;
    }
}

void createReliabilityMethod::fillFormSettings(std::shared_ptr<FORMSettings>& Settings, const basicSettings& bs, const size_t nStoch)
{
    Settings->MaximumIterations = bs.numExtraInt;
    Settings->GradientSettings->GradientType = GradientType::TwoDirections;
    Settings->FilterAtNonConvergence = true;
    switch (bs.startMethod)
    {
    case StartMethods::Zero:
        Settings->StartPointSettings->StartMethod = StartMethodType::None;
        break;
    case StartMethods::One:
        Settings->StartPointSettings->StartMethod = StartMethodType::One;
        break;
    case StartMethods::RaySearch:
    case StartMethods::RaySearchVector:
    case StartMethods::RaySearchVectorScaled:
        Settings->StartPointSettings->StartMethod = StartMethodType::RaySearch;
        Settings->StartPointSettings->MaximumLengthStartPoint = 18.1;
        Settings->StartPointSettings->dsdu = 0.3;
        Settings->StartPointSettings->startVector = copyStartVector(bs.startVector, nStoch);
        break;
    case StartMethods::SphereSearch:
        Settings->StartPointSettings->StartMethod = StartMethodType::SphereSearch;
        Settings->StartPointSettings->startVector = copyStartVector(bs.startVector, nStoch);
        break;
    case StartMethods::GivenVector:
        Settings->StartPointSettings->StartMethod = StartMethodType::GivenVector;
        Settings->StartPointSettings->startVector = copyStartVector(bs.startVector, nStoch);
        break;
    default:
        throw probLibException("not implemented: start method: ", (int)bs.startMethod);
        break;
    }
}

std::vector<double> createReliabilityMethod::copyStartVector(const double startValues[], const size_t nStoch)
{
    auto startVector = std::vector<double>(nStoch);
    for (size_t i = 0; i < nStoch; i++)
    {
        startVector[i] = startValues[i];
    }
    return startVector;
}

