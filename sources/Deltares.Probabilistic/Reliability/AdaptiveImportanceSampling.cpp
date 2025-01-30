// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of the Probabilistic Library.
//
// The Probabilistic Library is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// All names, logos, and references to "Deltares" are registered trademarks of
// Stichting Deltares and remain full property of Stichting Deltares at all times.
// All rights reserved.
//
#include "AdaptiveImportanceSampling.h"

#include <vector>

#if __has_include(<format>)
#include <format>
#endif

#include <memory>

#include "../Model/Sample.h"
#include "../Model/RandomSampleGenerator.h"
#include "../Optimization/KMeansClustering.h"
#include "ConvergenceReport.h"
#include "DesignPoint.h"
#include "AdaptiveImportanceSamplingSettings.h"
#include "DirectionReliability.h"
#include "ImportanceSampling.h"
#include "StochastSettings.h"
#include "StartPointCalculator.h"
#include "../Math/NumericSupport.h"

using namespace Deltares::Models;

namespace Deltares
{
    namespace Reliability
    {
        std::shared_ptr<DesignPoint> AdaptiveImportanceSampling::getDesignPoint(std::shared_ptr<Models::ModelRunner> modelRunner)
        {
            this->importanceSampling = std::make_shared<ImportanceSampling>();
            importanceSampling->Settings = this->Settings->importanceSamplingSettings->clone();
            importanceSampling->Settings->startPointSettings->StartMethod = StartMethodType::FixedValue;

            modelRunner->updateStochastSettings(importanceSampling->Settings->StochastSet);

            std::vector<std::shared_ptr<DesignPoint>> previousDesignPoints = std::vector<std::shared_ptr<DesignPoint>>();

            // initialize
            const std::shared_ptr<StartPointCalculator> startPointCalculator = std::make_shared<StartPointCalculator>();
            startPointCalculator->Settings = this->Settings->startPointSettings;
            startPointCalculator->Settings->StochastSet = importanceSampling->Settings->StochastSet;

            const std::shared_ptr<Sample> startPoint = startPointCalculator->getStartPoint(modelRunner);
            this->lastStartPoint = startPoint;

            if (Settings->startPointSettings->StartMethod != StartMethodType::FixedValue)
            {
                const std::shared_ptr<DesignPoint> startDesignPoint = modelRunner->getDesignPoint(startPoint, startPoint->getBeta());
                startDesignPoint->Identifier = "Start point";
                previousDesignPoints.push_back(startDesignPoint);
            }
            importanceSampling->Settings->StochastSet->setStartPoint(startPoint);

            modelRunner->doTextualProgress(ProgressType::Global, "Calculating design point.");

            if (Settings->MaxVarianceLoops > 1)
            {
                modelRunner->clear();

                if (Settings->AutoMaximumSamples)
                {
                    importanceSampling->Settings->MaximumSamples = Settings->importanceSamplingSettings->MaximumSamples;
                    importanceSampling->Settings->MaximumSamplesNoResult = Settings->importanceSamplingSettings->MaximumSamples;
                }
                else
                {
                    importanceSampling->Settings->MaximumSamples = Settings->importanceSamplingSettings->MaximumSamplesNoResult;
                    importanceSampling->Settings->MaximumSamplesNoResult = Settings->importanceSamplingSettings->MaximumSamplesNoResult;
                }

                int loopCounter = 1;

                this->setCallbacks(this->importanceSampling, loopCounter);

                std::shared_ptr<DesignPoint> designPoint = importanceSampling->getDesignPoint(modelRunner);

                designPoint->convergenceReport->VarianceFactor = Settings->VarianceFactor;

                bool fullExecuted = importanceSampling->Settings->MaximumSamples == this->Settings->importanceSamplingSettings->MaximumSamples;
                bool hasChanged = true;

                while (!isStopped() && hasChanged && isNextLoopAllowed(Settings, loopCounter, designPoint->convergenceReport, designPoint))
                {
                    loopCounter++;

                    const std::shared_ptr<ImportanceSamplingSettings> importanceSamplingSettings = importanceSampling->Settings;

                    this->importanceSampling = std::make_shared<ImportanceSampling>();
                    this->importanceSampling->Settings = importanceSamplingSettings;

                    this->setCallbacks(this->importanceSampling, loopCounter);

                    if (this->Settings->Clustering)
                    {
                        hasChanged = this->updateClusters(loopCounter);
                    }
                    else
                    {
                        hasChanged = this->updateStartPoint(designPoint, modelRunner, loopCounter);
                    }

                    if (hasChanged)
                    {
                        if (loopCounter == Settings->MaxVarianceLoops)
                        {
                            importanceSampling->Settings->MaximumSamples = Settings->importanceSamplingSettings->MaximumSamples;
                            importanceSampling->Settings->MaximumSamplesNoResult = Settings->importanceSamplingSettings->MaximumSamplesNoResult;
                        }

#ifdef __cpp_lib_format
                        std::string identifier = std::format("Variance loop {0:}", loopCounter - 1);
#else
                        std::string identifier = "Variance loop" + std::to_string(loopCounter - 1);
#endif
                        designPoint->Identifier = identifier;

                        previousDesignPoints.push_back(designPoint);

                        modelRunner->clear();
                        clusterSamples.clear();

                        reportVarianceLoop(modelRunner, loopCounter);

                        designPoint = importanceSampling->getDesignPoint(modelRunner);

                        designPoint->convergenceReport->VarianceFactor = Settings->VarianceFactor;

                        fullExecuted = importanceSampling->Settings->MaximumSamples == Settings->importanceSamplingSettings->MaximumSamples;
                    }
                }

                if (!fullExecuted)
                {
                    const std::shared_ptr<ImportanceSamplingSettings> importanceSamplingSettings = importanceSampling->Settings;

                    this->importanceSampling = std::make_shared<ImportanceSampling>();
                    this->importanceSampling->Settings = importanceSamplingSettings;

                    this->setCallbacks(this->importanceSampling, loopCounter);

                    this->setFactor(importanceSampling->Settings->StochastSet, Settings->VarianceFactor);

                    importanceSampling->Settings->MaximumSamples = Settings->importanceSamplingSettings->MaximumSamples;

                    designPoint = importanceSampling->getDesignPoint(modelRunner);
                    designPoint->convergenceReport->VarianceFactor = Settings->VarianceFactor;
                }

                for (size_t i = 0; i < previousDesignPoints.size(); i++)
                {
                    designPoint->ContributingDesignPoints.push_back(previousDesignPoints[i]);
                }

                return designPoint;
            }
            else
            {
                modelRunner->clear();

                std::shared_ptr<DesignPoint> designPoint = importanceSampling->getDesignPoint(modelRunner);

                for (size_t i = 0; i < previousDesignPoints.size(); i++)
                {
                    designPoint->ContributingDesignPoints.push_back(previousDesignPoints[i]);
                }

                return designPoint;
            }
        }

        bool AdaptiveImportanceSampling::isNextLoopAllowed(std::shared_ptr<AdaptiveImportanceSamplingSettings> settings, int counter, std::shared_ptr<ConvergenceReport> convergenceReport, std::shared_ptr<DesignPoint> designPoint)
        {
            if (isStopped())
            {
                return false;
            }
            else if (counter < settings->MinVarianceLoops)
            {
                return true;
            }
            else
            {
                return counter < settings->MaxVarianceLoops && !this->isConverged(settings, convergenceReport) && this->nextLoopsAllowed(settings, designPoint->Beta);
            }
        }

        bool AdaptiveImportanceSampling::isConverged(std::shared_ptr<AdaptiveImportanceSamplingSettings> settings, std::shared_ptr<ConvergenceReport> convergenceReport)
        {
            if (settings->AutoMaximumSamples)
            {
                return convergenceReport->MaxWeight / convergenceReport->FailWeight < settings->EpsWeightSample;
            }
            else
            {
                return std::min(convergenceReport->FailFraction, 1 - convergenceReport->FailFraction) >= settings->FractionFailed;
            }
        }

        bool AdaptiveImportanceSampling::nextLoopsAllowed(std::shared_ptr<AdaptiveImportanceSamplingSettings> settings, double beta)
        {
            return beta <= settings->MaxBeta && beta >= -settings->MaxBeta;
        }

        void AdaptiveImportanceSampling::setStopped()
        {
            this->Stop();

            if (this->importanceSampling != nullptr)
            {
                this->importanceSampling->Stop();
            }
        }

        void AdaptiveImportanceSampling::addFactor(std::shared_ptr<StochastSettingsSet> stochastSet, double addValue)
        {
            for (size_t i = 0; i < stochastSet->getVaryingStochastCount(); i++)
            {
                stochastSet->VaryingStochastSettings[i]->VarianceFactor += addValue;
            }
        }

        void AdaptiveImportanceSampling::setFactor(std::shared_ptr<StochastSettingsSet> stochastSet, double setValue)
        {
            for (size_t i = 0; i < stochastSet->getVaryingStochastCount(); i++)
            {
                stochastSet->VaryingStochastSettings[i]->VarianceFactor = setValue;
            }
        }

        bool AdaptiveImportanceSampling::updateClusters(int loopCounter)
        {
            if (loopCounter < Settings->MaxVarianceLoops && this->clusterSamples.size() < Settings->MinimumFailedSamples && Settings->LoopVarianceIncrement > 0)
            {
                this->addFactor(importanceSampling->Settings->StochastSet, Settings->LoopVarianceIncrement);
                return true;
            }
            else
            {
                // get new cluster centers
                std::vector<std::shared_ptr<Sample>> newClusterCenters = this->getClusterCenters(this->clusterSamples);

                // check whether there is a change
                bool hasChanged = newClusterCenters.size() != this->importanceSampling->Settings->Clusters.size();
                if (!hasChanged)
                {
                    for (size_t i = 0; i < newClusterCenters.size(); i++)
                    {
                        hasChanged |= !newClusterCenters[i]->areValuesEqual(this->importanceSampling->Settings->Clusters[i]);
                    }
                }

                // use new clusters if they have been modified

                if (hasChanged)
                {
                    this->importanceSampling->Settings->Clusters.clear();

                    for (std::shared_ptr<Sample> center : this->getClusterCenters(this->clusterSamples))
                    {
                        this->importanceSampling->Settings->Clusters.push_back(center);
                    }

                    this->setFactor(importanceSampling->Settings->StochastSet, Settings->VarianceFactor);
                }

                return hasChanged;
            }
        }

        bool AdaptiveImportanceSampling::updateStartPoint(std::shared_ptr<DesignPoint> designPoint, std::shared_ptr<Models::ModelRunner> modelRunner, int loopCounter)
        {
            LoopMeasureType loopMeasureType = LoopMeasureType::CopyDesignPoint;

            if (loopCounter < Settings->MaxVarianceLoops &&
                Settings->LoopVarianceIncrement > 0 &&
                designPoint->convergenceReport->FailedSamples < Settings->MinimumFailedSamples)
            {
                loopMeasureType = LoopMeasureType::IncreaseVariance;
            }

            switch (loopMeasureType)
            {
            case LoopMeasureType::IncreaseVariance:
                this->addFactor(importanceSampling->Settings->StochastSet, Settings->LoopVarianceIncrement);
                return true;
            case LoopMeasureType::CopyDesignPoint:
            {
                std::shared_ptr<Sample> newStartPoint = this->getStartPoint(modelRunner, designPoint);

                if (this->lastStartPoint != nullptr && !this->lastStartPoint->areValuesEqual(newStartPoint))
                {
                    this->lastStartPoint = newStartPoint;
                    importanceSampling->Settings->StochastSet->setStartPoint(newStartPoint);
                    this->setFactor(importanceSampling->Settings->StochastSet, Settings->VarianceFactor);

                    return true;
                }
                else
                {
                    return false;
                }
            }
            default:
                throw probLibException("Loop measure type");
            }
        }

        std::shared_ptr<Sample> AdaptiveImportanceSampling::getStartPoint(std::shared_ptr<ModelRunner> modelRunner, std::shared_ptr<DesignPoint> designPoint)
        {
            if (!this->nextLoopsAllowed(Settings, designPoint->Beta) && this->lastStartPoint != nullptr)
            {
                // reuse last start point
                return this->lastStartPoint;
            }
            else
            {
                std::shared_ptr<Sample> newSample = designPoint->convergenceReport->FailedSamples > 0 ? designPoint->getSample() : designPoint->convergenceReport->NearestSample;

                if (Settings->StartPointOnLimitState)
                {
                    const std::shared_ptr<DirectionReliability> direction = std::make_shared<DirectionReliability>();
                    modelRunner->updateStochastSettings(direction->Settings->StochastSet);
                    direction->Settings->StochastSet->setStartPoint(newSample);

                    const std::shared_ptr<DesignPoint> directionDesignPoint = direction->getDesignPoint(modelRunner);

                    newSample = directionDesignPoint->getSample();
                }
                else if (Settings->StartValueStepSize > 0 && this->lastStartPoint != nullptr)
                {
                    // avoid a new start point very close to the old start point
                    for (size_t i = 0; i < newSample->Values.size(); i++)
                    {
                        const double diff = newSample->Values[i] - this->lastStartPoint->Values[i];
                        const double steps = std::round(diff / Settings->StartValueStepSize);
                        newSample->Values[i] = this->lastStartPoint->Values[i] + steps * Settings->StartValueStepSize;
                    }
                }

                return newSample;
            }
        }

        std::vector<std::shared_ptr<Sample>> AdaptiveImportanceSampling::getClusterCenters(std::vector<std::shared_ptr<Sample>>& samples)
        {
            if (samples.empty())
            {
                return std::vector< std::shared_ptr<Sample>>();
            }
            else
            {
                // Get Multiple Design Points using K-Means
                std::shared_ptr<Optimization::KMeansClustering> clusterMethod = std::make_shared<Optimization::KMeansClustering>();
                clusterMethod->Settings = this->Settings->clusterSettings;
                clusterMethod->Settings->generatorType = this->Settings->importanceSamplingSettings->randomSettings->RandomGeneratorType;

                return clusterMethod->getClusterCenters(samples);
            }
        }

        void AdaptiveImportanceSampling::reportVarianceLoop(std::shared_ptr<Models::ModelRunner> modelRunner, const int loopCounter) const
        {
            for (std::shared_ptr<Sample> center : this->importanceSampling->Settings->Clusters)
            {
                modelRunner->reportMessage(Models::MessageType::Info, "Cluster = (" + Numeric::NumericSupport::ConvertToString(center->Values, ", ") + ")");
            }
#ifdef __cpp_lib_format
            auto text = std::format("Calculating variance loop #{0:}.", loopCounter);
#else
            auto text = "Calculating variance loop " + std::to_string(loopCounter) + ".";
#endif
            modelRunner->doTextualProgress(ProgressType::Global, text);
        }

        void AdaptiveImportanceSampling::setCallbacks(std::shared_ptr<ImportanceSampling> importanceSampling, int loopCounter)
        {
            if (this->Settings->Clustering)
            {
                importanceSampling->setSampleLambda([this](std::shared_ptr<Sample> sample)
                {
                    if (this->clusterSamples.size() < this->Settings->clusterSettings->MaxSamples)
                    {
                        this->clusterSamples.push_back(sample);
                    }
                });
            }
            else
            {
                importanceSampling->setBreakLoopLambda([this, loopCounter](std::shared_ptr<ImportanceSamplingCluster> results)
                {
                    if (this->Settings->AutoMaximumSamples && this->Settings->MaxVarianceLoops > 1 && loopCounter < this->Settings->MaxVarianceLoops)
                    {
                        double nAdditionEstimate = results->TotalCount * ((results->MaxFailWeight / results->FailWeight / this->Settings->EpsWeightSample) - 1.0);
                        double nRequiredIdealEstimate = 2 * (Statistics::StandardNormal::getUFromQ(results->ProbFailure) + 1) / this->Settings->EpsWeightSample;

                        if (nAdditionEstimate > nRequiredIdealEstimate)
                        {
                            return true;
                        }
                    }

                    return false;
                });
            }
        }
    }
}

