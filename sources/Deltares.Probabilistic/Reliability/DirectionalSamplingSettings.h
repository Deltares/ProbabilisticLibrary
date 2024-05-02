#pragma once

#include "../Model/RandomSettings.h"
#include "DesignPointBuilder.h"
#include "DirectionReliabilitySettings.h"
#include "StochastSettingsSet.h"

namespace Deltares
{
	namespace Reliability
	{
		/**
		 * \brief Settings for directional sampling
		 */
		class DirectionalSamplingSettings
		{
		public:
			/**
			 * \brief The minimum number of directions to be examined
			 */
			int MinimumDirections = 1000;

			/**
			 * \brief The maximum number of directions to be examined
			 */
			int MaximumDirections = 10000;

			/**
			 * \brief The directional sampling algorithm stops when the calculated variation coefficient is less than this value
			 */
			double VariationCoefficient = 0.05;

			/**
			 * \brief Method type how the design point (alpha values) is calculated
			 */
			DesignPointMethod designPointMethod = DesignPointMethod::CenterOfGravity;

			/**
			 * \brief Settings for calculating one direction
			 */
			std::shared_ptr<DirectionReliabilitySettings> DirectionSettings = std::make_shared<DirectionReliabilitySettings>();

			/**
			 * \brief Settings for performing model runs
			 */
			std::shared_ptr<Models::RunSettings> runSettings = std::make_shared<Models::RunSettings>();

			/**
			 * \brief Settings for generating random values
			 */
			std::shared_ptr<Models::RandomSettings> randomSettings = std::make_shared<Models::RandomSettings>();

			/**
			 * \brief Settings for individual stochastic variables
			 */
			std::shared_ptr<StochastSettingsSet> StochastSet = std::make_shared<StochastSettingsSet>();

			/**
			 * \brief Indicates whether these settings have valid values
			 * \return Indication
			 */
			bool isValid()
			{
				return MinimumDirections >= 1 &&
					MaximumDirections >= MinimumDirections &&
					VariationCoefficient >= 0 &&
					runSettings->isValid();
			}
		};
	}
}


