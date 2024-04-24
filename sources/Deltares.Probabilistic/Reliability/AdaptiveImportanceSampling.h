#pragma once
#include "AdaptiveImportanceSamplingSettings.h"
#include "ImportanceSampling.h"
#include "ReliabilityMethod.h"

namespace Deltares
{
	namespace Reliability
	{
		/**
		 * \brief Different types of modifying the start point of importance sampling
		 */
		enum LoopMeasureType { IncreaseVariance, CopyDesignPoint };

		/**
		 * \brief Performs adaptive importance sampling
		 */
		class AdaptiveImportanceSampling : public ReliabilityMethod
		{
		public:
			/**
			 * \brief Settings for adaptive importance sampling
			 */
			std::shared_ptr<AdaptiveImportanceSamplingSettings> Settings = std::make_shared<AdaptiveImportanceSamplingSettings>();

			/**
			 * \brief Executes the adaptive importance sampling
			 * \param modelRunner Performs execution of samples
			 * \return Design point
			 */
			std::shared_ptr<DesignPoint> getDesignPoint(std::shared_ptr<Models::ModelRunner> modelRunner) override;
		protected:
			void setStopped() override;
		private:
			bool isNextLoopAllowed(std::shared_ptr<AdaptiveImportanceSamplingSettings> settings, int counter, std::shared_ptr<ConvergenceReport> convergenceReport, std::shared_ptr<DesignPoint> designPoint);
			bool isConverged(std::shared_ptr<AdaptiveImportanceSamplingSettings> settings, std::shared_ptr<ConvergenceReport> convergenceReport);
			bool nextLoopsAllowed(std::shared_ptr<AdaptiveImportanceSamplingSettings> settings, double beta);

			void addFactor(std::shared_ptr<StochastSettingsSet> stochastSet, double addValue);
			void setFactor(std::shared_ptr<StochastSettingsSet> stochastSet, double setValue);
			bool updateClusters(int loopCounter);
			bool updateStartPoint(std::shared_ptr<DesignPoint> designPoint, std::shared_ptr<Models::ModelRunner> modelRunner, int loopCounter);
			std::shared_ptr<Sample> getStartPoint(std::shared_ptr<ModelRunner> modelRunner, std::shared_ptr<DesignPoint> designPoint);
			std::vector<std::shared_ptr<Sample>> getClusterCenters(std::vector<std::shared_ptr<Sample>>& samples);
			void setCallbacks(std::shared_ptr<ImportanceSampling> importanceSampling, int loopCounter);

			/**
			 * \brief Previous start point 
			 */
			std::shared_ptr<Sample> lastStartPoint = nullptr;

			/**
			 * \brief Internal importance sampling routine
			 */
			std::shared_ptr<ImportanceSampling> importanceSampling = nullptr;

			/**
			 * \brief Executed samples used for cluster determination
			 */
			std::vector<std::shared_ptr<Sample>> clusterSamples;
		};
	}
}

