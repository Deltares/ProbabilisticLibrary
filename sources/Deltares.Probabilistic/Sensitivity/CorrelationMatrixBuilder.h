#pragma once

#include <memory>

#include "../Math/WeightedValue.h"
#include "../Statistics/CorrelationMatrix.h"

namespace Deltares
{
    namespace Sensitivity
    {
        class CorrelationMatrixBuilder
        {
        public:
            /**
             * \brief Registers weights for all samples
             */
            void registerWeights(const std::vector<double>& weights);

            /**
             * \brief Registers the sample values of an uncertainty calculation
             * \param stochast The model for which the sensitivity is calculated
             * \param values sample values
             */
            void registerSamples(const std::shared_ptr<Statistics::Stochast> stochast, const std::vector<double>& values);

            /**
             * \brief Registers the sample input values
             * \param sample sample values
             */
            void registerStochastValue(std::shared_ptr<Statistics::Stochast> stochast, double u);

            /**
             * \brief Creates a correlation matrix for all registered stochasts
             * \return Correlation matrix
             */
            std::shared_ptr<Statistics::CorrelationMatrix> getCorrelationMatrix();

            /**
             * \brief Indicates whether input values have been added
             * \return Indication
             */
            bool isEmpty()
            {
                return stochasts.empty();
            }

        private:
            bool containsInputValues = false;
            std::vector< std::shared_ptr<Statistics::Stochast>> stochasts;
            std::vector<double> weights;
            std::unordered_map<std::shared_ptr<Statistics::Stochast>, std::vector<double>> stochastValues;

            /**
             * \brief Calculates the correlation value between wo stochasts
             * \param x First stochast
             * \param y Second stochast
             * \return Correlation value
             * \remark See https://en.wikipedia.org/wiki/Pearson_correlation_coefficient
             */
            double getCorrelationValue(std::shared_ptr<Statistics::Stochast> x, std::shared_ptr<Statistics::Stochast> y);
        };
    }
}

