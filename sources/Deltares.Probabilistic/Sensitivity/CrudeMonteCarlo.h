#pragma once
#include "CrudeMonteCarloSettings.h"
#include "SensitivityMethod.h"

namespace Deltares
{
    namespace Sensitivity
    {
        class CrudeMonteCarlo : public SensitivityMethod
        {
        public:
            std::shared_ptr<CrudeMonteCarloSettings> Settings = std::make_shared<CrudeMonteCarloSettings>();
            std::shared_ptr<Statistics::Stochast> getStochast(std::shared_ptr<Models::ModelRunner> modelRunner) override;
        private:
            bool checkConvergence(double pf, int samples, int sampleIndex);
            double getConvergence(double pf, int samples);
        };
    }
}

