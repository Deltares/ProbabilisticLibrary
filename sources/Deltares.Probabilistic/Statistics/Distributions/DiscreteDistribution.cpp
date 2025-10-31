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
#include "DiscreteDistribution.h"

#include <algorithm>
#include <cmath>

#include "DistributionSupport.h"
#include "../StandardNormal.h"
#include "../../Math/NumericSupport.h"


namespace Deltares
{
    namespace Statistics
    {
        void DiscreteDistribution::initializeForRun(StochastProperties& stochast)
        {
            std::sort(stochast.DiscreteValues.begin(), stochast.DiscreteValues.end(),
                [](const std::shared_ptr<DiscreteValue>& val1, const std::shared_ptr<DiscreteValue>& val2) {return val1->X < val2->X; });

            double sum = 0;
            for (std::shared_ptr<DiscreteValue>& discreteValue : stochast.DiscreteValues)
            {
                sum += discreteValue->Amount;
            }

            for (std::shared_ptr<DiscreteValue>& discreteValue : stochast.DiscreteValues)
            {
                discreteValue->NormalizedAmount = discreteValue->Amount / sum;
            }

            Utils::SetDirtyLambda setDirtyFunction = [&stochast]()
            {
                stochast.dirty = true;
            };

            double cumulative = 0;
            for (const std::shared_ptr<DiscreteValue>& discreteValue : stochast.DiscreteValues)
            {
                cumulative += discreteValue->NormalizedAmount;
                discreteValue->CumulativeNormalizedAmount = cumulative;

                discreteValue->setDirtyFunction(setDirtyFunction);
            }

            stochast.dirty = false;
        }

        void DiscreteDistribution::validate(Logging::ValidationReport& report, std::shared_ptr<StochastProperties> stochast, std::string& subject)
        {
            Logging::ValidationSupport::checkNotEmpty(report, stochast->DiscreteValues.size(), "discrete values", subject);

            for (std::shared_ptr<DiscreteValue> discreteValue : stochast->DiscreteValues)
            {
                discreteValue->validate(report, subject);
            }
        }

        bool DiscreteDistribution::isVarying(std::shared_ptr<StochastProperties> stochast)
        {
            return stochast->DiscreteValues.size() > 1;
        }

        double DiscreteDistribution::getXFromU(StochastProperties& stochast, double u)
        {
            constexpr double delta = 0.0000001;

            if (stochast.dirty)
            {
                initializeForRun(stochast);
            }

            if (stochast.DiscreteValues.empty())
            {
                return std::nan("");
            }

            const double p = StandardNormal::getPFromU(u);

            for (const auto& discreteValue : stochast.DiscreteValues)
            {
                if (discreteValue->CumulativeNormalizedAmount >= p - delta)
                {
                    return discreteValue->X;
                }
            }

            return stochast.DiscreteValues.back()->X;
        }

        double DiscreteDistribution::getUFromX(StochastProperties& stochast, double x)
        {
            if (stochast.dirty)
            {
                initializeForRun(stochast);
            }

            if (stochast.DiscreteValues.empty())
            {
                return std::nan("");
            }

            double prev = 0.0;

            for (const auto& discreteValue : stochast.DiscreteValues)
            {
                const double cum = discreteValue->CumulativeNormalizedAmount;

                if (x < discreteValue->X)
                {
                    return StandardNormal::getUFromP(prev);
                }
                else if (x == discreteValue->X)
                {
                    double p = (prev + cum) / 2.0;
                    return StandardNormal::getUFromP(p);
                }
                else
                {
                    prev = cum;
                }
            }

            return StandardNormal::getUFromP(prev);
        }

        std::vector<double> DiscreteDistribution::getDiscontinuityPoints(const StochastProperties& stochast)
        {
            std::vector<double> discontinuityPoints;
            for (const std::shared_ptr<DiscreteValue> & discreteValue : stochast.DiscreteValues)
            {
                if (discreteValue->Amount > 0)
                {
                    discontinuityPoints.push_back(discreteValue->X);
                }
            }

            return discontinuityPoints;
        }

        double DiscreteDistribution::getRepresentativeU(std::shared_ptr<StochastProperties> stochast, double u)
        {
            double x = this->getXFromU(*stochast, u);
            double uRepresentative = this->getUFromX(*stochast, x);

            return uRepresentative;
        }

        double DiscreteDistribution::getMean(StochastProperties& stochast)
        {
            if (stochast.dirty)
            {
                initializeForRun(stochast);
            }

            double sumX = 0;
            double sumAmounts = 0;

            for (const std::shared_ptr<DiscreteValue>& val : stochast.DiscreteValues)
            {
                sumX += val->X * val->Amount;
                sumAmounts += val->Amount;
            }

            return sumX / sumAmounts;
        }

        double DiscreteDistribution::getDeviation(StochastProperties& stochast)
        {
            double mean = getMean(stochast);

            double sumVariances = 0;
            double sumWeights = 0;

            for (const std::shared_ptr<DiscreteValue>& discreteValue : stochast.DiscreteValues)
            {
                const double diff = discreteValue->X - mean;

                sumVariances += diff * diff * discreteValue->Amount;
                sumWeights += discreteValue->Amount;
            }

            if (sumWeights == 0.0)
            {
                return 0;
            }
            else if (sumWeights <= 1.0)
            {
                return sqrt(sumVariances / sumWeights);
            }
            else
            {
                return sqrt(sumVariances / (sumWeights - 1.0));
            }
        }

        void DiscreteDistribution::setMeanAndDeviation(StochastProperties& stochast, double mean, double deviation)
        {
            if (stochast.dirty)
            {
                initializeForRun(stochast);
            }

            double currentMean = getMean(stochast);
            double diff = mean - currentMean;

            for (const std::shared_ptr<DiscreteValue>& val : stochast.DiscreteValues)
            {
                val->X += diff;
            }
        }

        double DiscreteDistribution::getPDF(StochastProperties& stochast, double x)
        {
            constexpr double delta = 0.0000001;

            if (stochast.dirty)
            {
                initializeForRun(stochast);
            }

            if (stochast.DiscreteValues.empty())
            {
                return std::nan("");
            }

            for (const auto& discreteValue : stochast.DiscreteValues)
            {
                if (Numeric::NumericSupport::areEqual(discreteValue->X, x, delta))
                {
                    return discreteValue->NormalizedAmount;
                }
            }

            return 0.0;
        }

        double DiscreteDistribution::getCDF(std::shared_ptr<StochastProperties> stochast, double x)
        {
            const double delta = 0.0000001;

            if (stochast->dirty)
            {
                initializeForRun(*stochast);
            }

            if (stochast->DiscreteValues.empty())
            {
                return std::nan("");
            }

            double p = 0;

            for (size_t i = 0; i < stochast->DiscreteValues.size(); i++)
            {
                if (Numeric::NumericSupport::areEqual(stochast->DiscreteValues[i]->X, x, delta))
                {
                    double pLow = p;
                    double pHigh = p + stochast->DiscreteValues[i]->NormalizedAmount;
                    double pAverage = (pLow + pHigh) / 2;

                    return pAverage;
                }
                else if (Numeric::NumericSupport::isLess(stochast->DiscreteValues[i]->X, x, delta))
                {
                    p += stochast->DiscreteValues[i]->NormalizedAmount;
                }
            }

            return p;
        }

        void DiscreteDistribution::fit(std::shared_ptr<StochastProperties> stochast, std::vector<double>& values, const double shift)
        {
            std::vector<double> weights = Numeric::NumericSupport::select(values, [](double x) {return 1.0; });
            return fitWeighted(stochast, values, weights);
        }

        void DiscreteDistribution::fitWeighted(std::shared_ptr<StochastProperties> stochast, std::vector<double>& values, std::vector<double>& weights)
        {
            stochast->DiscreteValues.clear();

            std::vector<std::shared_ptr<Numeric::WeightedValue>> x = DistributionSupport::GetWeightedValues(values, weights);

            for (size_t i = 0; i < x.size(); i++)
            {
                if (stochast->DiscreteValues.empty() || x[i]->value != x[i-1]->value)
                {
                    stochast->DiscreteValues.push_back(std::make_shared<DiscreteValue>(x[i]->value, x[i]->weight));
                }
                else
                {
                    stochast->DiscreteValues.back()->Amount += x[i]->weight;
                }
            }

            stochast->Observations = static_cast<int>(values.size());

            initializeForRun(*stochast);
        }

        std::vector<double> DiscreteDistribution::getSpecialPoints(std::shared_ptr<StochastProperties> stochast)
        {
            constexpr double offset = 0.000001;

            if (stochast->dirty)
            {
                initializeForRun(*stochast);
            }

            std::vector<double> specialPoints;

            if (!stochast->DiscreteValues.empty())
            {
                double min = stochast->DiscreteValues[0]->X;
                double max = stochast->DiscreteValues.back()->X;

                double bigOffset = (max - min) / 10;

                if (bigOffset < offset)
                {
                    bigOffset = 1;
                }

                specialPoints.push_back(stochast->DiscreteValues[0]->X - bigOffset);

                std::shared_ptr<DiscreteValue> previousValue = nullptr;

                for (size_t i = 0; i < stochast->DiscreteValues.size(); i++)
                {
                    std::shared_ptr<DiscreteValue> value = stochast->DiscreteValues[i];
                    if (previousValue != nullptr && Numeric::NumericSupport::areEqual(value->X, previousValue->X, offset))
                    {
                        double lastValueStochastValue = specialPoints[specialPoints.size() - 2];
                        double lastAfterStochastValue = specialPoints[specialPoints.size() - 1];

                        std::erase(specialPoints, lastValueStochastValue);
                        std::erase(specialPoints, lastAfterStochastValue);

                        specialPoints.push_back(value->X);
                        specialPoints.push_back(value->X + offset);
                    }
                    else
                    {
                        specialPoints.push_back(value->X - offset);
                        specialPoints.push_back(value->X);
                        specialPoints.push_back(value->X + offset);
                    }

                    previousValue = value;
                }

                specialPoints.push_back(stochast->DiscreteValues[stochast->DiscreteValues.size() - 1]->X + bigOffset);
            }

            return specialPoints;
        }

    }
}

