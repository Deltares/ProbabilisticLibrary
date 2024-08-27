#pragma once
#include "CrudeMonteCarloSettingsS.h"
#include "SensitivityMethod.h"

namespace Deltares
{
    namespace Sensitivity
    {
        /**
         * \brief Calculates the sensitivity using the Crude Monte Carlo algorithm
         */
        class CrudeMonteCarloS : public SensitivityMethod
        {
        public:
            /**
             * \brief Settings for this algorithm
             */
            std::shared_ptr<CrudeMonteCarloSettingsS> Settings = std::make_shared<CrudeMonteCarloSettingsS>();

            /**
             * \brief Gets the sensitivity
             * \param modelRunner The model for which the sensitivity is calculated
             * \return The sensitivity in the form of a stochastic variable
             */
            std::shared_ptr<Statistics::Stochast> getStochast(std::shared_ptr<Models::ModelRunner> modelRunner) override;
        private:
            bool checkConvergence(double pf, int samples, int sampleIndex);
            double getConvergence(double pf, int samples);
        };
    }
}

