#include "CrudeMonteCarloSettingsS.h"

namespace Deltares
{
    namespace Sensitivity
    {
        /**
 * \brief Gets the number of runs which is needed to achieve the variation coefficient at the probability for convergence
 */
        int CrudeMonteCarloSettingsS::getRequiredSamples()
        {
            double probability = this->ProbabilityForConvergence;
            double variationCoefficient = this->VariationCoefficient;

            if (probability > 0.5)
            {
                probability = 1 - probability;
            }

            double samples = (1 - probability) / (variationCoefficient * variationCoefficient * probability);

            return static_cast<int>(std::ceil(samples));
        }

        /**
         * \brief Modifies the variation coefficient so that the number of required samples matches a given value
         */
        void CrudeMonteCarloSettingsS::setRequiredSamples(int samples)
        {
            double probability = this->ProbabilityForConvergence;
            if (probability > 0 && probability < 1)
            {
                if (probability > 0.5)
                {
                    probability = 1 - probability;
                }
                double varPf = sqrt((1 - probability) / (samples * probability));
                this->VariationCoefficient = varPf;
            }
        }
    }
}


