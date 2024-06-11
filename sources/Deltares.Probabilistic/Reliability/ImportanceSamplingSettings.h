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

		    double VarianceFactor = 1;

            /**
             * \brief Method type how the design point (alpha values) is calculated
             */
            DesignPointMethod designPointMethod = DesignPointMethod::CenterOfGravity;

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

            std::shared_ptr<Deltares::Models::RandomSettings> randomSettings = std::make_shared<Deltares::Models::RandomSettings>();

			bool Clustering = false;
			std::vector<std::shared_ptr<Sample>> Clusters;

			bool isValid()
			{
				return MinimumSamples >= 1 &&
					MaximumSamples >= MinimumSamples &&
					VarianceFactor >= 0.01 &&
					this->isStochastSetValid() &&

					runSettings->isValid();
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

            /**
             * \brief Makes a copy of these settings (deep copy of stochast settings)
             * \return Copy
             */
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

                clone->StochastSet->AreStartValuesCorrelated = this->StochastSet->AreStartValuesCorrelated;

			    clone->runSettings = this->runSettings;
				clone->randomSettings = this->randomSettings;
                clone->startPointSettings = this->startPointSettings->clone();

				for (size_t i = 0; i < this->StochastSet->getStochastCount(); i++)
				{
					clone->StochastSet->stochastSettings.push_back(this->StochastSet->stochastSettings[i]->clone());
				}

				return clone;
			}
		};
	}
}

