#pragma once
#include "CorrelationMatrixBuilder.h"
#include "../Statistics/Stochast.h"
#include "../Model/ModelRunner.h"

#include <memory>

namespace Deltares
{
    namespace Sensitivity
    {
        /**
         * \brief Base class for calculation of the sensitivity
         */
        class SensitivityMethod
        {
        public:

            virtual ~SensitivityMethod() = default;

            /**
             * \brief Gets the sensitivity
             * \param modelRunner The model for which the sensitivity is calculated
             * \return The sensitivity in the form of a stochastic variable
             */
            virtual std::shared_ptr<Statistics::Stochast> getStochast(std::shared_ptr<Models::ModelRunner> modelRunner) { return nullptr; }

            /**
             * \brief Gets the correlation matrix
             * \return Correlation matrix from all getStochast runs
             */
            virtual std::shared_ptr<Statistics::CorrelationMatrix> getCorrelationMatrix() { return this->correlationMatrixBuilder->getCorrelationMatrix(); }

            /**
             * \brief Indicates whether the calculation has been stopped
             */
            bool isStopped();

            /**
             * \brief Stops the calculation
             * \remark Usually called from another thread
             */
            void Stop();

        protected:
            virtual void setStopped();
            std::shared_ptr<Statistics::Stochast> getStochastFromSamples(std::vector<double>& samples, std::vector<double>& weights);

            /**
             * \brief Creates a correlation matrix from all sensitivity runs
             */
            std::shared_ptr<CorrelationMatrixBuilder> correlationMatrixBuilder = std::make_shared<CorrelationMatrixBuilder>();

        private:
            bool stopped = false;

        };
    }
}

