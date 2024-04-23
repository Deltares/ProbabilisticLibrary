#pragma once

#include "../Model/RandomSettings.h"
#include "../Model/RunSettings.h"
#include "DesignPointBuilder.h"
#include "StochastSettingsSet.h"

namespace Deltares
{
	namespace Reliability
	{
		class ImportanceSamplingSettings
		{
		public:
			int MinimumSamples = 1000;
			int MaximumSamples = 10000;
			int MaximumSamplesNoResult = 10000;
			double VariationCoefficient = 0.05;
			double VarianceFactor = 1;

			// to be moved to adaptive importance sampling settings
			//int MaxVarianceLoops = 5;
			//bool AutoMaximumSamplesNoResult = false;
			//int MaxClusters = 1;
			//double EpsWeightSample = 0.1;
			//bool OptimizeNumberClusters = false;

			DesignPointMethod designPointMethod = DesignPointMethod::CenterOfGravity;
			std::shared_ptr<Deltares::Models::RandomSettings> randomSettings = std::make_shared<Deltares::Models::RandomSettings>();
			std::shared_ptr<Models::RunSettings> RunSettings = std::make_shared<Models::RunSettings>();
			std::shared_ptr<StochastSettingsSet> StochastSet = std::make_shared<StochastSettingsSet>();

			bool Clustering = false;
			std::vector<std::shared_ptr<Sample>> Clusters;

			bool isValid()
			{
				return MinimumSamples >= 1 &&
					MaximumSamples >= MinimumSamples &&
					VarianceFactor >= 0.01 &&
					this->isStochastSetValid() &&

					RunSettings->isValid();
			}

			bool isStochastSetValid()
			{
				for (size_t i = 0; i < this->StochastSet->getVaryingStochastCount(); i++)
				{
					std::shared_ptr<StochastSettings> stochastSettings = this->StochastSet->VaryingStochastSettings[i];
					bool valid =
						stochastSettings->VarianceFactor >= 0.1 &&
						stochastSettings->StartValue >= -Statistics::StandardNormal::UMax &&
						stochastSettings->StartValue <= Statistics::StandardNormal::UMax;

						if (!valid)
						{
							return false;
						}
				}

				return true;
			}

			std::shared_ptr<ImportanceSamplingSettings> clone()
			{
				std::shared_ptr<ImportanceSamplingSettings> clone = std::make_shared<ImportanceSamplingSettings>();

				clone->MaximumSamples = this->MaximumSamples;
				clone->MaximumSamplesNoResult = this->MaximumSamplesNoResult;
				clone->MinimumSamples = this->MinimumSamples;
				clone->VarianceFactor = this->VarianceFactor;
				clone->VariationCoefficient = this->VariationCoefficient;
				clone->designPointMethod = this->designPointMethod;

				// move to adaptive importance sampling settings
				clone->Clustering = this->Clustering;

				clone->RunSettings = this->RunSettings;
				clone->randomSettings = this->randomSettings;

				clone->StochastSet->AreStartValuesCorrelated = this->StochastSet->AreStartValuesCorrelated;

				for (size_t i = 0; i < this->StochastSet->getStochastCount(); i++)
				{
					clone->StochastSet->stochastSettings.push_back(this->StochastSet->stochastSettings[i]->clone());
				}

				return clone;
			}
		};
	}
}

