#pragma once
#include "ImportanceSamplingCluster.h"
#include "ImportanceSamplingSettings.h"
#include "ReliabilityMethod.h"

namespace Deltares
{
	namespace Reliability
	{
		typedef std::function<void(std::shared_ptr<Sample>)> RegisterSampleLambda;
		typedef std::function<bool(std::shared_ptr<ImportanceSamplingCluster>)> BreakLoopLambda;

		class ImportanceSampling : public ReliabilityMethod
		{
		public:
			std::shared_ptr<ImportanceSamplingSettings> Settings = std::make_shared<ImportanceSamplingSettings>();
			std::shared_ptr<DesignPoint> getDesignPoint(std::shared_ptr<Models::ModelRunner> modelRunner) override;
			void setSampleLambda(RegisterSampleLambda sampleFunction);
			void setBreakLoopLambda(BreakLoopLambda breakFunction);
		private:
			RegisterSampleLambda sampleFunction = nullptr;
			BreakLoopLambda breakFunction = nullptr;
			std::vector<std::shared_ptr<ImportanceSamplingCluster>> getClusters();
			bool checkConvergence(std::shared_ptr<Models::ModelRunner> modelRunner, double pf, double minWeight, int samples, int nmaal);
			double getConvergence(double pf, double minWeight, int samples);
			double getDimensionality(std::vector<double> factors);
			double getStandardNormalPDF2(double u2);
			double getSampleWeight(std::shared_ptr<Sample> sample, std::shared_ptr<Sample> center, double dimensionality, std::vector<double> factors);
			double getWeight(std::shared_ptr<Sample> modifiedSample, std::shared_ptr<Sample> sample, double dimensionality);
			double getPDF(std::shared_ptr<Sample> sample);
			std::vector<double> getFactors(std::shared_ptr<StochastSettingsSet> stochastSettings);
			std::shared_ptr<Sample> getOriginalSample(std::shared_ptr<Sample> sample, std::shared_ptr<Sample> center, std::vector<double> factors);
			double getProbabilityOfFailure(const std::vector<std::shared_ptr<ImportanceSamplingCluster>>& clusters);

			bool prematureExit(std::shared_ptr<ImportanceSamplingSettings> settings, int samples, int runs);
			double getCorrectionForOverlappingClusters(std::shared_ptr<Sample> sample, std::shared_ptr<ImportanceSamplingCluster> clusterResult, std::vector<std::shared_ptr<ImportanceSamplingCluster>> clusterResults);
			std::shared_ptr<ImportanceSamplingCluster> getNearestCluster(std::shared_ptr<Sample> sample, std::vector<std::shared_ptr<ImportanceSamplingCluster>> clusters);
			std::shared_ptr<ImportanceSamplingCluster> findMostContributingCluster(std::vector<std::shared_ptr<ImportanceSamplingCluster>> clusters);

			bool breakLoopWithFailureObs(std::shared_ptr<ImportanceSamplingSettings> settings, bool enoughSamples, bool smallEnough, std::shared_ptr<ImportanceSamplingCluster> results);
			bool breakLoopWithNoFailureObs(std::shared_ptr<ModelRunner> modelRunner, std::shared_ptr<ImportanceSamplingSettings> settings, int sampleIndex, bool& reported);

			void report(std::shared_ptr<ModelRunner> modelRunner, int sampleIndex);
		};
	}
}

