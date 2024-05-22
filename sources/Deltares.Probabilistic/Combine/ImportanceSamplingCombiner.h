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
        class ImportanceSamplingCombiner : public Combiner
        {
        public:
            std::shared_ptr<DesignPoint> CombineDesignPoints(const std::vector<std::shared_ptr<DesignPoint>>& designPoints, const std::vector<double>& rho, const combineAndOr cmbType) override;
            std::shared_ptr<DesignPoint> combineDesignPoints(combineAndOr combineMethodType, std::vector<std::shared_ptr<DesignPoint>>& designPoints, std::shared_ptr<Statistics::CorrelationMatrix> correlationMatrix = nullptr, std::shared_ptr<ProgressIndicator> progress = nullptr) override;
        private:
            double getApproximatedProbability(combineAndOr combineMethodType, std::vector<std::shared_ptr<DesignPoint>>& designPoints);
            void invert(std::shared_ptr<DesignPoint> designPoint);
            std::vector<std::shared_ptr<Statistics::Stochast>> getUniqueStochasts(std::vector<std::shared_ptr<DesignPoint>>& designPoints);
            std::shared_ptr<DesignPoint> combineDesignPointsAdjusted(combineAndOr combineMethodType, std::shared_ptr<Statistics::CorrelationMatrix> correlationMatrix, std::shared_ptr<ProgressIndicator> progress, std::vector<std::shared_ptr<DesignPoint>>& designPoints);
            std::shared_ptr<DesignPoint> getSeriesProbability(combineAndOr combineMethodType, std::shared_ptr<Statistics::CorrelationMatrix> correlationMatrix, std::shared_ptr<DesignPoint> currenDesignPoint, std::vector<std::shared_ptr<DesignPoint>>& previousDesignPoints, std::shared_ptr<DesignPoint> startPoint, std::vector<std::shared_ptr<Stochast>>& stochasts, std::shared_ptr<ProgressIndicator> progress);
            std::shared_ptr<DesignPoint> getParallelProbability(combineAndOr combineMethodType, std::shared_ptr<Statistics::CorrelationMatrix> correlationMatrix, std::vector<std::shared_ptr<DesignPoint>>& previousDesignPoints, std::vector<std::shared_ptr<Stochast>>& stochasts, std::shared_ptr<ProgressIndicator> progress);

            void fillSettingsSeries(std::shared_ptr<DesignPoint> startPoint, std::shared_ptr<CombinedDesignPointModel> model, std::shared_ptr<ImportanceSamplingSettings> settings);
            void fillSettingsParallel(std::shared_ptr<CombinedDesignPointModel> model, std::shared_ptr<ImportanceSamplingSettings> settings, double factor);

            std::shared_ptr<CombinedDesignPointModel> getModel(combineAndOr combineMethodType, std::shared_ptr<DesignPoint> currentDesignPoint, std::vector<std::shared_ptr<DesignPoint>>& previousDesignPoints, std::vector<std::shared_ptr<Stochast>>& stochasts);
            std::shared_ptr<Project> getProject(std::shared_ptr<CombinedDesignPointModel> model, std::shared_ptr<Statistics::CorrelationMatrix> correlationMatrix);
        };
    }
}

