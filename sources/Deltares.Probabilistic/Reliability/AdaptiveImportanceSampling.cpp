#include "AdaptiveImportanceSampling.h"

#include <vector>

#if __has_include(<format>)
#include <format>
#else
#include "../Utils/probLibString.h"
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

using namespace Deltares::Models;

namespace Deltares
{
	namespace Reliability
	{
		std::shared_ptr<DesignPoint> AdaptiveImportanceSampling::getDesignPoint(std::shared_ptr<Models::ModelRunner> modelRunner)
		{
			modelRunner->updateStochastSettings(this->Settings->StochastSet);

			this->importanceSampling = std::make_shared<ImportanceSampling>();
			importanceSampling->Settings = this->InternalSettings->clone();

			std::vector<std::shared_ptr<DesignPoint>> previousDesignPoints = std::vector<std::shared_ptr<DesignPoint>>();

			// initialize
			const std::shared_ptr<StartPointCalculator> startPointCalculator = std::make_shared<StartPointCalculator>();
			startPointCalculator->Settings = this->Settings->StartPointSettings;
			startPointCalculator->Settings->StochastSet = this->Settings->StochastSet;

			const std::shared_ptr<Sample> startPoint = startPointCalculator->getStartPoint(modelRunner);
			this->lastStartPoint = startPoint;

			if (Settings->StartPointSettings->StartMethod != StartMethodType::None)
			{
				const std::shared_ptr<DesignPoint> startDesignPoint = modelRunner->getDesignPoint(startPoint, startPoint->getBeta());
				startDesignPoint->Identifier = "Start point";
				previousDesignPoints.push_back(startDesignPoint);

				importanceSampling->Settings->StochastSet->setStartPoint(startPoint);
			}

			importanceSampling->Settings->AutoMaximumSamplesNoResult = Settings->AutoMaximumSamplesNoResult;
			importanceSampling->Settings->EpsWeightSample = Settings->EpsWeightSample;

			modelRunner->doTextualProgress(ProgressType::Global, "Calculating design point.");

			if (Settings->MaxVarianceLoops > 1)
			{
				modelRunner->clear();

				if (!Settings->AutoMaximumSamplesNoResult)
				{
					importanceSampling->Settings->MaximumSamples = Settings->MaximumSamplesNoResult;
				}

				int loopCounter = 1;

				importanceSampling->Settings->Counter = loopCounter;

				this->setCallbacks(this->importanceSampling);

				std::shared_ptr<DesignPoint> designPoint = importanceSampling->getDesignPoint(modelRunner);

				designPoint->convergenceReport->VarianceFactor = Settings->VarianceFactor;

				bool fullExecuted = importanceSampling->Settings->MaximumSamples == Settings->MaximumSamples;

				while (!isStopped() && isNextLoopAllowed(Settings, loopCounter, designPoint->convergenceReport, designPoint))
				{
					loopCounter++;

					const std::shared_ptr<ImportanceSamplingSettings> importanceSamplingSettings = importanceSampling->Settings;

					this->importanceSampling = std::make_shared<ImportanceSampling>();
					this->importanceSampling->Settings = importanceSamplingSettings;

					this->setCallbacks(this->importanceSampling);

					if (this->Settings->Clustering)
					{
						this->importanceSampling->Settings->Clusters.clear();

						for (std::shared_ptr<Sample> center : this->getClusterCenters(this->clusterSamples))
						{
							this->importanceSampling->Settings->Clusters.push_back(center);
						}

						this->clusterSamples.clear();
					}
					else
					{
						this->updateStartPoint(designPoint, modelRunner, loopCounter);
					}

					if (loopCounter == Settings->MaxVarianceLoops)
					{
						importanceSampling->Settings->MaximumSamples = Settings->MaximumSamples;
					}

					previousDesignPoints.push_back(designPoint);

					modelRunner->clear();
					clusterSamples.clear();

					importanceSampling->Settings->Counter = loopCounter;

#ifdef __cpp_lib_format
					auto text = std::format("Calculating variance loop #{0:}.", loopCounter);
#else
					auto pl = Deltares::Reliability::probLibString();
					auto text = "Variance loop = " + pl.int2str(loopCounter);
#endif
					modelRunner->doTextualProgress(ProgressType::Global, text);
					designPoint = importanceSampling->getDesignPoint(modelRunner);

					designPoint->convergenceReport->VarianceFactor = Settings->VarianceFactor;

					fullExecuted = importanceSampling->Settings->MaximumSamples == Settings->MaximumSamples;
				}

				if (!fullExecuted)
				{
					const std::shared_ptr<ImportanceSamplingSettings> importanceSamplingSettings = importanceSampling->Settings;

					this->importanceSampling = std::make_shared<ImportanceSampling>();
					this->importanceSampling->Settings = importanceSamplingSettings;

					this->setCallbacks(this->importanceSampling);

					this->setFactor(importanceSampling->Settings->StochastSet, Settings->VarianceFactor);

					importanceSampling->Settings->MaximumSamples = Settings->MaximumSamples;
					importanceSampling->Settings->Counter = loopCounter;

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

				importanceSampling->Settings->Counter = 0;

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
			if (settings->AutoMaximumSamplesNoResult)
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

		void AdaptiveImportanceSampling::updateStartPoint(std::shared_ptr<DesignPoint> designPoint, std::shared_ptr<Models::ModelRunner> modelRunner, int loopCounter)
		{
			LoopMeasureType loopMeasureType = LoopMeasureType::CopyDesignPoint;
			if (loopCounter < Settings->MaxVarianceLoops && designPoint->convergenceReport->FailedSamples < Settings->MinimumFailedSamples)
			{
				loopMeasureType = LoopMeasureType::IncreaseVariance;
			}

			switch (loopMeasureType)
			{
			case LoopMeasureType::IncreaseVariance:
				this->addFactor(importanceSampling->Settings->StochastSet, Settings->LoopVarianceIncrement);
				break;
			case LoopMeasureType::CopyDesignPoint:
			{
				std::shared_ptr<Sample> newStartPoint = this->getStartPoint(modelRunner, designPoint);
				importanceSampling->Settings->StochastSet->setStartPoint(newStartPoint);
				this->setFactor(importanceSampling->Settings->StochastSet, Settings->VarianceFactor);
				break;
			}
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
					direction->Settings->StochastSet->setStartPoint(newSample);

					const std::shared_ptr<DesignPoint> directionDesignPoint = direction->getDesignPoint(modelRunner);

					newSample = directionDesignPoint->getSample();
				}

				this->lastStartPoint = newSample;

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
				clusterMethod->Settings = this->Settings->ClusterSettings;

				return clusterMethod->getClusterCenters(samples);
			}
		}

		void AdaptiveImportanceSampling::setCallbacks(std::shared_ptr<ImportanceSampling> importanceSampling)
		{
			if (this->Settings->Clustering)
			{
				importanceSampling->setSampleLambda([this](std::shared_ptr<Sample> sample) {this->clusterSamples.push_back(sample); });
			}
		}
	}
}

