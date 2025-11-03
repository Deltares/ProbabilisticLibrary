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
#include "TruncatedDistribution.h"

#include <cmath>

#include "DistributionFitter.h"
#include "../DistributionPropertyType.h"
#include <algorithm>
#include "../StandardNormal.h"
#include "../../Math/NumericSupport.h"

namespace Deltares
{
    namespace Statistics
    {
        struct Truncated
        {
            double LowerProbability = 0;
            double UpperProbability = 1;
            double Factor = 1;
        };

        Truncated TruncatedDistribution::getTruncatedValue(StochastProperties& stochast) const
        {
            // detect exceeding probability of limits if it were a normal standard distribution
            const double p = getProbability(stochast, true);
            const double q = getProbability(stochast, false);

            // the shape of the distribution must be multiplied with a factor, so that the truncated shape has a total area of 1
            const double factor = 1.0 / (1.0 - p - q);

            Truncated truncated;
            truncated.LowerProbability = p;
            truncated.UpperProbability = q;
            truncated.Factor = factor;

            return truncated;
        }

        double TruncatedDistribution::getProbability(StochastProperties& stochast, bool isMinimum) const
        {
            double value = isMinimum ? stochast.Minimum : stochast.Maximum;

            if (std::isnan(value) || std::isinf(value))
            {
                return 0;
            }
            else
            {
                double u = this->innerDistribution->getUFromX(stochast.clone(), value); // TODO
                if (std::isnan(u))
                {
                    return 0;
                }
                else
                {
                    return isMinimum ? StandardNormal::getPFromU(u) : StandardNormal::getQFromU(u);
                }
            }
        }

        double TruncatedDistribution::getUntruncatedU(double u, StochastProperties& stochast)
        {
            double p = StandardNormal::getPFromU(u);
            double q = StandardNormal::getQFromU(u);

            Truncated truncated = getTruncatedValue(stochast);

            if (q < 0.5)
            {
                // zt is the u-value in the truncated distribution, which has the same exceeding probability as u in the standard normal distribution
                // the area in the truncated distribution is: factor * (qzt - qb) = qz
                double pTruncatedExceeding = q / truncated.Factor + truncated.UpperProbability;
                return StandardNormal::getUFromQ(pTruncatedExceeding);
            }
            else
            {
                // zt is the u-value in the truncated distribution, which has the same exceeding probability as u in the standard normal distribution
                // the area in the truncated distribution is: factor * (qzt - pa) = pz
                double pTruncatedNonExceeding = p / truncated.Factor + truncated.LowerProbability;
                return StandardNormal::getUFromP(pTruncatedNonExceeding);
            }
        }

        bool TruncatedDistribution::isVarying(StochastProperties& stochast)
        {
            return stochast.Minimum != stochast.Maximum && innerDistribution->isVarying(stochast);
        }

        void TruncatedDistribution::validate(Logging::ValidationReport& report, StochastProperties& stochast, std::string& subject)
        {
            Logging::ValidationSupport::checkMinimum(report, stochast.Minimum, stochast.Maximum, "maximum", subject);
            innerDistribution->validate(report, stochast, subject);
        }

        double TruncatedDistribution::getMean(StochastProperties& stochast)
        {
            return innerDistribution->getMean(stochast);
        }

        double TruncatedDistribution::getDeviation(StochastProperties& stochast)
        {
            return innerDistribution->getDeviation(stochast);
        }

        void TruncatedDistribution::setMeanAndDeviation(StochastProperties& stochast, double mean, double deviation)
        {
            innerDistribution->setMeanAndDeviation(stochast, mean, deviation);
        }

        void TruncatedDistribution::setShift(StochastProperties& stochast, const double shift, bool inverted)
        {
            innerDistribution->setShift(stochast, shift, inverted);
        }

        double TruncatedDistribution::getXFromU(StochastProperties& stochast, double u)
        {
            if (stochast.Minimum == stochast.Maximum)
            {
                return stochast.Minimum;
            }
            else
            {
                double x = this->innerDistribution->getXFromU(stochast, getUntruncatedU(u, stochast));

                if (!std::isinf(stochast.Maximum) && !std::isnan(stochast.Maximum) && x > stochast.Maximum)
                {
                    x = stochast.Maximum;
                }
                else if (!std::isinf(stochast.Minimum) && !std::isnan(stochast.Minimum) && x < stochast.Minimum)
                {
                    x = stochast.Minimum;
                }

                return x;
            }
        }

        double TruncatedDistribution::getUFromX(StochastProperties& stochast, double x)
        {
            if (stochast.Minimum == stochast.Maximum)
            {
                return 0;
            }
            else if (x <= stochast.Minimum)
            {
                return -StandardNormal::UMax;
            }
            else if (x >= stochast.Maximum)
            {
                return StandardNormal::UMax;
            }
            else
            {
                Truncated truncated = getTruncatedValue(stochast);

                double u = innerDistribution->getUFromX(stochast, x);
                double q = StandardNormal::getQFromU(u);

                double qTruncated = (q - truncated.UpperProbability) * truncated.Factor;

                return StandardNormal::getUFromQ(qTruncated);
            }
        }

        double TruncatedDistribution::getPDF(StochastProperties& stochast, double x)
        {
            if (stochast.Minimum == stochast.Maximum)
            {
                return x == stochast.Minimum ? 1 : 0;
            }
            else if (x < stochast.Minimum || x > stochast.Maximum)
            {
                return 0;
            }

            Truncated truncated = getTruncatedValue(stochast);
            return truncated.Factor * innerDistribution->getPDF(stochast, x);
        }

        double TruncatedDistribution::getCDF(StochastProperties& stochast, double x)
        {
            if (x < stochast.Minimum)
            {
                return 0.0;
            }
            else if (x > stochast.Maximum)
            {
                return 1.0;
            }

            double u = getUFromX(stochast, x);

            return StandardNormal::getPFromU(u);
        }

        void TruncatedDistribution::setXAtU(StochastProperties& stochast, double x, double u, ConstantParameterType constantType)
        {
            this->innerDistribution->setXAtU(stochast, x, getUntruncatedU(u, stochast), constantType);
        }

        void TruncatedDistribution::fit(StochastProperties& stochast, const std::vector<double>& values, const double shift)
        {
            // perform the fit without truncation
            innerDistribution->fit(stochast, values, shift);
            fitMinMax(stochast, values);
        }

        void TruncatedDistribution::fitPrior(StochastProperties& stochast, const std::vector<double>& values, StochastProperties& prior, const double shift)
        {
            // perform the fit without truncation
            innerDistribution->fitPrior(stochast, values, prior, shift);
            fitMinMax(stochast, values);
        }

        double TruncatedDistribution::getMaxShiftValue(std::vector<double>& values)
        {
            return innerDistribution->getMaxShiftValue(values);
        }

        void TruncatedDistribution::fitMinMax(StochastProperties& stochast, const std::vector<double>& values)
        {
            // sets minimum and maximum
            double min = *std::min_element(values.begin(), values.end());
            double max = *std::max_element(values.begin(), values.end());

            double diff = max - min;
            double add = diff / values.size();

            stochast.Minimum = min - add;
            stochast.Maximum = max + add;

            std::vector<DistributionPropertyType> properties = getParameters();

            std::vector<double> minValues;
            std::vector<double> maxValues;
            std::vector<double> initValues;
            std::vector<DistributionPropertyType> fitProperties;

            for (DistributionPropertyType property : properties)
            {
                switch (property)
                {
                case DistributionPropertyType::Location:
                {
                    minValues.push_back(stochast.Location - 3.0 * stochast.Scale);
                    maxValues.push_back(stochast.Location + 3.0 * stochast.Scale);
                    initValues.push_back(stochast.Location);
                    fitProperties.push_back(property);
                    break;
                }
                case DistributionPropertyType::Scale:
                {
                    minValues.push_back(0.1 * stochast.Scale);
                    maxValues.push_back(1.9 * stochast.Scale);
                    initValues.push_back(stochast.Scale);
                    fitProperties.push_back(property);
                    break;
                }
                case DistributionPropertyType::Shift:
                {
                    minValues.push_back(stochast.Shift - 3.0 * stochast.Scale);
                    maxValues.push_back(stochast.Shift + 3.0 * stochast.Scale);
                    initValues.push_back(stochast.Shift);
                    fitProperties.push_back(property);
                    break;
                }
                case DistributionPropertyType::Shape:
                {
                    minValues.push_back(0.1 * stochast.Shape);
                    maxValues.push_back(1.9 * stochast.Shape);
                    initValues.push_back(stochast.Shape);
                    fitProperties.push_back(property);
                    break;
                }
                }
            }

            std::shared_ptr<DistributionFitter> distributionFitter = std::make_shared<DistributionFitter>();

            std::vector<double> parameters = distributionFitter->fitByLogLikelihood(values, this, stochast, minValues, maxValues, initValues, fitProperties);

            for (size_t i = 0; i < fitProperties.size(); i++)
            {
                double value = parameters[i];

                const bool allowNegative = fitProperties[i] != DistributionPropertyType::Scale;
                if (!allowNegative)
                {
                    value = std::max(0.0, value);
                }

                stochast.applyValue(fitProperties[i], value);
            }
        }

        std::vector<DistributionPropertyType> TruncatedDistribution::getParameters()
        {
            std::vector<DistributionPropertyType> parameters = innerDistribution->getParameters();
            parameters.push_back(DistributionPropertyType::Minimum);
            parameters.push_back(DistributionPropertyType::Maximum);

            return parameters;
        }

        double TruncatedDistribution::getLogLikelihood(StochastProperties& stochast, double x)
        {
            Truncated truncated = getTruncatedValue(stochast);

            if (truncated.Factor == 0 || std::isinf(truncated.Factor) || std::isnan(truncated.Factor))
            {
                return nan("");
            }
            else
            {
                return log(truncated.Factor) + this->innerDistribution->getLogLikelihood(stochast, x);
            }
        }

        std::vector<double> TruncatedDistribution::getSpecialPoints(StochastProperties& stochast)
        {
            std::vector<double> specialPoints;

            const bool hasMinimum = !std::isnan(stochast.Minimum) && !std::isinf(stochast.Minimum);
            const bool hasMaximum = !std::isnan(stochast.Maximum) && !std::isinf(stochast.Maximum);

            if (hasMinimum)
            {
                specialPoints.push_back(stochast.Minimum + Numeric::NumericSupport::getFraction(stochast.Minimum, -0.1));
                specialPoints.push_back(stochast.Minimum + Numeric::NumericSupport::getFraction(stochast.Minimum, -0.000001));
                specialPoints.push_back(stochast.Minimum);
            }

            std::vector<double> innerPoints = this->innerDistribution->getSpecialPoints(stochast);

            for (double x : innerPoints)
            {
                if ((!hasMinimum || x > stochast.Minimum) && (!hasMaximum || x < stochast.Maximum))
                {
                    specialPoints.push_back(x);
                }
            }

            if (hasMaximum)
            {
                specialPoints.push_back(stochast.Maximum);
                specialPoints.push_back(stochast.Maximum + Numeric::NumericSupport::getFraction(stochast.Maximum, 0.000001));
                specialPoints.push_back(stochast.Maximum + Numeric::NumericSupport::getFraction(stochast.Maximum, 0.1));
            }

            return specialPoints;
        }
    }
}
