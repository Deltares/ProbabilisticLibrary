#include "DiscreteDistribution.h"

#include <algorithm>
#include <cmath>

#include "../StandardNormal.h"
#include "../../Math/NumericSupport.h"


namespace Deltares
{
    namespace Statistics
    {
        void DiscreteDistribution::initializeForRun(std::shared_ptr<StochastProperties> stochast)
        {
            std::sort(stochast->DiscreteValues.begin(), stochast->DiscreteValues.end(), [](std::shared_ptr<DiscreteValue> val1, std::shared_ptr<DiscreteValue> val2) {return val1->X < val2->X; });

            double sum = 0;
            for (std::shared_ptr<DiscreteValue> discreteValue : stochast->DiscreteValues)
            {
                sum += discreteValue->Amount;
            }

            for (std::shared_ptr<DiscreteValue> discreteValue : stochast->DiscreteValues)
            {
                discreteValue->NormalizedAmount = discreteValue->Amount / sum;
            }

            Utils::SetDirtyLambda setDirtyFunction = [stochast]()
            {
                stochast->dirty = true;
            };

            double cumulative = 0;
            for (std::shared_ptr<DiscreteValue> discreteValue : stochast->DiscreteValues)
            {
                cumulative += discreteValue->NormalizedAmount;
                discreteValue->CumulativeNormalizedAmount = cumulative;

                discreteValue->setDirtyFunction(setDirtyFunction);
            }

            stochast->dirty = false;
        }

        bool DiscreteDistribution::isValid(std::shared_ptr<StochastProperties> stochast)
        {
            return !stochast->DiscreteValues.empty();
        }

        bool DiscreteDistribution::isVarying(std::shared_ptr<StochastProperties> stochast)
        {
            return stochast->DiscreteValues.size() > 1;
        }

        double DiscreteDistribution::getXFromU(std::shared_ptr<StochastProperties> stochast, double u)
        {
            const double delta = 0.0000001;

            if (stochast->dirty)
            {
                initializeForRun(stochast);
            }

            if (stochast->DiscreteValues.empty())
            {
                return std::nan("");
            }

            const double p = StandardNormal::getPFromU(u);

            for (size_t i = 0; i < stochast->DiscreteValues.size(); i++)
            {
                if (stochast->DiscreteValues[i]->CumulativeNormalizedAmount >= p - delta)
                {
                    return stochast->DiscreteValues[i]->X;
                }
            }

            return stochast->DiscreteValues.back()->X;
        }

        double DiscreteDistribution::getUFromX(std::shared_ptr<StochastProperties> stochast, double x)
        {
            if (stochast->dirty)
            {
                initializeForRun(stochast);
            }

            if (stochast->DiscreteValues.empty())
            {
                return std::nan("");
            }

            double prev = 0;

            for (size_t i = 0; i < stochast->DiscreteValues.size(); i++)
            {
                double cum = stochast->DiscreteValues[i]->CumulativeNormalizedAmount;

                if (x < stochast->DiscreteValues[i]->X)
                {
                    return StandardNormal::getUFromP(prev);
                }
                else if (x == stochast->DiscreteValues[i]->X)
                {
                    double p = (prev + cum) / 2;
                    return StandardNormal::getUFromP(p);
                }
                else
                {
                    prev = cum;
                }
            }

            return StandardNormal::getUFromP(prev);
        }

        double DiscreteDistribution::getRepresentativeU(std::shared_ptr<StochastProperties> stochast, double u)
        {
            double x = this->getXFromU(stochast, u);
            double uRepresentative = this->getUFromX(stochast, x);

            return uRepresentative;
        }

        double DiscreteDistribution::getMean(std::shared_ptr<StochastProperties> stochast)
        {
            if (stochast->dirty)
            {
                initializeForRun(stochast);
            }

            double sumX = 0;
            double sumAmounts = 0;

            for (std::shared_ptr<DiscreteValue> val : stochast->DiscreteValues)
            {
                sumX += val->X * val->Amount;
                sumAmounts += val->Amount;
            }

            return sumX / sumAmounts;
        }

        double DiscreteDistribution::getDeviation(std::shared_ptr<StochastProperties> stochast)
        {
            double mean = getMean(stochast);

            double sumVariances = 0;
            double sumWeights = 0;

            for (std::shared_ptr<DiscreteValue> discreteValue : stochast->DiscreteValues)
            {
                const double diff = discreteValue->X - mean;

                sumVariances += diff * diff * discreteValue->Amount;
                sumWeights += discreteValue->Amount;
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

        void DiscreteDistribution::setMeanAndDeviation(std::shared_ptr<StochastProperties> stochast, double mean, double deviation)
        {
            if (stochast->dirty)
            {
                initializeForRun(stochast);
            }

            double currentMean = getMean(stochast);
            double diff = mean - currentMean;

            for (std::shared_ptr<DiscreteValue> val : stochast->DiscreteValues)
            {
                val->X += diff;
            }
        }

        double DiscreteDistribution::getPDF(std::shared_ptr<StochastProperties> stochast, double x)
        {
            const double delta = 0.0000001;

            if (stochast->dirty)
            {
                initializeForRun(stochast);
            }

            if (stochast->DiscreteValues.empty())
            {
                return std::nan("");
            }

            for (size_t i = 0; i < stochast->DiscreteValues.size(); i++)
            {
                if (Numeric::NumericSupport::areEqual(stochast->DiscreteValues[i]->X, x, delta))
                {
                    return stochast->DiscreteValues[i]->NormalizedAmount;
                }
            }

            return 0;

        }

        double DiscreteDistribution::getCDF(std::shared_ptr<StochastProperties> stochast, double x)
        {
            const double delta = 0.0000001;

            if (stochast->dirty)
            {
                initializeForRun(stochast);
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

        void DiscreteDistribution::fit(std::shared_ptr<StochastProperties> stochast, std::vector<double>& values)
        {
            std::vector<double> weights = Numeric::NumericSupport::select(values, [](double x) {return 1.0; });
            return fitWeighted(stochast, values, weights);
        }

        void DiscreteDistribution::fitWeighted(std::shared_ptr<StochastProperties> stochast, std::vector<double>& values, std::vector<double>& weights)
        {
            stochast->DiscreteValues.clear();

            std::vector<std::shared_ptr<Numeric::WeightedValue>> x = this->GetWeightedValues(values, weights);

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

            initializeForRun(stochast);
        }

        std::vector<double> DiscreteDistribution::getSpecialPoints(std::shared_ptr<StochastProperties> stochast)
        {
            constexpr double offset = 0.000001;

            if (stochast->dirty)
            {
                initializeForRun(stochast);
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

