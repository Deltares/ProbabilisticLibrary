#include "createReliabilityMethod.h"
#include "../Deltares.Probabilistic.Kernels/Reliability/CrudeMonteCarlo.h"
#include "../Deltares.Probabilistic.Kernels/Reliability/DirectionalSampling.h"

using namespace Deltares::ProbLibCore;
using namespace Deltares::Reliability;

ReliabilityMethod* createReliabilityMethod::selectMethod(const basicSettings& bs)
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


