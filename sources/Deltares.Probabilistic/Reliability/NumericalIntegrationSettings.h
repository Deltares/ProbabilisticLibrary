#pragma once
#include "DesignPointBuilder.h"

namespace Deltares
{
    namespace Reliability
    {
        /**
         * \brief Settings for numerical integration
         */
        class NumericalIntegrationSettings
        {
        public:
            /**
             * \brief Method type how the design point (alpha values) is calculated
             */
            DesignPointMethod designPointMethod = DesignPointMethod::CenterOfGravity;

            /**
             * \brief Settings per stochastic variable, contains (among others) the center value and multiplication factor used to shift samples in the importance sampling algorithm
             */
            std::shared_ptr<StochastSettingsSet> StochastSet = std::make_shared<StochastSettingsSet>();

            /**
             * \brief Settings for performing model runs
             */
            std::shared_ptr<Models::RunSettings> runSettings = std::make_shared<Models::RunSettings>();

            /**
             * \brief Indicates whether these settings have valid values
             * \return Indication
             */
            bool isValid() const
            {
                return
                    this->isStochastSetValid() &&
                    runSettings->isValid();
            }

        private:
            /**
             * \brief Indicates whether all stochast settings have valid values
             * \return True if all valid, false otherwise
             */
            bool isStochastSetValid() const
            {
                for (size_t i = 0; i < this->StochastSet->getVaryingStochastCount(); i++)
                {
                    const std::shared_ptr<StochastSettings> stochastSettings = this->StochastSet->VaryingStochastSettings[i];
                    const bool valid =
                        stochastSettings->MinValue < stochastSettings->MaxValue &&
                        stochastSettings->Intervals >= 1 &&
                        stochastSettings->MinValue >= -Statistics::StandardNormal::UMax && stochastSettings->MinValue <= Statistics::StandardNormal::UMax &&
                        stochastSettings->MaxValue >= -Statistics::StandardNormal::UMax && stochastSettings->MaxValue <= Statistics::StandardNormal::UMax;

                    if (!valid)
                    {
                        return false;
                    }
                }

                return true;
            }
        };
    }
}

