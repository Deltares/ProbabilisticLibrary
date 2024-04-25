#pragma once

#include "../Model/RandomSettings.h"
#include "DesignPointBuilder.h"
#include "DirectionReliabilitySettings.h"
#include "StochastSettingsSet.h"

namespace Deltares
{
	namespace Reliability
	{
		class DirectionalSamplingSettings
		{
		public:
			int MinimumSamples = 1000;
			int MaximumSamples = 10000;
			double VariationCoefficient = 0.05;
			DesignPointMethod designPointMethod = DesignPointMethod::CenterOfGravity;
			std::shared_ptr<DirectionReliabilitySettings> DirectionSettings = std::make_shared<DirectionReliabilitySettings>();
			std::shared_ptr<Models::RunSettings> runSettings = std::make_shared<Models::RunSettings>();
			std::shared_ptr<Models::RandomSettings> randomSettings = std::make_shared<Models::RandomSettings>();
			std::shared_ptr<StochastSettingsSet> StochastSet = std::make_shared<StochastSettingsSet>();

			bool isValid()
			{
				return MinimumSamples >= 1 &&
					MaximumSamples >= MinimumSamples &&
					VariationCoefficient >= 0 &&
					runSettings->isValid();
			}
		};
	}
}


