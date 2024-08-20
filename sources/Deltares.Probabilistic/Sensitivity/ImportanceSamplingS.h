#pragma once
#include "ImportanceSamplingSettingsS.h"
#include "SensitivityMethod.h"

namespace Deltares
{
    namespace Sensitivity
    {
        /**
         * \brief Calculates the sensitivity using the Importance Sampling algorithm
         * \remark This algorithm focuses on the tail of the sensitivity, but only works well when the start point (in the settings) is specified well by the user
         */
        class ImportanceSamplingS : public SensitivityMethod
        {
        public:
            /**
             * \brief Settings for this algorithm
             */
            std::shared_ptr<ImportanceSamplingSettingsS> Settings = std::make_shared<ImportanceSamplingSettingsS>();

            /**
             * \brief Gets the sensitivity
             * \param modelRunner The model for which the sensitivity is calculated
             * \return The sensitivity in the form of a stochastic variable
             */
            std::shared_ptr<Statistics::Stochast> getStochast(std::shared_ptr<Models::ModelRunner> modelRunner) override;
        private:
            double getDimensionality(std::vector<double> factors);
            double getWeight(std::shared_ptr<Models::Sample> modifiedSample, std::shared_ptr<Models::Sample> sample, double dimensionality);
            std::vector<double> getFactors(std::shared_ptr<Reliability::StochastSettingsSet> stochastSettings);
            double getConvergence(int samples, double weightedSum);
        };
    }
}

