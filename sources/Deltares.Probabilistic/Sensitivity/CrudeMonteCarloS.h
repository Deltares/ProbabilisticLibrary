#pragma once
#include "CrudeMonteCarloSettingsS.h"
#include "SensitivityMethod.h"

namespace Deltares
{
    namespace Sensitivity
    {
        class CrudeMonteCarloS : public SensitivityMethod
        {
        public:
            std::shared_ptr<CrudeMonteCarloSettingsS> Settings = std::make_shared<CrudeMonteCarloSettingsS>();
            std::shared_ptr<Statistics::Stochast> getStochast(std::shared_ptr<Models::ModelRunner> modelRunner) override;
        private:
            bool checkConvergence(double pf, int samples, int sampleIndex);
            double getConvergence(double pf, int samples);
        };
    }
}

