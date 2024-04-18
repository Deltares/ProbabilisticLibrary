#pragma once
#include "AdaptiveImportanceSamplingSettings.h"
#include "ImportanceSampling.h"
#include "ImportanceSamplingSettings.h"
#include "ReliabilityMethod.h"

namespace Deltares
{
	namespace Reliability
	{
		enum LoopMeasureType { IncreaseVariance, CopyDesignPoint };

		class AdaptiveImportanceSampling : public ReliabilityMethod
		{
		public:
			std::shared_ptr<AdaptiveImportanceSamplingSettings> Settings = std::make_shared<AdaptiveImportanceSamplingSettings>();
			std::shared_ptr<ImportanceSamplingSettings> InternalSettings = std::make_shared<ImportanceSamplingSettings>();
			std::shared_ptr<DesignPoint> getDesignPoint(std::shared_ptr<Models::ModelRunner> modelRunner) override;
		protected:
			void setStopped() override;
		private:
			bool isNextLoopAllowed(std::shared_ptr<AdaptiveImportanceSamplingSettings> settings, int counter, std::shared_ptr<ConvergenceReport> convergenceReport, std::shared_ptr<DesignPoint> designPoint);
			bool isConverged(std::shared_ptr<AdaptiveImportanceSamplingSettings> settings, std::shared_ptr<ConvergenceReport> convergenceReport);
			bool nextLoopsAllowed(std::shared_ptr<AdaptiveImportanceSamplingSettings> settings, double beta);

			void addFactor(std::shared_ptr<StochastSettingsSet> stochastSet, double addValue);
			void setFactor(std::shared_ptr<StochastSettingsSet> stochastSet, double setValue);
			void updateStartPoint(std::shared_ptr<DesignPoint> designPoint, std::shared_ptr<Models::ModelRunner> modelRunner, int loopCounter);
			std::shared_ptr<Sample> getStartPoint(std::shared_ptr<ModelRunner> modelRunner, std::shared_ptr<DesignPoint> designPoint);
			std::vector<std::shared_ptr<Sample>> getClusterCenters(std::vector<std::shared_ptr<Sample>>& samples);
			void setCallbacks(std::shared_ptr<ImportanceSampling> importanceSampling);

			std::shared_ptr<Sample> lastStartPoint = nullptr;
			std::shared_ptr<ImportanceSampling> importanceSampling = nullptr;
			std::vector<std::shared_ptr<Sample>> clusterSamples;

		};
	}
}

