#pragma once
#include "ImportanceSamplingCluster.h"
#include "ImportanceSamplingSettings.h"
#include "ReliabilityMethod.h"

namespace Deltares
{
	namespace Reliability
	{
		class ImportanceSampling : public ReliabilityMethod
		{
		public:
			std::shared_ptr<ImportanceSamplingSettings> Settings = std::make_shared<ImportanceSamplingSettings>();
			std::shared_ptr<DesignPoint> getDesignPoint(std::shared_ptr<Models::ModelRunner> modelRunner) override;
		private:
			std::vector<std::shared_ptr<ImportanceSamplingCluster>> getClusters();

			bool checkConvergence(std::shared_ptr<Models::ModelRunner> modelRunner, double pf, double minWeight, int samples, int nmaal);
			double getConvergence(double pf, double minWeight, int samples);
			double getDimensionality(std::shared_ptr<StochastSettingsSet> stochastSettings);
			double getStandardNormalPDF2(double u2);
			double getWeight(std::shared_ptr<Sample> modifiedSample, std::shared_ptr<Sample> sample, double dimensionality);
			double getPDF(std::shared_ptr<Sample> sample);

			bool prematureExit(std::shared_ptr<ImportanceSamplingSettings> settings, int samples, int runs);
			double getCorrectionForOverlappingClusters(std::shared_ptr<Sample> sample, std::shared_ptr<ImportanceSamplingCluster> clusterResult, std::vector<std::shared_ptr<ImportanceSamplingCluster>> clusterResults);
			std::shared_ptr<ImportanceSamplingCluster> getNearestCluster(std::shared_ptr<Sample> sample, std::vector<std::shared_ptr<ImportanceSamplingCluster>> clusters);

			bool breakLoopWithFailureObs(std::shared_ptr<ImportanceSamplingSettings> settings, bool enoughSamples, bool smallEnough, std::shared_ptr<ImportanceSamplingCluster> results);
			bool breakLoopWithNoFailureObs(std::shared_ptr<ModelRunner> modelRunner, std::shared_ptr<ImportanceSamplingSettings> settings, int sampleIndex, bool& reported);

			std::shared_ptr<ConvergenceReport> getConvergenceReport(std::shared_ptr<ImportanceSamplingSettings> settings);
			void report(std::shared_ptr<ModelRunner> modelRunner, int sampleIndex);
		};
	}
}

