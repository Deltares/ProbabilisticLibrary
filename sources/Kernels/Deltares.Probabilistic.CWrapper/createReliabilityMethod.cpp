#include "createReliabilityMethod.h"
#include "../Deltares.Probabilistic.Kernels/Reliability/CrudeMonteCarlo.h"
#include "../Deltares.Probabilistic.Kernels/Reliability/DirectionalSampling.h"
#include "../Deltares.Probabilistic.Kernels/Reliability/FORM.h"
#include <iostream>

using namespace Deltares::ProbLibCore;
using namespace Deltares::Reliability;

RandomSettings* createReliabilityMethod::getRnd(const basicSettings& bs)
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

ReliabilityMethod* createReliabilityMethod::selectMethod(const basicSettings& bs)
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
        switch (bs.startMethod)
        {
        case StartMethods::Zero:
            form->Settings->StartPointSettings->StartMethod = StartMethodType::None;
            break;
        case StartMethods::RaySearch:
            form->Settings->StartPointSettings->StartMethod = StartMethodType::RaySearch;
            break;
        case StartMethods::SphereSearch:
            form->Settings->StartPointSettings->StartMethod = StartMethodType::SphereSearch;
            break;
        default:
            std::cout << "not implemented: start method: " << (int)bs.startMethod << std::endl;
            break;
        }
        return form; }
        break;
    default:
        throw probLibException("method not implemented yet:", (int)bs.methodId);
        break;
    }
}


