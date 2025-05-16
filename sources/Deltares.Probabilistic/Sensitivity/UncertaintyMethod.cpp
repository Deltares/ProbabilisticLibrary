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

#include <memory>
#include <algorithm>
#include "UncertaintyMethod.h"
#include "../Math/NumericSupport.h"

namespace Deltares::Sensitivity
{
    bool SensitivityMethod::isStopped() const
    {
        return stopped;
    }

    void SensitivityMethod::setStopped()
    {
        stopped = true;
    }

    void SensitivityMethod::Stop()
    {
        setStopped();
    }

    void SensitivityMethod::filterSamples(std::vector<double>& samples, std::vector<double>& weights)
    {
        bool hasInvalidValues = false;

        for (size_t i = samples.size(); i < samples.size(); i++)
        {
            if (std::isnan(samples[i]) || std::isnan(weights[i]) || weights[i] == 0.0)
            {
                hasInvalidValues = true;
                break;
            }
        }

        if (hasInvalidValues)
        {
            std::vector<double> newSamples;
            std::vector<double> newWeights;

            for (size_t i = 0; i < samples.size(); i++)
            {
                if (!(std::isnan(samples[i]) || std::isnan(weights[i]) || weights[i] == 0.0))
                {
                    newSamples.push_back(samples[i]);
                    newWeights.push_back(weights[i]);
                }
            }

            samples.clear();
            weights.clear();

            for (size_t i = 0; i < newSamples.size(); i++)
            {
                samples.push_back(newSamples[i]);
                weights.push_back(newWeights[i]);
            }
        }
    }

    std::shared_ptr<Statistics::Stochast> SensitivityMethod::getStochastFromSamples(std::vector<std::shared_ptr<Numeric::WeightedValue>>& weightedValues)
    {
        std::vector<double> values;
        std::vector<double> weights;

        for (const std::shared_ptr<Numeric::WeightedValue>& weightedValue : weightedValues)
        {
            values.push_back(weightedValue->value);
            weights.push_back(weightedValue->weight);
        }

        return getStochastFromSamples(values, weights);
    }

    std::shared_ptr<Statistics::Stochast> SensitivityMethod::getStochastFromSamples(std::vector<double>& samples, std::vector<double>& weights)
    {
        filterSamples(samples, weights);

        auto stochast = std::make_shared<Statistics::Stochast>();

        if (samples.empty())
        {
            stochast->setDistributionType(Statistics::DistributionType::Deterministic);
            stochast->getProperties()->Location = std::nan("");
        }
        else
        {
            const double min = Numeric::NumericSupport::getMinimum(samples);
            const double max = Numeric::NumericSupport::getMaximum(samples);

            if (min == max)
            {
                stochast->setDistributionType(Statistics::DistributionType::Deterministic);
                stochast->getProperties()->Location = min;
            }
            else
            {
                stochast->setDistributionType(Statistics::DistributionType::Table);
                stochast->fitWeighted(samples, weights);
                stochast->distributionChangeType = Statistics::FitFromHistogramValues;
            }
        }

        return stochast;
    }

    int SensitivityMethod::getQuantileIndex(const std::vector<std::shared_ptr<Numeric::WeightedValue>>& weightedValues, double quantile)
    {
        std::vector<double> values;
        std::vector<double> weights;

        for (const std::shared_ptr<Numeric::WeightedValue>& weightedValue : weightedValues)
        {
            values.push_back(weightedValue->value);
            weights.push_back(weightedValue->weight);
        }

        return getQuantileIndex(values, weights, quantile);
    }

    int SensitivityMethod::getQuantileIndex(std::vector<double>& samples, std::vector<double>& weights, double quantile)
    {
        if (samples.empty())
        {
            return -1;
        }

        std::vector<std::shared_ptr<Numeric::WeightedValue>> values;
        std::unordered_map<std::shared_ptr<Numeric::WeightedValue>, size_t> index;
        double sumWeights = 0;
        for (size_t i = 0; i < samples.size(); i++)
        {
            std::shared_ptr<Numeric::WeightedValue> value = std::make_shared<Numeric::WeightedValue>(samples[i], weights[i]);
            index[value] = i;
            values.push_back(value);
            if (!std::isnan(weights[i]))
            {
                sumWeights += weights[i];
            }
        }

        std::sort(values.begin(), values.end(), []
            (const std::shared_ptr<Numeric::WeightedValue>& lhs, const std::shared_ptr<Numeric::WeightedValue>& rhs)
        {
            return lhs->value < rhs->value;
        });

        double cumulativeWeight = 0;
        const double requestedCumulativeWeight = sumWeights * quantile;

        for (const auto& value: values)
        {
            if (!std::isnan(value->weight))
            {
                cumulativeWeight += value->weight;
                if (cumulativeWeight >= requestedCumulativeWeight)
                {
                    return static_cast<int>(index[value]);
                }
            }
        }

        // should not come here
        return static_cast<int>(index[values.back()]);
    }
}


