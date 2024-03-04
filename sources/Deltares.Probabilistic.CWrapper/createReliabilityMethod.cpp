#include "createReliabilityMethod.h"
#include "../Deltares.Probabilistic/Reliability/CrudeMonteCarlo.h"
#include "../Deltares.Probabilistic/Reliability/DirectionalSampling.h"
#include "../Deltares.Probabilistic/Reliability/FORM.h"

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
        form->Settings->MaximumIterations = bs.numExtraInt;
        form->Settings->GradientSettings->GradientType = GradientType::TwoDirections;
        switch (bs.startMethod)
        {
        case StartMethods::Zero:
            form->Settings->StartPointSettings->StartMethod = StartMethodType::None;
            break;
        case StartMethods::One:
            form->Settings->StartPointSettings->StartMethod = StartMethodType::One;
            break;
        case StartMethods::RaySearch:
            form->Settings->StartPointSettings->StartMethod = StartMethodType::RaySearch;
            form->Settings->StartPointSettings->MaximumLengthStartPoint = 18.1;
            form->Settings->StartPointSettings->dsdu = 0.3;
            form->Settings->StartPointSettings->startVector = std::vector<double>(nStoch);
            for (size_t i = 0; i < nStoch; i++)
            {
                form->Settings->StartPointSettings->startVector[i] = bs.startVector[i];
            }
            break;
        case StartMethods::SphereSearch:
            form->Settings->StartPointSettings->StartMethod = StartMethodType::SphereSearch;
            form->Settings->StartPointSettings->startVector = std::vector<double>(nStoch);
            for (size_t i = 0; i < nStoch; i++)
            {
                form->Settings->StartPointSettings->startVector[i] = bs.startVector[i];
            }
            break;
        default:
            throw probLibException ( "not implemented: start method: " , (int)bs.startMethod );
            break;
        }
        return form; }
        break;
    default:
        throw probLibException("method not implemented yet: ", (int)bs.methodId);
        break;
    }
}


