#pragma once

#include "../Model/RunSettings.h"
#include "../Optimization/ClusterSettings.h"
#include "StartPointCalculatorSettings.h"
#include "StochastSettingsSet.h"

namespace Deltares
{
	namespace Reliability
	{
		class AdaptiveImportanceSamplingSettings
		{
		public:
			int MaxVarianceLoops = 5;
			bool AutoMaximumSamplesNoResult = false;
			int MaximumSamplesNoResult = 10000;
			int MaximumSamples = 10000;
			int MinimumFailedSamples = 0;
			double LoopVarianceIncrement = 0.5;
			bool StartPointOnLimitState = false;
			double RestartStepSize = 0;
			double VarianceFactor= 1.5;
			int MinVarianceLoops = 1;
			double EpsWeightSample = 0.1;
			double FractionFailed = 0.1;
			bool Clustering = false;
			double MaxBeta = Statistics::StandardNormal::BetaMax;

			std::shared_ptr<Models::RunSettings> RunSettings = std::make_shared<Models::RunSettings>();
			std::shared_ptr<Optimization::ClusterSettings> ClusterSettings = std::make_shared<Optimization::ClusterSettings>();
			std::shared_ptr<StartPointCalculatorSettings> StartPointSettings = std::make_shared<StartPointCalculatorSettings>();
			std::shared_ptr<StochastSettingsSet> StochastSet = std::make_shared<StochastSettingsSet>();

			bool isValid()
			{
				return MinVarianceLoops >= 1 &&
					MaxVarianceLoops >= MinVarianceLoops &&
					RestartStepSize >= 0 &&
					FractionFailed >= 0 && FractionFailed <= 0.5 &&
					StartPointSettings->isValid() &&
					RunSettings->isValid();
			}
		};
	}
}

