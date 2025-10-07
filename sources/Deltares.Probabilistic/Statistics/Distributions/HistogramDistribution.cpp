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
#include "HistogramDistribution.h"

#include <algorithm>
#include <cmath>
#include "../StochastProperties.h"
#include "../StandardNormal.h"
#include "../../Math/NumericSupport.h"
#include "../../Math/WeightedValue.h"
#include "../../Utils/DirtySupport.h"

#include <limits>

namespace Deltares
{
    namespace Statistics
    {
        void HistogramDistribution::initializeForRun(std::shared_ptr<StochastProperties> stochast)
        {
            // check whether it is already sorted
            bool isSorted = true;
            for (size_t i = 1; i < stochast->HistogramValues.size(); i++)
            {
                if (!stochast->HistogramValues[i]->compareTo(stochast->HistogramValues[i - 1]))
                {
                    isSorted = false;
                    break;
                }
            }

            // if not sorted, sort
            if (!isSorted)
            {
                std::sort(stochast->HistogramValues.begin(), stochast->HistogramValues.end(), [](std::shared_ptr<HistogramValue> val1, std::shared_ptr<HistogramValue> val2) {return val1->compareTo(val2); });
            }

            double sum = 0;
            for (std::shared_ptr<HistogramValue> histogramValue : stochast->HistogramValues)
            {
                sum += histogramValue->Amount;
            }

            for (std::shared_ptr<HistogramValue> histogramValue : stochast->HistogramValues)
            {
                histogramValue->NormalizedAmount = histogramValue->Amount / sum;
            }

            Utils::SetDirtyLambda setDirtyFunction = [stochast]()
            {
                stochast->dirty = true;
            };

            double cumulative = 0;
            for (std::shared_ptr<HistogramValue> histogramValue : stochast->HistogramValues)
            {
                cumulative += histogramValue->NormalizedAmount;
                histogramValue->CumulativeNormalizedAmount = cumulative;

                histogramValue->setDirtyFunction(setDirtyFunction);
            }

            stochast->dirty = false;
        }

        void HistogramDistribution::setMeanAndDeviation(std::shared_ptr<StochastProperties> stochast, double mean, double deviation)
        {
            if (stochast->dirty)
            {
                initializeForRun(stochast);
            }

            double currentMean = this->getMean(stochast);
            double diff = mean - currentMean;

            for (std::shared_ptr<HistogramValue> histogramValue : stochast->HistogramValues)
            {
                histogramValue->LowerBound += diff;
                histogramValue->UpperBound += diff;
            }
        }

        void HistogramDistribution::validate(Logging::ValidationReport& report, std::shared_ptr<StochastProperties> stochast, std::string& subject)
        {
            if (stochast->dirty)
            {
                initializeForRun(stochast);
            }

            Logging::ValidationSupport::checkNotEmpty(report, stochast->HistogramValues.size(), "histogram values", subject);

            std::shared_ptr<HistogramValue> previousHistogramValue = nullptr;

            for (std::shared_ptr<HistogramValue> histogramValue : stochast->HistogramValues)
            {
                histogramValue->validate(report, previousHistogramValue, subject);
                previousHistogramValue = histogramValue;
            }
        }


        bool HistogramDistribution::isVarying(std::shared_ptr<StochastProperties> stochast)
        {
            return !stochast->HistogramValues.empty();
        }

        double HistogramDistribution::getMean(std::shared_ptr<StochastProperties> stochast)
        {
            if (stochast->dirty)
            {
                initializeForRun(stochast);
            }

            double sum = 0;
            double sumWeights = 0;

            for (std::shared_ptr<HistogramValue> histogramValue : stochast->HistogramValues)
            {
                sum += histogramValue->getCenter() * histogramValue->NormalizedAmount;
                sumWeights += histogramValue->NormalizedAmount;
            }

            return sum / sumWeights;
        }

        double HistogramDistribution::getDeviation(std::shared_ptr<StochastProperties> stochast)
        {
            double mean = getMean(stochast);

            double sumVariances = 0;
            double sumWeights = 0;

            for (std::shared_ptr<HistogramValue> histogramValue : stochast->HistogramValues)
            {
                const double diff = histogramValue->getCenter() - mean;

                sumVariances += diff * diff * histogramValue->Amount;
                sumWeights += histogramValue->Amount;
            }

            if (sumWeights == 0)
            {
                return 0;
            }
            else if (sumWeights <= 1)
            {
                return sqrt(sumVariances / sumWeights);
            }
            else
            {
                return sqrt(sumVariances / (sumWeights - 1));
            }
        }

        double HistogramDistribution::getXFromU(std::shared_ptr<StochastProperties> stochast, double u)
        {
            if (stochast->dirty)
            {
                initializeForRun(stochast);
            }

            double p = StandardNormal::getPFromU(u);

            if (stochast->HistogramValues.empty())
            {
                return std::nan("");
            }

            for (size_t i = 0; i < stochast->HistogramValues.size(); i++)
            {
                if (stochast->HistogramValues[i]->CumulativeNormalizedAmount >= p)
                {
                    double previousCumulativeAmount = i > 0 ? stochast->HistogramValues[i - 1]->CumulativeNormalizedAmount : 0;
                    double offset = (p - previousCumulativeAmount) / (stochast->HistogramValues[i]->CumulativeNormalizedAmount - previousCumulativeAmount);

                    // linear interpolation within range
                    return offset * stochast->HistogramValues[i]->UpperBound + (1 - offset) * stochast->HistogramValues[i]->LowerBound;
                }
            }

            return stochast->HistogramValues.back()->UpperBound;
        }

        double HistogramDistribution::getUFromX(std::shared_ptr<StochastProperties> stochast, double x)
        {
            if (stochast->dirty)
            {
                initializeForRun(stochast);
            }

            double cdf = this->getCDF(stochast, x);

            return StandardNormal::getUFromP(cdf);
        }

        double HistogramDistribution::getSizeForEmptySizedRange(std::shared_ptr<StochastProperties> stochast)
        {
            double max = std::numeric_limits<double>::infinity();

            for (std::shared_ptr<HistogramValue> histogramValue : stochast->HistogramValues)
            {
                const double size = histogramValue->getSize();
                if (size < max && size > 0)
                {
                    max = size;
                }
            }

            if (!std::isinf(max))
            {
                return max;
            }
            else
            {
                return 1;
            }
        }

        double HistogramDistribution::getPDF(std::shared_ptr<StochastProperties> stochast, double x)
        {
            constexpr double delta = 0.0000001;

            if (stochast->dirty)
            {
                initializeForRun(stochast);
            }

            getSizeForEmptySizedRange(stochast);

            for (const auto& histogram_value : stochast->HistogramValues)
            {
                if (Numeric::NumericSupport::areEqual(histogram_value->LowerBound, histogram_value->UpperBound, delta) &&
                    Numeric::NumericSupport::areEqual(histogram_value->LowerBound, x, delta))
                {
                    return histogram_value->NormalizedAmount / getSizeForEmptySizedRange(stochast);
                }
                else
                {
                    if (histogram_value->contains(x))
                    {
                        return histogram_value->NormalizedAmount / histogram_value->getSize();
                    }
                }
            }

            return 0;
        }

        double HistogramDistribution::getCDF(std::shared_ptr<StochastProperties> stochast, double x)
        {
            if (stochast->dirty)
            {
                initializeForRun(stochast);
            }

            if (stochast->HistogramValues.empty())
            {
                return std::nan("");
            }

            double p = 0;

            for (const auto& histogram_value : stochast->HistogramValues)
            {
                if (histogram_value->LowerBound < x)
                {
                    if (histogram_value->UpperBound < x)
                    {
                        p += histogram_value->NormalizedAmount;
                    }
                    else
                    {
                        const double offset = (x - histogram_value->LowerBound) / (histogram_value->UpperBound - histogram_value->LowerBound);
                        p += histogram_value->NormalizedAmount * offset;
                    }
                }
            }

            return p;
        }

        std::vector<double> HistogramDistribution::getDiscontinuityPoints(const StochastProperties& stochast)
        {
            const double minDiff = 1E-6;

            std::vector<double> discontinuityPoints;
            for (const std::shared_ptr<HistogramValue>&  value : stochast.HistogramValues)
            {
                if (value->Amount > 0 && std::abs(value->UpperBound - value->LowerBound) <= minDiff)
                {
                    discontinuityPoints.push_back(value->LowerBound);
                }
            }

            return discontinuityPoints;
        }

        std::vector<double> HistogramDistribution::getSpecialPoints(std::shared_ptr<StochastProperties> stochast)
        {
            constexpr double delta = 0.0000001;

            if (stochast->dirty)
            {
                initializeForRun(stochast);
            }

            std::vector<double> x;

            for (std::shared_ptr<HistogramValue> range : stochast->HistogramValues)
            {
                if (range->UpperBound - delta > range->LowerBound + delta)
                {
                    x.push_back(range->LowerBound - delta);
                    x.push_back(range->LowerBound + delta);

                    x.push_back(range->UpperBound - delta);
                    x.push_back(range->UpperBound + delta);
                }
                else
                {
                    x.push_back(range->LowerBound - delta);
                    x.push_back((range->LowerBound + range->UpperBound) / 2);
                    x.push_back(range->UpperBound + delta);
                }
            }

            return x;
        }

        void HistogramDistribution::fit(std::shared_ptr<StochastProperties> stochast, std::vector<double>& values)
        {
            auto weights = std::vector(values.size(), 1.0);
            return fitWeighted(stochast, values, weights);
        }

        void HistogramDistribution::fitWeighted(std::shared_ptr<StochastProperties> stochast, std::vector<double>& values, std::vector<double>& weights)
        {
            constexpr int maxRanges = 100;
            constexpr double addFactor = 0.1;
            constexpr double accuracy = 1E-10;
            constexpr double distinctFraction = 0.1;

            if (values.size() != weights.size())
            {
                throw Reliability::probLibException("Histogram fit: sizes values and weighs should be same");
            }

            // Build up list of weighted values
            std::vector<std::shared_ptr<Numeric::WeightedValue>> x = this->GetWeightedValues(values, weights);

            if (x.empty())
            {
                stochast->HistogramValues.clear();
                return;
            }

            mergeLowWeights(x);

            // assume a distribution of distinct values if there is only a fraction of different result values
            size_t distinctValuesCount = getDistinctCount(x);
            bool isDistinctDistribution = distinctValuesCount < x.size() * distinctFraction && distinctValuesCount > 1;

            // Determine required number of ranges
            double min = x[0]->value;
            double max = x[x.size() - 1]->value;

            // Determine minimum distance between two points, if not zero
            double minWidth = std::numeric_limits<double>::max();
            for (size_t i = 0; i < x.size() - 1; i++)
            {
                double diff = x[i + 1]->value - x[i]->value;
                if (diff < minWidth && diff > accuracy)
                {
                    minWidth = diff;
                }
            }

            // Determine additional offset beyond range of physical values
            double extra = isDistinctDistribution ? minWidth / 2 : (max - min) * addFactor;

            if (extra < accuracy)
            {
                if (std::fabs(x[0]->value) < accuracy)
                {
                    // probably x[0] is zero
                    extra = 0.5;
                }
                else
                {
                    extra = std::min(0.5, std::fabs(x[0]->value / 10));
                }
            }

            if (minWidth < std::numeric_limits<double>::max())
            {
                extra = std::max(extra, minWidth / 2);
            }

            // when there are multiple equal values at the extremes, they are regarded as guard limits, which will not be exceeded in the fitted distribution
            bool multipleAtMin = x.size() > 1 && x[0]->value == x[1]->value;
            bool multipleAtMax = x.size() > 1 && x[x.size()-1]->value == x[x.size() - 2]->value;

            int nMultiple = 0;

            if (!multipleAtMin)
            {
                min -= extra;
            }
            else
            {
                nMultiple++;
            }

            if (!multipleAtMax)
            {
                max += extra;
            }
            else
            {
                nMultiple++;
            }

            double rangeReductionFactor = isDistinctDistribution ? 1 : 0.9;

            int requiredRanges = isDistinctDistribution ? std::round((max - min) / minWidth) + 1 - nMultiple : std::round(distinctValuesCount * rangeReductionFactor);
            requiredRanges = std::min(requiredRanges, maxRanges);

            int ranges = requiredRanges;

            // Determine the width of a range
            double step = (max - min) / ranges;

            stochast->HistogramValues.clear();

            for (int i = 0; i < ranges; i++)
            {
                std::shared_ptr<HistogramValue> range = std::make_shared<HistogramValue>(min + i * step, min + (i + 1) * step);

                range->Amount = getAmount(range, x);

                if (range->Amount > 0)
                {
                    stochast->HistogramValues.push_back(range);
                }
            }

            // Loop which determines ranges until enough are found

            bool enoughRanges = stochast->HistogramValues.size() >= requiredRanges / 2;
            int counter = 0;

            while (!enoughRanges)
            {
                constexpr int maxSplitRanges = 100;

                splitRanges(stochast, x);

                enoughRanges = counter > maxSplitRanges || stochast->HistogramValues.size() >= requiredRanges / 2;
                counter++;
            }

            if (multipleAtMin)
            {
                std::shared_ptr<HistogramValue> minRange = std::make_shared<HistogramValue>(min, min);

                minRange->Amount = getAmount(minRange, x);

                if (!stochast->HistogramValues.empty())
                {
                    stochast->HistogramValues[0]->Amount -= minRange->Amount;
                    if (stochast->HistogramValues[0]->Amount == 0)
                    {
                        std::erase(stochast->HistogramValues, stochast->HistogramValues[0]);
                    }

                    stochast->HistogramValues.insert(stochast->HistogramValues.begin(), minRange);
                }
                else
                {
                    stochast->HistogramValues.push_back(minRange);
                }
            }

            if (multipleAtMax && min != max)
            {
                std::shared_ptr<HistogramValue> maxRange = std::make_shared<HistogramValue>(max,max);

                maxRange->Amount = getAmount(maxRange, x);

                stochast->HistogramValues.push_back(maxRange);
            }

            stochast->Observations = static_cast<int>(values.size());

            initializeForRun(stochast);
        }

        void HistogramDistribution::splitRanges(std::shared_ptr<StochastProperties> stochast, std::vector < std::shared_ptr<Numeric::WeightedValue>> & values)
        {
            std::vector<std::shared_ptr<HistogramValue>> existingRanges(stochast->HistogramValues);

            stochast->HistogramValues.clear();

            for (auto range : existingRanges)
            {
                std::shared_ptr<HistogramValue> lowerRange = std::make_shared<HistogramValue>(range->LowerBound, (range->LowerBound + range->UpperBound) / 2);
                std::shared_ptr<HistogramValue> upperRange = std::make_shared<HistogramValue>((range->LowerBound + range->UpperBound) / 2, range->UpperBound);

                lowerRange->Amount = getAmount(lowerRange, values);
                upperRange->Amount = getAmount(upperRange, values);

                if (lowerRange->Amount > 0)
                {
                    stochast->HistogramValues.push_back(lowerRange);
                }

                if (upperRange->Amount > 0)
                {
                    stochast->HistogramValues.push_back(upperRange);
                }
            }
        }

        double HistogramDistribution::getAmount(std::shared_ptr<HistogramValue> range, std::vector<std::shared_ptr<Numeric::WeightedValue>>& values)
        {
            double amount = 0;
            for (size_t j = 0; j < values.size(); j++)
            {
                if (range->contains(values[j]->value))
                {
                    amount += values[j]->weight;
                }
            }

            return amount;
        }

        size_t HistogramDistribution::getDistinctCount(std::vector<std::shared_ptr<Numeric::WeightedValue>>& values)
        {
            size_t count = 1;

            for (size_t i = 1; i < values.size(); i++)
            {
                if (values[i]->value != values[i-1]->value)
                {
                    count++;
                }
            }

            return count;
        }


        void HistogramDistribution::mergeLowWeights(std::vector<std::shared_ptr<Numeric::WeightedValue>>& values)
        {
            constexpr double minWeightNormalized = 0.00001;

            double wSum = 0;
            for (size_t i = 0; i < values.size(); i++)
            {
                wSum += values[i]->weight;
            }

            double weightedMean = 0;
            for (size_t i = 0; i < values.size(); i++)
            {
                weightedMean += values[i]->value * values[i]->weight;
            }

            weightedMean /= wSum;
            double minWeight = minWeightNormalized * wSum;

            bool changed = values.size() > 1;
            while (changed)
            {
                changed = false;

                std::vector<std::shared_ptr<Numeric::WeightedValue>> newWeightedValues;

                for (size_t i = 0; i < values.size() - 1; i++)
                {
                    double combinedWeight = values[i]->weight + values[i + 1]->weight;

                    if (combinedWeight < minWeight)
                    {
                        changed = true;

                        // add the weighted value nearest to the mean
                        if (values[i]->value < weightedMean)
                        {
                            values[i + 1]->weight += values[i]->weight;

                            newWeightedValues.push_back(values[i + 1]);
                        }
                        else
                        {
                            values[i]->weight += values[i + 1]->weight;

                            newWeightedValues.push_back(values[i]);
                        }

                        i++;
                    }
                    else
                    {
                        newWeightedValues.push_back(values[i]);
                        if (i == values.size() - 2)
                        {
                            newWeightedValues.push_back(values[i + 1]);
                        }
                    }
                }

                values.clear();
                for (size_t i = 0; i < newWeightedValues.size(); i++)
                {
                    values.push_back(newWeightedValues[i]);
                }

                changed = changed && values.size() > 1;
            }
        }
    }
}




