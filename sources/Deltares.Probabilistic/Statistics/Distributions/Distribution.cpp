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
#include "Distribution.h"

#include <algorithm>

#include "../../Math/NumericSupport.h"
#include "../../Math/RootFinders/BisectionRootFinder.h"
#include "../../Utils/probLibException.h"
#include "../StandardNormal.h"

#include <numbers>

namespace Deltares
{
    namespace Statistics
    {
        void Distribution::setXAtUByIteration(std::shared_ptr<StochastProperties> stochast, double x, double u, ConstantParameterType constantType)
        {
            const double delta = 0.00001;
            double margin = std::min(delta, std::fabs(x / 1000000));

            double currentMean = this->getMean(stochast);
            double currentDeviation = this->getDeviation(stochast);

            auto bisection = Numeric::BisectionRootFinder(margin);

            if (constantType == Deviation)
            {
                Numeric::RootFinderMethod function = [this, stochast, currentDeviation, u](double mean)
                {
                    this->setMeanAndDeviation(stochast, mean, currentDeviation);
                    return this->getXFromU(stochast, u);
                };

                double newMean = bisection.CalculateValue(x, currentMean, x, function);
                this->setMeanAndDeviation(stochast, newMean, currentDeviation);
            }
            else if (constantType == VariationCoefficient)
            {
                double variationCoefficient = currentMean == 0.0 ? currentDeviation : std::fabs(currentDeviation / currentMean);

                Numeric::RootFinderMethod function = [this, stochast, variationCoefficient, u](double mean)
                {
                    double deviation = std::fabs(mean * variationCoefficient);
                    this->setMeanAndDeviation(stochast, mean, deviation);
                    return this->getXFromU(stochast, u);
                };

                double newMean = bisection.CalculateValue(x, currentMean, x, function);

                this->setMeanAndDeviation(stochast, newMean, std::fabs(newMean * variationCoefficient));
            }
            else
            {
                throw Deltares::Reliability::probLibException("Constant type not supported");
            }
        }

        double Distribution::getXFromUByIteration(std::shared_ptr<StochastProperties> stochast, double u)
        {

            Numeric::RootFinderMethod function = [this, stochast](double x)
            {
                return this->getCDF(stochast, x);
            };

            double cdf = StandardNormal::getPFromU(u);

            double margin = std::min(std::fabs(1 - cdf) / 1000, std::fabs(cdf) / 1000);

            const double delta = 0.00001;
            margin = std::min(delta, margin);
            auto bisection = Numeric::BisectionRootFinder(margin, 0);

            double x = bisection.CalculateValue(0, 1, cdf, function);
            return x;
        }

        double Distribution::getLogLikelihood(std::shared_ptr<StochastProperties> stochast, double x)
        {
            return log(this->getPDF(stochast, x));
        }

        double Distribution::getFittedMinimum(std::vector<double>& x)
        {
            double min = Numeric::NumericSupport::getMinimum(x);
            double max = Numeric::NumericSupport::getMaximum(x);

            double diff = max - min;
            double add = diff / x.size();

            return min - add;
        }

        double Distribution::getMeanByIteration(std::shared_ptr<StochastProperties> stochast)
        {
            std::vector<double> values = getValuesForIteration(stochast);
            return Numeric::NumericSupport::getMean(values);
        }

        double Distribution::getDeviationByIteration(std::shared_ptr<StochastProperties> stochast)
        {
            std::vector<double> values = getValuesForIteration(stochast);
            return Numeric::NumericSupport::getStandardDeviation(values);
        }

        std::vector<double> Distribution::getValuesForIteration(std::shared_ptr<StochastProperties> stochast)
        {
            const int steps = 1000;

            std::vector<double> values(steps);

            for (int i = 0; i < steps; i++)
            {
                double p = (i + 0.5) / steps;
                double u = StandardNormal::getUFromP(p);
                double x = this->getXFromU(stochast, u);

                values[i] = x;
            }

            return values;
        }

        std::vector<std::shared_ptr<Numeric::WeightedValue>> Distribution::GetWeightedValues(std::vector<double>& values, std::vector<double>& weights)
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
}




