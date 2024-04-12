#include "createReliabilityMethod.h"
#include "../Deltares.Probabilistic/Reliability/CrudeMonteCarlo.h"
#include "../Deltares.Probabilistic/Reliability/DirectionalSampling.h"
#include "../Deltares.Probabilistic/Reliability/FORM.h"
#include "../Deltares.Probabilistic/Reliability/FORMThenDirectionalSampling.h"
#include "../Deltares.Probabilistic/Reliability/DirectionalSamplingThenFORM.h"

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
        fillDsSettings(ds->Settings, bs);
        return ds; }
        break;
    case (ProbMethod::FORM): {
        auto form = new FORM();
        fillFormSettings(form->Settings, bs, nStoch);
        return form; }
        break;
    case (ProbMethod::FDIR): {
        auto fdir = new FORMThenDirectionalSampling(bs.numExtraReal1);
        fillDsSettings(fdir->DsSettings, bs);
        fillFormSettings(fdir->formSettings, bs, nStoch);
        return fdir; }
        break;
    case (ProbMethod::DSFIHR):
    case (ProbMethod::DSFI): {
        auto dsfi = new DirectionalSamplingThenFORM();
        fillDsSettings(dsfi->DsSettings, bs);
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
    Settings->GradientSettings->gradientType = GradientType::TwoDirections;
    Settings->FilterAtNonConvergence = true;
    Settings->RelaxationFactor = bs.relaxationFactor;
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

void createReliabilityMethod::fillDsSettings(std::shared_ptr<DirectionalSamplingSettings>& DsSettings, const basicSettings& bs)
{
    std::shared_ptr<RandomSettings> r(getRnd(bs));
    DsSettings->RandomSettings.swap(r);
    DsSettings->VariationCoefficient = bs.tolB;
    DsSettings->MinimumSamples = bs.minSamples;
    DsSettings->MaximumSamples = bs.maxSamples;
    switch (bs.iterationMethod)
    {
    case DSiterationMethods::DirSamplingIterMethodRobust:
    case DSiterationMethods::DirSamplingIterMethodRobustBisection:
        DsSettings->DirectionSettings->Dsdu = 1.0;
        break;
    default:
        DsSettings->DirectionSettings->Dsdu = 3.0;
        break;
    }
    DsSettings->DirectionSettings->EpsilonUStepSize = bs.tolC;
}

