#pragma once

#include "../Model/RandomSettings.h"
#include "../Model/RunSettings.h"
#include "DesignPointBuilder.h"
#include "StartPointCalculatorSettings.h"
#include "StochastSettingsSet.h"

namespace Deltares
{
	namespace Reliability
	{
		/**
		 * \brief Settings for importance sampling
		 */
		class ImportanceSamplingSettings
		{
		public:
			/**
			 * \brief The minimum number of samples to be examined
			 */
			int MinimumSamples = 1000;

			/**
			 * \brief The maximum number of samples to be examined
			 */
			int MaximumSamples = 10000;

			/**
			 * \brief Maximum number of samples when all samples have the same qualitative result (Z < 0 or Z > 0)
			 */
			int MaximumSamplesNoResult = 10000;

			/**
			 * \brief The importance sampling algorithm stops when the calculated variation coefficient is less than this value
			 */
			double VariationCoefficient = 0.05;

			/**
			 * \brief Method type how the design point (alpha values) is calculated
			 */
			DesignPointMethod designPointMethod = DesignPointMethod::CenterOfGravity;

			/**
			 * \brief Settings for generating random values
			 */
			std::shared_ptr<Deltares::Models::RandomSettings> randomSettings = std::make_shared<Deltares::Models::RandomSettings>();

			/**
			 * \brief Settings how to derive the first center
			 */
			std::shared_ptr<StartPointCalculatorSettings> startPointSettings = std::make_shared<StartPointCalculatorSettings>();

			/**
			 * \brief Settings for performing model runs
			 */
			std::shared_ptr<Models::RunSettings> runSettings = std::make_shared<Models::RunSettings>();

			/**
			 * \brief Settings per stochastic variable, contains (among others) the center value and multiplication factor used to shift samples in the importance sampling algorithm
			 */
			std::shared_ptr<StochastSettingsSet> StochastSet = std::make_shared<StochastSettingsSet>();

			/**
			 * \brief List of center points, if empty the center point is derived from the StochastSet
			 */
			std::vector<std::shared_ptr<Sample>> Clusters;

			/**
			 * \brief Indicates whether these settings have valid values
			 * \return Indication
			 */
			bool isValid() const
			{
				return MinimumSamples >= 1 &&
					MaximumSamples >= MinimumSamples &&
					this->isStochastSetValid() &&
					runSettings->isValid();
			}

			/**
			 * \brief Makes a copy of these settings (deep copy of stochast settings)
			 * \return Copy
			 */
			std::shared_ptr<ImportanceSamplingSettings> clone() const
			{
				std::shared_ptr<ImportanceSamplingSettings> clone = std::make_shared<ImportanceSamplingSettings>();

				clone->MaximumSamples = this->MaximumSamples;
				clone->MaximumSamplesNoResult = this->MaximumSamplesNoResult;
				clone->MinimumSamples = this->MinimumSamples;
				clone->VariationCoefficient = this->VariationCoefficient;
				clone->designPointMethod = this->designPointMethod;

				clone->runSettings = this->runSettings;
				clone->randomSettings = this->randomSettings->clone();
				clone->startPointSettings = this->startPointSettings->clone();

				clone->StochastSet->AreStartValuesCorrelated = this->StochastSet->AreStartValuesCorrelated;

				for (size_t i = 0; i < this->StochastSet->getStochastCount(); i++)
				{
					clone->StochastSet->stochastSettings.push_back(this->StochastSet->stochastSettings[i]->clone());
				}

				clone->startPointSettings->StochastSet = clone->StochastSet;
				clone->randomSettings->StochastSet = clone->StochastSet;

				return clone;
			}
		private:
			/**
			 * \brief Indicates whether all stochast settings have valid values
			 * \return True if all valid, false otherwise
			 */
			bool isStochastSetValid() const
			{
				for (size_t i = 0; i < this->StochastSet->getVaryingStochastCount(); i++)
				{
					const std::shared_ptr<StochastSettings> stochastSettings = this->StochastSet->VaryingStochastSettings[i];
					const bool valid =
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
		};
	}
}

