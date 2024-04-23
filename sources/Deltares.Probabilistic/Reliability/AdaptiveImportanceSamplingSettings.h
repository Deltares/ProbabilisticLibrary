#pragma once

#include "ImportanceSamplingSettings.h"
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
			int MinVarianceLoops = 1;
			int MaxVarianceLoops = 5;

			bool AutoMaximumSamplesNoResult = false;
			int MinimumFailedSamples = 0;

			double VarianceFactor = 1.5;
			double LoopVarianceIncrement = 0.5;
			bool StartPointOnLimitState = false;
			double RestartStepSize = 0;
			double EpsWeightSample = 0.1;
			double FractionFailed = 0.1;

			double MaxClusters = 1;
			bool Clustering = false;
			double MaxBeta = Statistics::StandardNormal::BetaMax;

			std::shared_ptr<Optimization::ClusterSettings> ClusterSettings = std::make_shared<Optimization::ClusterSettings>();
			std::shared_ptr<StartPointCalculatorSettings> StartPointSettings = std::make_shared<StartPointCalculatorSettings>();
			std::shared_ptr<ImportanceSamplingSettings> ImportanceSamplingSettings = std::make_shared<Deltares::Reliability::ImportanceSamplingSettings>();

			bool isValid()
			{
				return MinVarianceLoops >= 1 &&
					MaxVarianceLoops >= MinVarianceLoops &&
					RestartStepSize >= 0 &&
					FractionFailed >= 0 && FractionFailed <= 0.5 &&
					EpsWeightSample >= 0.0001 && EpsWeightSample <= 1 &&
					MaxClusters >= 1 &&
					StartPointSettings->isValid() &&
					ClusterSettings->isValid() &&
					ImportanceSamplingSettings->isValid();
			}
		};
	}
}

