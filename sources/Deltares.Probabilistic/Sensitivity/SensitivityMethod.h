#pragma once
#include "../Statistics/Stochast.h"
#include "../Model/ModelRunner.h"

namespace Deltares
{
    namespace Sensitivity
    {
        /**
         * \brief Base class for calculation of the sensitivity
         */
        class SensitivityMethod
        {
        private:
            bool stopped = false;

        protected:

            virtual void setStopped();
            std::shared_ptr<Statistics::Stochast> getStochastFromSamples(std::vector<double>& samples, std::vector<double>& weights);

        public:
            /**
             * \brief Gets the sensitivity
             * \param modelRunner The model for which the sensitivity is calculated
             * \return The sensitivity in the form of a stochastic variable
             */
            virtual std::shared_ptr<Statistics::Stochast> getStochast(std::shared_ptr<Models::ModelRunner> modelRunner) { return nullptr; }

            virtual ~SensitivityMethod() = default;

            /**
             * \brief Indicates whether the calculation has been stopped
             */
            bool isStopped();

            /**
             * \brief Stops the calculation
             * \remark Usually called from another thread
             */
            void Stop();
        };
    }
}

