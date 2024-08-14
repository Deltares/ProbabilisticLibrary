#pragma once
#include "ImportanceSamplingSettingsS.h"
#include "SensitivityMethod.h"

namespace Deltares
{
    namespace Sensitivity
    {
        class ImportanceSamplingS : public SensitivityMethod
        {
        public:
            std::shared_ptr<ImportanceSamplingSettingsS> Settings = std::make_shared<ImportanceSamplingSettingsS>();
            std::shared_ptr<Statistics::Stochast> getStochast(std::shared_ptr<Models::ModelRunner> modelRunner) override;
        private:
            double getDimensionality(std::vector<double> factors);
            double getWeight(std::shared_ptr<Models::Sample> modifiedSample, std::shared_ptr<Models::Sample> sample, double dimensionality);
            std::vector<double> getFactors(std::shared_ptr<Reliability::StochastSettingsSet> stochastSettings);
            double getConvergence(int samples, double weightedSum);
        };
    }
}

