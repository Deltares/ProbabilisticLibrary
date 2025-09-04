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
#include "CorrelationMatrixBuilder.h"

namespace Deltares::Uncertainty
{
    void CorrelationMatrixBuilder::registerWeights(const std::vector<double>& weights)
    {
        this->weights = weights;
    }

    void CorrelationMatrixBuilder::registerSamples(const std::shared_ptr<Statistics::Stochast> stochast, const std::vector<double>& values)
    {
        this->stochasts.push_back(stochast);

        stochast->initializeForRun();

        std::vector<double> uValues;

        for (size_t i = 0; i < values.size(); i++)
        {
            uValues.push_back(stochast->getUFromX(values[i]));
        }

        this->stochastValues[stochast] = uValues;
    };

    void CorrelationMatrixBuilder::registerWeightedValues(const std::shared_ptr<Statistics::Stochast> stochast, const std::vector<std::shared_ptr<Numeric::WeightedValue>>& weightedValues)
    {
        std::vector<double> values;
        std::vector<double> weights;

        for (auto weightedValue : weightedValues)
        {
            values.push_back(weightedValue->value);
            weights.push_back(weightedValue->weight);
        }

        registerWeights(weights);
        registerSamples(stochast, values);
    }

    void CorrelationMatrixBuilder::registerStochastValue(std::shared_ptr<Statistics::Stochast> stochast, double u)
    {
        if (!this->stochastValues.contains(stochast))
        {
            this->stochasts.push_back(stochast);
            this->stochastValues[stochast] = std::vector<double>();
        }

        this->stochastValues[stochast].push_back(u);
    }


    double CorrelationMatrixBuilder::getCorrelationValue(std::shared_ptr<Statistics::Stochast> x, std::shared_ptr<Statistics::Stochast> y)
    {
        double sumCross = 0;
        double sumProducts2X = 0;
        double sumProducts2Y = 0;

        std::vector<double> xValues = this->stochastValues[x];
        std::vector<double> yValues = this->stochastValues[y];

        for (size_t i = 0; i < std::min(xValues.size(), yValues.size()); i++)
        {
            double weight = weights.empty() ? 1.0 : weights[i];

            sumCross += weight * xValues[i] * yValues[i];
            sumProducts2X += weight * xValues[i] * xValues[i];
            sumProducts2Y += weight * yValues[i] * yValues[i];
        }

        double correlationCoefficient = sumCross / std::sqrt(sumProducts2X * sumProducts2Y);

        return correlationCoefficient;
    }

    std::shared_ptr<Statistics::CorrelationMatrix> CorrelationMatrixBuilder::getCorrelationMatrix()
    {
        std::shared_ptr<Statistics::CorrelationMatrix> correlationMatrix = std::make_shared<Statistics::CorrelationMatrix>();

        correlationMatrix->init(this->stochasts);

        for (size_t i = 0; i < stochasts.size(); i++)
        {
            for (size_t j = 0; j < i; j++)
            {
                double correlationValue = this->getCorrelationValue(stochasts[i], stochasts[j]);
                correlationMatrix->SetCorrelation(stochasts[i], stochasts[j], correlationValue);
            }
        }

        return correlationMatrix;
    }
}

