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
#include "TriangularDistribution.h"
#include "../StandardNormal.h"
#include "../StochastProperties.h"
#include "../../Math/NumericSupport.h"
#include "../../Math/RootFinders/BisectionRootFinder.h"

#include <cmath>
#include <algorithm>

#include "DistributionSupport.h"

namespace Deltares
{
    namespace Statistics
    {
        void TriangularDistribution::initialize(std::shared_ptr<StochastProperties> stochast, std::vector<double> values)
        {
            stochast->Minimum = values[0];
            stochast->Shift = values[1];
            stochast->Maximum = values[2];
        }

        void TriangularDistribution::validate(Logging::ValidationReport& report, std::shared_ptr<StochastProperties> stochast, std::string& subject)
        {
            Logging::ValidationSupport::checkFinite(report, stochast->Minimum, "minimum", subject);
            Logging::ValidationSupport::checkFinite(report, stochast->Maximum, "maximum", subject);
            Logging::ValidationSupport::checkMinimum(report, stochast->Minimum, stochast->Shift, "shift", subject);
            Logging::ValidationSupport::checkMaximum(report, stochast->Maximum, stochast->Shift, "shift", subject);
        }

        void TriangularDistribution::setMeanAndDeviation(StochastProperties& stochast, double mean, double deviation)
        {
            // set mean value
            if (this->isValid(stochast))
            {
                double currentValue = this->getMean(stochast);
                double diff = mean - currentValue;

                stochast.Minimum += diff;
                stochast.Shift += diff;
                stochast.Maximum += diff;
            }
            else
            {
                stochast.Minimum = mean;
                stochast.Shift = mean;
                stochast.Maximum = mean;
            }

            // set deviation
            if (deviation <= 0)
            {
                stochast.Minimum = mean;
                stochast.Shift = mean;
                stochast.Maximum = mean;
            }
            else
            {
                if (stochast.Minimum == stochast.Maximum)
                {
                    stochast.Minimum = mean - deviation;
                    stochast.Maximum = mean + deviation;
                }

                std::shared_ptr<StochastProperties> copiedStochast = stochast.clone();

                Numeric::RootFinderMethod method = [this, copiedStochast, &stochast](double x)
                {
                    double min = stochast.Shift - stochast.Minimum;
                    double max = stochast.Maximum - stochast.Shift;

                    copiedStochast->Minimum = stochast.Shift - x * min;
                    copiedStochast->Maximum = stochast.Shift + x * max;

                    return this->getDeviation(copiedStochast);
                };

                const double minStart = 0.0;
                const double maxStart = 32.0;
                const double tolerance = 0.00001;
                auto bisection = Numeric::BisectionRootFinder(tolerance);

                double factor = bisection.CalculateValue(minStart, maxStart, deviation, method);

                stochast.Minimum = stochast.Shift - factor * (stochast.Shift - stochast.Minimum);
                stochast.Maximum = stochast.Shift + factor * (stochast.Maximum - stochast.Shift);

                // set mean value again
                double currentValue = getMean(stochast);
                double diff = mean - currentValue;

                stochast.Minimum += diff;
                stochast.Shift += diff;
                stochast.Maximum += diff;

            }
        }

        bool TriangularDistribution::isVarying(std::shared_ptr<StochastProperties> stochast)
        {
            return stochast->Maximum > stochast->Minimum;
        }

        double TriangularDistribution::getMean(StochastProperties& stochast)
        {
            return (stochast.Minimum + stochast.Shift + stochast.Maximum) / 3.0;
        }

        double TriangularDistribution::getDeviation(std::shared_ptr<StochastProperties> stochast)
        {
            double a = stochast->Minimum;
            double b = stochast->Maximum;
            double c = stochast->Shift;

            return sqrt((a * a + b * b + c * c - (a * b + a * c + b * c)) / 18);
        }

        double TriangularDistribution::getPDF(std::shared_ptr<StochastProperties> stochast, double x)
        {
            if (stochast->Minimum == stochast->Maximum)
            {
                return x == stochast->Minimum ? 1 : 0;
            }
            else if (x <= stochast->Minimum || x >= stochast->Maximum)
            {
                return 0;
            }
            else
            {
                double maxHeight = 2 / (stochast->Maximum - stochast->Minimum);
                if (x <= stochast->Minimum || x >= stochast->Maximum)
                {
                    return 0;
                }
                else if (x < stochast->Shift)
                {
                    return maxHeight * (x - stochast->Minimum) / (stochast->Shift - stochast->Minimum);
                }
                else
                {
                    return maxHeight * (1 - (x - stochast->Shift) / (stochast->Maximum - stochast->Shift));
                }
            }
        }

        double TriangularDistribution::getCDF(std::shared_ptr<StochastProperties> stochast, double x)
        {
            if (x < stochast->Minimum)
            {
                return 0;
            }
            else if (x < stochast->Shift)
            {
                return (x - stochast->Minimum) * (x - stochast->Minimum) / ((stochast->Shift - stochast->Minimum) * (stochast->Maximum - stochast->Minimum));
            }
            else if (x < stochast->Maximum)
            {
                double q = (stochast->Maximum - x) * (stochast->Maximum - x) / ((stochast->Maximum - stochast->Shift) * (stochast->Maximum - stochast->Minimum));
                return 1 - q;
            }
            else
            {
                return 1;
            }
        }

        double TriangularDistribution::getXFromU(std::shared_ptr<StochastProperties> stochast, double u)
        {
            double p = StandardNormal::getPFromU(u);

            if (p < (stochast->Shift - stochast->Minimum) / (stochast->Maximum - stochast->Minimum))
            {
                return stochast->Minimum + std::sqrt(p * (stochast->Shift - stochast->Minimum) * (stochast->Maximum - stochast->Minimum));
            }
            else
            {
                double q = 1 - p;
                return stochast->Maximum - std::sqrt(q * (stochast->Maximum - stochast->Shift) * (stochast->Maximum - stochast->Minimum));
            }
        }

        double TriangularDistribution::getUFromX(std::shared_ptr<StochastProperties> stochast, double x)
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

        void TriangularDistribution::setXAtU(std::shared_ptr<StochastProperties> stochast, double x, double u, ConstantParameterType constantType)
        {
            if (constantType == ConstantParameterType::Deviation)
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
                    stochast->Maximum += diff;
                }
            }
            else
            {
                DistributionSupport::setXAtUByIteration(*this, stochast, x, u, constantType);
            }
        }

        void TriangularDistribution::fit(std::shared_ptr<StochastProperties> stochast, std::vector<double>& values, const double shift)
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

            stochast->Observations = static_cast<int>(values.size());
        }

        std::vector<double> TriangularDistribution::getSpecialPoints(std::shared_ptr<StochastProperties> stochast)
        {
            std::vector<double> specialPoints;

            specialPoints.push_back(stochast->Minimum);
            specialPoints.push_back(stochast->Shift);
            specialPoints.push_back(stochast->Maximum);

            return specialPoints;
        }
    }
}





