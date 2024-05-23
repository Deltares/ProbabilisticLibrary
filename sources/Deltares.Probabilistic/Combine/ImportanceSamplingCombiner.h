#pragma once
#include <vector>
#include <memory>

#include "CombinedDesignPointModel.h"
#include "combiner.h"
#include "../Statistics/CorrelationMatrix.h"
#include "../Reliability/DesignPoint.h"
#include "../Reliability/ImportanceSamplingSettings.h"
#include "../Model/ProgressIndicator.h"
#include "../Model/Project.h"

namespace Deltares
{
    namespace Reliability
    {
        /**
         * \brief Combines design points with importance sampling algorithm
         */
        class ImportanceSamplingCombiner : public Combiner
        {
        public:
            std::shared_ptr<DesignPoint> CombineDesignPoints(const std::vector<std::shared_ptr<DesignPoint>>& designPoints, const std::vector<double>& rho, const combineAndOr cmbType) override;

            /**
             * \brief Combines a number of design points
             * \param combineMethodType Series (or-combination) or Parallel (and-combination) combination type
             * \param designPoints Design points to be combined
             * \param correlationMatrix Correlation matrix (to be removed?)
             * \param selfCorrelationMatrix Administration of correlations between different design points
             * \param progress Progress indicator (optional)
             * \return Design point resembling the combined reliability and alpha values
             */
            std::shared_ptr<DesignPoint> combineDesignPoints(combineAndOr combineMethodType, std::vector<std::shared_ptr<DesignPoint>>& designPoints, std::shared_ptr<Statistics::CorrelationMatrix> correlationMatrix = nullptr, std::shared_ptr<Statistics::SelfCorrelationMatrix> selfCorrelationMatrix = nullptr, std::shared_ptr<ProgressIndicator> progress = nullptr) override;
        private:
            /**
             * \brief Approximates the probability by assuming design points are independent
             * \param combineMethodType Series or parallel combination type
             * \param designPoints Design points to be combined
             * \return Approximated probability of failure
             */
            double getApproximatedProbability(combineAndOr combineMethodType, std::vector<std::shared_ptr<DesignPoint>>& designPoints);

            /**
             * \brief Inverts the design point, so the probability of failure will be the probability of non-failure of the non-inverted design point
             * \param designPoint Design point to be inverted
             */
            void invert(std::shared_ptr<DesignPoint> designPoint);

            /**
             * \brief Gets all stochasts in a number of design points in the design point without doubling
             * \param designPoints design points
             * \return All stochasts without doubling
             */
            std::vector<std::shared_ptr<Statistics::Stochast>> getUniqueStochasts(std::vector<std::shared_ptr<DesignPoint>>& designPoints);

            /**
             * \brief Combines design points with possibly inverted design points
             * \param combineMethodType Series (or-combination) or Parallel (and-combination) combination type
             * \param designPoints Design points to be combined
             * \param correlationMatrix Correlation matrix (to be removed?)
             * \param selfCorrelationMatrix Administration of correlations between different design points
             * \param progress Progress indicator (optional)
             * \return Design point resembling the combined reliability and alpha values
             */
            std::shared_ptr<DesignPoint> combineDesignPointsAdjusted(combineAndOr combineMethodType, std::shared_ptr<Statistics::CorrelationMatrix> correlationMatrix, std::shared_ptr<Statistics::SelfCorrelationMatrix> selfCorrelationMatrix, std::shared_ptr<ProgressIndicator> progress, std::vector<std::shared_ptr<DesignPoint>>& designPoints);

            /**
             * \brief Calculates the probability that a sample does not fail for previous design points when it fails to the current design point
             * \param correlationMatrix Correlation matrix (to be removed?)
             * \param selfCorrelationMatrix Administration of correlations between different design points
             * \param currentDesignPoint Current design point
             * \param previousDesignPoints Previous design point
             * \param startPoint Starting point of the calculation
             * \param stochasts All stochasts 
             * \param progress Progress indicator (optional)
             * \return Probability
             */
            std::shared_ptr<DesignPoint> getSeriesProbability(std::shared_ptr<Statistics::CorrelationMatrix> correlationMatrix, std::shared_ptr<Statistics::SelfCorrelationMatrix> selfCorrelationMatrix, std::shared_ptr<DesignPoint> currentDesignPoint, std::vector<std::shared_ptr<DesignPoint>>& previousDesignPoints, std::shared_ptr<DesignPoint> startPoint, std::vector<std::shared_ptr<Stochast>>& stochasts, std::shared_ptr<ProgressIndicator> progress);

            /**
             * \brief Calculates the design point when combination type is parallel
             * \param correlationMatrix Correlation matrix (to be removed?)
             * \param selfCorrelationMatrix Administration of correlations between different design points
             * \param previousDesignPoints Previous design point
             * \param stochasts All stochasts
             * \param progress Progress indicator (optional)
             * \return Design point
             */
            std::shared_ptr<DesignPoint> getParallelProbability(std::shared_ptr<Statistics::CorrelationMatrix> correlationMatrix, std::shared_ptr<Statistics::SelfCorrelationMatrix> selfCorrelationMatrix, std::vector<std::shared_ptr<DesignPoint>>& previousDesignPoints, std::vector<std::shared_ptr<Stochast>>& stochasts, std::shared_ptr<ProgressIndicator> progress);

            /**
             * \brief Fills the settings for series algorithm
             * \param startPoint Starting point
             * \param model Linearized model
             * \param settings Settings to be filled
             */
            void fillSettingsSeries(std::shared_ptr<DesignPoint> startPoint, std::shared_ptr<CombinedDesignPointModel> model, std::shared_ptr<ImportanceSamplingSettings> settings);

            /**
             * \brief Fills the settings for parallel algorithm
             * \param model Linearized model
             * \param settings Settings to be filled
             */
            void fillSettingsParallel(std::shared_ptr<CombinedDesignPointModel> model, std::shared_ptr<ImportanceSamplingSettings> settings, double factor);

            /**
             * \brief Gets the linearized model representing the design points to be combined
             * \param combineMethodType Combination type
             * \param currentDesignPoint Current design point
             * \param previousDesignPoints Previous design points 
             * \param stochasts All stochasts
             * \param selfCorrelationMatrix Administration of correlations between different design points
             * \return Linearized model
             */
            std::shared_ptr<CombinedDesignPointModel> getModel(combineAndOr combineMethodType, std::shared_ptr<DesignPoint> currentDesignPoint, std::vector<std::shared_ptr<DesignPoint>>& previousDesignPoints, std::vector<std::shared_ptr<Stochast>>& stochasts, std::shared_ptr<Statistics::SelfCorrelationMatrix> selfCorrelationMatrix);

            /**
             * \brief Gets a project containing the linearized model
             * \param model Linearized model
             * \param correlationMatrix Correlation matrix
             * \param selfCorrelationMatrix Self correlation administration
             * \return Project
             */
            std::shared_ptr<Project> getProject(std::shared_ptr<CombinedDesignPointModel> model, std::shared_ptr<Statistics::CorrelationMatrix> correlationMatrix, std::shared_ptr<SelfCorrelationMatrix> selfCorrelationMatrix);
        };
    }
}

