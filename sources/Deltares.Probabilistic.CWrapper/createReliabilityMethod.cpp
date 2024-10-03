// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of Streams.
//
// Streams is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// All names, logos, and references to "Deltares" are registered trademarks of
// Stichting Deltares and remain full property of Stichting Deltares at all times.
// All rights reserved.
//
#include "createReliabilityMethod.h"
#include "../Deltares.Probabilistic/Reliability/NumericalIntegration.h"
#include "../Deltares.Probabilistic/Reliability/CrudeMonteCarlo.h"
#include "../Deltares.Probabilistic/Reliability/DirectionalSampling.h"
#include "../Deltares.Probabilistic/Reliability/FORM.h"
#include "../Deltares.Probabilistic/Reliability/FORMThenDirectionalSampling.h"
#include "../Deltares.Probabilistic/Reliability/DirectionalSamplingThenFORM.h"
#include "../Deltares.Probabilistic/Reliability/ImportanceSampling.h"
#include "../Deltares.Probabilistic/Reliability/AdaptiveImportanceSampling.h"

using namespace Deltares::ProbLibCore;
using namespace Deltares::Models;
using namespace Deltares::Reliability;
using namespace Deltares::Statistics;

std::shared_ptr<RandomSettings> createReliabilityMethod::getRnd(const basicSettings& bs)
{
    auto rnd = std::make_shared<RandomSettings>();
    switch (bs.rnd)
    {
    case rndTypes::GeorgeMarsaglia:
        rnd->RandomGeneratorType = Deltares::Numeric::RandomValueGeneratorType::GeorgeMarsaglia;
        break;
    case rndTypes::MersenneTwister:
        rnd->RandomGeneratorType = Deltares::Numeric::RandomValueGeneratorType::MersenneTwister;
        break;
    default:
        throw probLibException("Random generator type is not implemented in C wrapper");
        break;
    }
    rnd->Seed = bs.seed1;
    rnd->SeedB = bs.seed2;
    rnd->IsRepeatableRandom = bs.isRepeatableRandom != 0;

    return rnd;
}
 
std::shared_ptr<ReliabilityMethod> createReliabilityMethod::selectMethod(const basicSettings& bs,
                                                                         const size_t nStoch,
                                                                         std::vector<std::shared_ptr<Stochast>>& stochasts)
{
    switch (bs.methodId)
    {
    case (ProbMethod::NI): {
        auto ni = std::make_shared<NumericalIntegration>();
        ni->Settings.designPointMethod = DesignPointMethod::NearestToMean;
        for (size_t i = 0; i < nStoch; i++)
        {
            auto s = std::make_shared<StochastSettings>();
            s->stochast = stochasts[i];
            s->Intervals = bs.numExtraInt;
            s->MinValue = bs.numExtraReal1;
            s->MaxValue = bs.numExtraReal2;
            ni->Settings.StochastSet->stochastSettings.push_back(s);
        }
        return ni; }
        break;
    case (ProbMethod::CM): {
        auto cm = std::make_shared<CrudeMonteCarlo>();
        std::shared_ptr<RandomSettings> r(getRnd(bs));
        cm->Settings->randomSettings.swap(r);
        cm->Settings->VariationCoefficient = bs.tolB;
        cm->Settings->MinimumSamples = bs.minSamples;
        cm->Settings->MaximumSamples = bs.maxSamples;
        return cm; }
        break;
    case (ProbMethod::DS): {
        auto ds = std::make_shared<DirectionalSampling>();
        fillDsSettings(ds->Settings, bs);
        return ds; }
        break;
    case (ProbMethod::FORM): {
        auto form = std::make_shared<FORM>();
        fillFormSettings(form->Settings, bs, nStoch);
        return form; }
        break;
    case (ProbMethod::FDIR): {
        auto fdir = std::make_shared<FORMThenDirectionalSampling>(bs.numExtraReal1);
        fillDsSettings(fdir->DsSettings, bs);
        fillFormSettings(fdir->formSettings, bs, nStoch);
        return fdir; }
        break;
    case (ProbMethod::DSFIHR):
    case (ProbMethod::DSFI): {
        auto dsfi = std::make_shared<DirectionalSamplingThenFORM>();
        fillDsSettings(dsfi->DsSettings, bs);
        fillFormSettings(dsfi->formSettings, bs, nStoch);
        return dsfi; }
        break;
    case (ProbMethod::IM): {
        auto impSampling = std::make_shared<ImportanceSampling>();
        fillImportanceSamplingSettings(impSampling->Settings, bs, stochasts);
        return impSampling; }
        break;
    case (ProbMethod::AdaptiveIM): {
        auto AdaptImpSampling = std::make_shared<AdaptiveImportanceSampling>();
        fillImportanceSamplingSettings(AdaptImpSampling->Settings->importanceSamplingSettings, bs, stochasts);
        AdaptImpSampling->Settings->MaxVarianceLoops = bs.trialLoops;
        AdaptImpSampling->Settings->LoopVarianceIncrement = bs.numExtraReal2;
        AdaptImpSampling->Settings->AutoMaximumSamples = bs.numExtraInt != 0;
        AdaptImpSampling->Settings->MinimumFailedSamples = bs.numExtraInt2;
        fillStartVector(AdaptImpSampling->Settings->startPointSettings, bs, nStoch);
        return AdaptImpSampling; }
        break;
    default:
        throw probLibException("method not implemented yet: ", (int)bs.methodId);
        break;
    }
}

DesignPointMethod createReliabilityMethod::convertDp(const DPoptions dp)
{
    switch (dp)
    {
    case DPoptions::CenterOfGravity:
        return DesignPointMethod::CenterOfGravity;
    case DPoptions::CenterOfAngles:
        return DesignPointMethod::CenterOfAngles;
    default:
        return DesignPointMethod::NearestToMean;
    }
}

void createReliabilityMethod::fillStartVector(std::shared_ptr<StartPointCalculatorSettings> startPoint, const basicSettings& bs, const size_t nStoch)
{
    switch (bs.startMethod)
    {
    case StartMethods::Zero:
        startPoint->StartMethod = StartMethodType::FixedValue;
        break;
    case StartMethods::One:
        {
            startPoint->StartMethod = StartMethodType::FixedValue;
            for (size_t i = 0; i < nStoch; i++)
            {
                startPoint->startVector.push_back(1.0);
            }
        }
        break;
    case StartMethods::RaySearch:
    case StartMethods::RaySearchVector:
    case StartMethods::RaySearchVectorScaled:
        {
            startPoint->StartMethod = StartMethodType::RaySearch;
            startPoint->MaximumLengthStartPoint = 18.1;
            startPoint->dsdu = 0.3;
            startPoint->startVector = copyStartVector(bs.startVector, nStoch);
        }
        break;
    case StartMethods::SphereSearch:
        {
            startPoint->StartMethod = StartMethodType::SphereSearch;
            startPoint->startVector = copyStartVector(bs.startVector, nStoch);
            startPoint->allQuadrants = (bs.allQuadrants != 0);
            startPoint->maxStepsSphereSearch = bs.maxStepsSphereSearch;
        }
        break;
    case StartMethods::GivenVector:
        {
            startPoint->StartMethod = StartMethodType::FixedValue;
            startPoint->startVector = copyStartVector(bs.startVector, nStoch);
        }
        break;
    case StartMethods::Sensitivity:
        startPoint->StartMethod = StartMethodType::SensitivitySearch;
        break;
    default:
        throw probLibException("not implemented: start method: ", (int)bs.startMethod);
        break;
    }
}

void createReliabilityMethod::fillFormSettings(std::shared_ptr<FORMSettings>& Settings, const basicSettings& bs, const size_t nStoch)
{
    Settings->MaximumIterations = bs.numExtraInt;
    Settings->GradientSettings->gradientType = GradientType::TwoDirections;
    Settings->FilterAtNonConvergence = true;
    Settings->RelaxationFactor = bs.relaxationFactor;
    fillStartVector(Settings->StartPointSettings, bs, nStoch);
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
    DsSettings->randomSettings.swap(r);
    DsSettings->VariationCoefficient = bs.tolB;
    DsSettings->MinimumDirections = bs.minSamples;
    DsSettings->MaximumDirections = bs.maxSamples;
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

void createReliabilityMethod::fillImportanceSamplingSettings(std::shared_ptr<ImportanceSamplingSettings> settings,
                                                             const basicSettings& bs,
                                                             std::vector<std::shared_ptr<Stochast>>& stochasts)
{
    auto r = getRnd(bs);
    settings->randomSettings.swap(r);
    settings->VariationCoefficient = bs.tolB;
    settings->MinimumSamples = bs.minSamples;
    settings->MaximumSamples = bs.maxSamples;
    if (bs.methodId == ProbMethod::AdaptiveIM)
    {
        settings->MaximumSamplesNoResult = bs.maxSamples;
        settings->designPointMethod = convertDp(bs.designPointOptions);
    }
    for (size_t i = 0; i < stochasts.size(); i++)
    {
        auto s = std::make_shared<StochastSettings>();
        s->stochast = stochasts[i];
        s->VarianceFactor = bs.varianceFactor;
        settings->StochastSet->stochastSettings.push_back(s);
    }
}
