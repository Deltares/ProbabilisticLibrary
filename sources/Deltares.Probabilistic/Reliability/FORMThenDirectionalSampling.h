#pragma once

#include "FORMSettings.h"
#include "DirectionalSamplingSettings.h"
#include "ReliabilityMethod.h"

namespace Deltares
{
    namespace Reliability
    {
        // class for FORMThenDirectionalSampling: FORM, but in case of non convergence fall back on Directional Sampling
        class FORMThenDirectionalSampling : public ReliabilityMethod
        {
        public:
            FORMThenDirectionalSampling() {}
            FORMThenDirectionalSampling(const double threshold) : thresholdBeta(threshold) {}
            std::shared_ptr<FORMSettings> formSettings = std::make_shared<FORMSettings>();
            std::shared_ptr<DirectionalSamplingSettings> DsSettings = std::make_shared<DirectionalSamplingSettings>();
            std::shared_ptr<DesignPoint> getDesignPoint(std::shared_ptr<Models::ModelRunner> modelRunner) override;
        private:
            const double thresholdBeta = -999.0; // minimal beta value to accept as result from FORM
        };
    }
}
