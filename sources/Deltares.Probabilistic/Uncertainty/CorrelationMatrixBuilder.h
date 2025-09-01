// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of the Probabilistic Library.
//
// The Probabilistic Library is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// All names, logos, and references to "Deltares" are registered trademarks of
// Stichting Deltares and remain full property of Stichting Deltares at all times.
// All rights reserved.
//
#pragma once

#include <memory>

#include "../Statistics/CorrelationMatrix.h"

namespace Deltares::Uncertainty
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
         * \brief Registers the sample values and weights of an uncertainty calculation
         * \param stochast The model for which the sensitivity is calculated
         * \param weightedValues sample values and weights
         */
        void registerWeightedValues(const std::shared_ptr<Statistics::Stochast> stochast, const std::vector<std::shared_ptr<Numeric::WeightedValue>>& weightedValues);

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

