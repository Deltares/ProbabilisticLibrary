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
#include "TrapezoidalDistribution.h"
#include "../StandardNormal.h"
#include "../StochastProperties.h"
#include "../../Math/NumericSupport.h"
#include "../../Math/RootFinders/BisectionRootFinder.h"

#include <cmath>
#include <algorithm>

#include "DistributionFitter.h"

namespace Deltares
{
    namespace Statistics
    {
        void TrapezoidalDistribution::initialize(std::shared_ptr<StochastProperties> stochast, std::vector<double> values)
        {
            stochast->Minimum = values[0];
            stochast->Shift = values[1];
            stochast->ShiftB = values[2];
            stochast->Maximum = values[3];
        }

        bool TrapezoidalDistribution::isValid(std::shared_ptr<StochastProperties> stochast)
        {
            return stochast->Minimum <= stochast->Shift &&
                stochast->Shift <= stochast->ShiftB &&
                stochast->ShiftB <= stochast->Maximum &&
                std::isfinite(stochast->Minimum) &&
                std::isfinite(stochast->Maximum);
        }

        bool TrapezoidalDistribution::isVarying(std::shared_ptr<StochastProperties> stochast)
        {
            return stochast->Maximum > stochast->Minimum;
        }

        double TrapezoidalDistribution::getExponentDifference(double x, double y, int exp)
        {
            if (x == y)
            {
                constexpr double delta = 0.00001;
                return getExponentDifference(x - delta, x + delta, exp);
            }
            else
            {
                return (std::pow(y, exp) - std::pow(x, exp)) / (y - x);
            }
        }


        double TrapezoidalDistribution::getMean(std::shared_ptr<StochastProperties> stochast)
        {
            double a = stochast->Minimum;
            double b = stochast->Maximum;

            if (a == b)
            {
                return a;
            }
            else
            {
                double c = stochast->Shift;
                double d = stochast->ShiftB;

                double length = (b + d - a - c) / 2;

                return (getExponentDifference(d, b, 3) - getExponentDifference(c, a, 3)) / (6 * length);
            }
        }

        double TrapezoidalDistribution::getDeviation(std::shared_ptr<StochastProperties> stochast)
        {
            double a = stochast->Minimum;
            double b = stochast->Maximum;

            if (a == b)
            {
                return 0;
            }
            else
            {
                double c = stochast->Shift;
                double d = stochast->ShiftB;

                double length = (b + d - a - c) / 2;

                double m = this->getMean(stochast);
                double s2 = (getExponentDifference(d, b, 4) - getExponentDifference(c, a, 4)) / (12 * length);
                return sqrt(s2 - m * m);
            }
        }

        void TrapezoidalDistribution::setMeanAndDeviation(std::shared_ptr<StochastProperties> stochast, double mean, double deviation)
        {
            // set mean value
            if (this->isValid(stochast))
            {
                double currentValue = this->getMean(stochast);
                double diff = mean - currentValue;

                stochast->Minimum += diff;
                stochast->Shift += diff;
                stochast->ShiftB += diff;
                stochast->Maximum += diff;
            }
            else
            {
                stochast->Minimum = mean;
                stochast->Shift = mean;
                stochast->ShiftB = mean;
                stochast->Maximum = mean;
            }

            // set deviation
            if (deviation <= 0)
            {
                stochast->Minimum = mean;
                stochast->Shift = mean;
                stochast->ShiftB = mean;
                stochast->Maximum = mean;
            }
            else
            {
                if (stochast->Minimum == stochast->Maximum)
                {
                    stochast->Minimum = mean - deviation;
                    stochast->Maximum = mean + deviation;
                    stochast->Shift = mean - 0.5 * deviation;
                    stochast->ShiftB = mean + 0.5 * deviation;
                }

                std::shared_ptr<StochastProperties> copiedStochast = stochast->clone();

                Numeric::RootFinderMethod method = [this, copiedStochast, stochast](double x)
                {
                    copiedStochast->Minimum = x * stochast->Minimum;
                    copiedStochast->Shift = x * stochast->Shift;
                    copiedStochast->ShiftB = x * stochast->ShiftB;
                    copiedStochast->Maximum = x * stochast->Maximum;

                    return this->getDeviation(copiedStochast);
                };

                auto bisection = Numeric::BisectionRootFinder(tolBisection);

                double minStart = 0.0;
                double maxStart = 32.0;

                double factor = bisection.CalculateValue(minStart, maxStart, deviation, method);

                stochast->Minimum *= factor;
                stochast->Shift *= factor;
                stochast->ShiftB *= factor;
                stochast->Maximum *= factor;

                // set mean value again
                double currentValue = this->getMean(stochast);
                double diff = mean - currentValue;

                stochast->Minimum += diff;
                stochast->Shift += diff;
                stochast->ShiftB += diff;
                stochast->Maximum += diff;
            }
        }

        double TrapezoidalDistribution::getPDF(std::shared_ptr<StochastProperties> stochast, double x)
        {
            double a = stochast->Minimum;
            double b = stochast->Maximum;
            double c = stochast->Shift;
            double d = stochast->ShiftB;

            double length = (b + d - a - c) / 2;

            if (x < a)
            {
                return 0;
            }
            else if (x < c)
            {
                return (x - a) / (length * (c - a));
            }
            else if (x < d)
            {
                return 1 / length;
            }
            else if (x < b)
            {
                return (b - x) / (length * (b - d));
            }
            else
            {
                return 0;
            }
        }

        double TrapezoidalDistribution::getCDF(std::shared_ptr<StochastProperties> stochast, double x)
        {
            double a = stochast->Minimum;
            double b = stochast->Maximum;
            double c = stochast->Shift;
            double d = stochast->ShiftB;

            double length = (b + d - a - c) / 2;

            if (x < a)
            {
                return 0;
            }
            else if (x < c)
            {
                return (x - a) * (x - a) / (2 * length * (c - a));
            }
            else if (x == c)
            {
                return (c - a) / (2 * length);
            }
            else if (x < d)
            {
                return (2 * x - a - c) / (2 * length);
            }
            else if (x == d)
            {
                return (2 * d - a - c) / (2 * length);
            }
            else if (x < b)
            {
                return 1 - (b - x) * (b - x) / (2 * length * (b - d));
            }
            else
            {
                return 1;
            }
        }

        double TrapezoidalDistribution::getXFromU(std::shared_ptr<StochastProperties> stochast, double u)
        {
            double a = stochast->Minimum;
            double b = stochast->Maximum;
            double c = stochast->Shift;
            double d = stochast->ShiftB;

            double length = (b + d - a - c) / 2;

            double p = StandardNormal::getPFromU(u);

            if (p < (c - a) / (2 * length))
            {
                return a + sqrt(p * 2 * length * (c - a));
            }
            else if (p < (2 * d - a - c) / (2 * length))
            {
                return (p * 2 * length + a + c) / 2;
            }
            else
            {
                double q = 1 - p;
                return b - sqrt(q * 2 * length * (b - d));
            }
        }

        double TrapezoidalDistribution::getUFromX(std::shared_ptr<StochastProperties> stochast, double x)
        {
            if (x <= stochast->Minimum)
            {
                return -StandardNormal::UMax;
            }
            else if (x >= stochast->Maximum)
            {
                return StandardNormal::UMax;
            }
            else
            {
                double cdf = this->getCDF(stochast, x);
                return StandardNormal::getUFromP(cdf);
            }
        }

        void TrapezoidalDistribution::setXAtU(std::shared_ptr<StochastProperties> stochast, double x, double u, ConstantParameterType constantType)
        {
            if (constantType == Deviation)
            {
                if (stochast->Minimum == stochast->Maximum)
                {
                    stochast->Minimum = x;
                    stochast->Shift = x;
                    stochast->Maximum = x;
                }
                else
                {
                    double currentValue = this->getXFromU(stochast, u);
                    double diff = x - currentValue;

                    stochast->Minimum += diff;
                    stochast->Shift += diff;
                    stochast->ShiftB += diff;
                    stochast->Maximum += diff;
                }
            }
            else
            {
                setXAtUByIteration(stochast, x, u, constantType);
            }
        }

        void TrapezoidalDistribution::fit(std::shared_ptr<StochastProperties> stochast, std::vector<double>& values)
        {
            double min = *std::min_element(values.begin(), values.end());
            double max = *std::max_element(values.begin(), values.end());

            double diff = max - min;
            double add = diff / values.size();

            stochast->Minimum = min - add;
            stochast->Maximum = max + add;

            double mean = Numeric::NumericSupport::getMean(values);
            stochast->Shift = 3 * mean - (min + max);

            stochast->Shift = std::min(stochast->Shift, stochast->Maximum);
            stochast->Shift = std::max(stochast->Shift, stochast->Minimum);

            std::shared_ptr<DistributionFitter> fitter = std::make_shared<DistributionFitter>();

            std::vector<double> minValues = { stochast->Minimum, stochast->Minimum };
            std::vector<double> maxValues = { stochast->Maximum, stochast->Maximum };
            std::vector<double> initValues = { stochast->Minimum + 0.3 * diff, stochast->Minimum + 0.7 * diff };
            std::vector<DistributionPropertyType> properties = { Shift, ShiftB };

            std::vector<double> parameters = fitter->fitByLogLikelihood(values,this, stochast,minValues,maxValues,initValues,properties);

            stochast->Shift = std::max(stochast->Minimum, parameters[0]);
            stochast->ShiftB = std::min(stochast->Maximum, parameters[1]);
            stochast->Observations = static_cast<int>(values.size());
        }

        std::vector<double> TrapezoidalDistribution::getSpecialPoints(std::shared_ptr<StochastProperties> stochast)
        {
            std::vector<double> specialPoints;

            specialPoints.push_back(stochast->Minimum);
            specialPoints.push_back(stochast->Shift);
            specialPoints.push_back(stochast->ShiftB);
            specialPoints.push_back(stochast->Maximum);

            return specialPoints;
        }
    }
}





