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
#include "DistributionSupport.h"

#include <algorithm>

#include "../../Math/NumericSupport.h"
#include "../../Math/RootFinders/BisectionRootFinder.h"
#include "../../Utils/probLibException.h"
#include "../StandardNormal.h"

namespace Deltares::Statistics
{
    void DistributionSupport::setXAtUByIteration(Distribution& distribution, StochastProperties& stochast, double x, double u, ConstantParameterType constantType)
    {
        constexpr double delta = 0.00001;
        const double margin = std::min(delta, std::fabs(x / 1000000));

        double currentMean = distribution.getMean(stochast);
        double currentDeviation = distribution.getDeviation(stochast);

        auto bisection = Numeric::BisectionRootFinder(margin);

        if (constantType == ConstantParameterType::Deviation)
        {
            Numeric::RootFinderMethod function = [&distribution, &stochast, currentDeviation, u](double mean)
            {
                distribution.setMeanAndDeviation(stochast, mean, currentDeviation);
                return distribution.getXFromU(stochast, u);
            };

            double newMean = bisection.CalculateValue(x, currentMean, x, function);
            distribution.setMeanAndDeviation(stochast, newMean, currentDeviation);
        }
        else if (constantType == ConstantParameterType::VariationCoefficient)
        {
            double variationCoefficient = currentMean == 0.0 ? currentDeviation : std::fabs(currentDeviation / currentMean);

            Numeric::RootFinderMethod function = [&distribution, &stochast, variationCoefficient, u](double mean)
            {
                double deviation = std::fabs(mean * variationCoefficient);
                distribution.setMeanAndDeviation(stochast, mean, deviation);
                return distribution.getXFromU(stochast, u);
            };

            double newMean = bisection.CalculateValue(x, currentMean, x, function);

            distribution.setMeanAndDeviation(stochast, newMean, std::fabs(newMean * variationCoefficient));
        }
        else
        {
            throw Deltares::Reliability::probLibException("Constant type not supported");
        }
    }

    double DistributionSupport::getXFromUByIteration(Distribution& distribution, StochastProperties& stochast, double u)
    {
        // check whether the cdf value is at a discontinuity

        for (double x : distribution.getDiscontinuityPoints(stochast))
        {
            constexpr double minDiff = 1E-6;
            constexpr double fractionDiff = 1E-10;

            double diff = std::max(minDiff, std::abs(x) * fractionDiff);
            if (distribution.getUFromX(stochast, x - diff) <= u && distribution.getUFromX(stochast, x + diff) >= u)
            {
                return x;
            }
        }

        // not at a discontinuity, search by bisection

        Numeric::RootFinderMethod function = [&distribution, &stochast](double x)
        {
            return distribution.getCDF(stochast, x);
        };

        double cdf = StandardNormal::getPFromU(u);
        double margin = std::min(std::fabs(1 - cdf) / 1000, std::fabs(cdf) / 1000);

        constexpr double delta = 0.00001;
        margin = std::min(delta, margin);
        auto bisection = Numeric::BisectionRootFinder(margin, 0);

        double x = bisection.CalculateValue(0, 1, cdf, function);
        return x;
    }

    double DistributionSupport::getMeanByIteration(Distribution& distribution, StochastProperties& stochast)
    {
        std::vector<double> values = getValuesForIteration(distribution, stochast);
        return Numeric::NumericSupport::getMean(values);
    }

    double DistributionSupport::getMeanByIteration(Distribution& distribution, std::shared_ptr<StochastProperties> stochast)
    {
        return getMeanByIteration(distribution, *stochast);
    }

    double DistributionSupport::getDeviationByIteration(Distribution& distribution, std::shared_ptr<StochastProperties> stochast)
    {
        std::vector<double> values = getValuesForIteration(distribution, *stochast);
        return Numeric::NumericSupport::getStandardDeviation(values);
    }

    double DistributionSupport::getDeviationByIteration(Distribution& distribution, StochastProperties& stochast)
    {
        std::vector<double> values = getValuesForIteration(distribution, stochast);
        return Numeric::NumericSupport::getStandardDeviation(values);
    }

    std::vector<double> DistributionSupport::getValuesForIteration(Distribution& distribution, std::shared_ptr<StochastProperties> stochast)
    {
        return getValuesForIteration(distribution, *stochast);
    }

    std::vector<double> DistributionSupport::getValuesForIteration(Distribution& distribution, StochastProperties& stochast)
    {
        constexpr int steps = 1000;

        std::vector<double> values(steps);

        for (int i = 0; i < steps; i++)
        {
            double p = (i + 0.5) / steps;
            double u = StandardNormal::getUFromP(p);
            double x = distribution.getXFromU(stochast, u);

            values[i] = x;
        }

        return values;
    }

    std::vector<std::shared_ptr<Numeric::WeightedValue>> DistributionSupport::GetWeightedValues(const std::vector<double>& values, const std::vector<double>& weights)
    {
        std::vector<std::shared_ptr<Numeric::WeightedValue>> weightedValues;

        for (size_t i = 0; i < values.size(); i++)
        {
            if (!std::isnan(values[i]))
            {
                weightedValues.push_back(std::make_shared<Numeric::WeightedValue>(values[i], weights[i]));
            }
        }

        if (!weightedValues.empty())
        {
            std::sort(weightedValues.begin(), weightedValues.end(), [](std::shared_ptr<Numeric::WeightedValue> val1, std::shared_ptr<Numeric::WeightedValue> val2) {return val1->value < val2->value; });
        }

        return weightedValues;
    }
}




