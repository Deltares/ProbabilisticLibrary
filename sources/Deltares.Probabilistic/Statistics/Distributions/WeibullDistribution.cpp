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
#include "WeibullDistribution.h"

#include "../StandardNormal.h"
#include "../StochastProperties.h"
#include "../../Math/NumericSupport.h"
#include "../../Math/SpecialFunctions.h"
#include "../../Math/RootFinders/BisectionRootFinder.h"
#include <cmath>
#include <numbers>

namespace Deltares
{
    namespace Statistics
    {
        void WeibullDistribution::initialize(std::shared_ptr<StochastProperties> stochast, std::vector<double> values)
        {
            setMeanAndDeviation(stochast, values[0], values[1]);
        }

        bool WeibullDistribution::isValid(std::shared_ptr<StochastProperties> stochast)
        {
            return stochast->Scale >= 0 && stochast->Shape >= 0;
        }

        bool WeibullDistribution::isVarying(std::shared_ptr<StochastProperties> stochast)
        {
            return stochast->Scale > 0;
        }

        double WeibullDistribution::getMean(std::shared_ptr<StochastProperties> stochast)
        {
            double gamma1km = Numeric::SpecialFunctions::getGamma(1 + 1 / stochast->Shape);
            return stochast->Scale * gamma1km + stochast->Shift;
        }

        double WeibullDistribution::getDeviation(std::shared_ptr<StochastProperties> stochast)
        {
            double gamma1k = Numeric::SpecialFunctions::getGamma(1 + 1 / stochast->Shape);
            double gamma2k = Numeric::SpecialFunctions::getGamma(1 + 2 / stochast->Shape);
            return stochast->Scale * std::sqrt(gamma2k - gamma1k * gamma1k);
        }

        double WeibullDistribution::getXFromU(std::shared_ptr<StochastProperties> stochast, double u)
        {
            double q = StandardNormal::getQFromU(u);

            if (q == 1)
            {
                // handle special case for exceeding probability of 1
                // note that q can be exactly 1 due to numerical limitations in getQFromU
                return stochast->Shift;
            }
            else
            {
                const double log = std::log(q);
                const double xScale = std::pow(-log, 1 / stochast->Shape);
                const double x = xScale * stochast->Scale;
                return x + stochast->Shift;
            }
        }

        double WeibullDistribution::getUFromX(std::shared_ptr<StochastProperties> stochast, double x)
        {
            if (stochast->Scale == 0)
            {
                return x < stochast->Shift ? -StandardNormal::UMax : StandardNormal::UMax;
            }
            else
            {
                double cdf = getCDF(stochast, x);
                return StandardNormal::getUFromP(cdf);
            }
        }

        double WeibullDistribution::getPDF(std::shared_ptr<StochastProperties> stochast, double x)
        {
            if (x <= stochast->Shift)
            {
                return 0;
            }
            else if (stochast->Scale == 0)
            {
                return x == stochast->Shift ? 1 : 0;
            }
            else
            {
                return (stochast->Shape / stochast->Scale) * std::pow((x - stochast->Shift) / stochast->Scale, stochast->Shape - 1) * (1 - getCDF(stochast, x));
            }
        }

        double WeibullDistribution::getCDF(std::shared_ptr<StochastProperties> stochast, double x)
        {
            if (x <= stochast->Shift)
            {
                return 0;
            }
            else if (stochast->Scale == 0)
            {
                return x < stochast->Shape ? 0 : 1;
            }
            else
            {
                return 1 - std::exp(-std::pow((x - stochast->Shift) / stochast->Scale, stochast->Shape));
            }
        }

        void WeibullDistribution::setMeanAndDeviation(std::shared_ptr<StochastProperties> stochast, double mean, double deviation)
        {
            mean = mean - stochast->Shift;

            // the quotient deviation / mean is only dependent from the shape parameter, so this will be determined first
            double u = deviation / mean;

            // see https://www.researchgate.net/post/Can_anybody_explain_how_to_find_out_the_shape_and_scale_parameters_for_weibull_statistics_for_average_wind_speed_data_for_a_month
            double kGuess = 1.2785 / u - 0.5004;

            auto bisection = Numeric::BisectionRootFinder(tolBisection);

            Numeric::RootFinderMethod method = [](double k)
            {
                double gamma1k = Numeric::SpecialFunctions::getGamma(1 + 1 / k);
                double gamma2k = Numeric::SpecialFunctions::getGamma(1 + 2 / k);

                return std::sqrt(gamma2k - gamma1k * gamma1k) / gamma1k;
            };

            double minStart = std::max(0.01, kGuess - 0.1);
            double maxStart = kGuess + 0.1;

            stochast->Shape = bisection.CalculateValue(minStart, maxStart, u, method);
            stochast->Scale = mean / Numeric::SpecialFunctions::getGamma(1 + 1 / stochast->Shape);
        }

        void WeibullDistribution::setXAtU(std::shared_ptr<StochastProperties> stochast, double x, double u, ConstantParameterType constantType)
        {
            if (constantType == ConstantParameterType::Deviation)
            {
                constexpr double delta = 0.00001;
                double margin = std::min(delta, std::fabs(x / 1000000));

                auto bisection = Numeric::BisectionRootFinder(margin);

                double deviation = this->getDeviation(stochast);
                Distribution* distribution = this;

                Numeric::RootFinderMethod method = [distribution, stochast, deviation, u](double mean)
                {
                    distribution->setMeanAndDeviation(stochast, mean, deviation);
                    return distribution->getXFromU(stochast, u);
                };

                double mean = bisection.CalculateValue(x, this->getMean(stochast), x, method);

                this->setMeanAndDeviation(stochast, mean, deviation);
            }
            else if (constantType == ConstantParameterType::VariationCoefficient)
            {
                this->setXAtUByIteration(stochast, x, u, constantType);
            }
        }

        void WeibullDistribution::fit(std::shared_ptr<StochastProperties> stochast, std::vector<double>& values)
        {
            stochast->Shift = this->getFittedMinimum(values);

            std::vector<double> shiftedValues = Numeric::NumericSupport::select(values, [stochast](double p) { return p - stochast->Shift; });

            constexpr double tolerance = 0.001;
            auto bisection = Numeric::BisectionRootFinder(tolerance);

            Numeric::RootFinderMethod method = [&shiftedValues](double x)
            {
                double sumLog = Numeric::NumericSupport::sum(shiftedValues, [](double p) { return log(p); });
                double sumPow = Numeric::NumericSupport::sum(shiftedValues, [x](double p) { return pow(p, x); });
                double sumLogPow = Numeric::NumericSupport::sum(shiftedValues, [x](double p) { return pow(p, x) * log(p); });

                return 1 / x + sumLog / shiftedValues.size() - sumLogPow / sumPow;
            };

            double minStart = Numeric::NumericSupport::getMinValidValue(method);
            double maxStart = Numeric::NumericSupport::getMaxValidValue(method);

            stochast->Shape = bisection.CalculateValue(minStart, maxStart, 0, method);

            double sum = Numeric::NumericSupport::sum(shiftedValues, [stochast](double p) {return std::pow(p, stochast->Shape); });

            stochast->Scale = std::pow(sum / shiftedValues.size(), 1 / stochast->Shape);

            stochast->Observations = static_cast<int>(values.size());
        }

        std::vector<double> WeibullDistribution::getSpecialPoints(std::shared_ptr<StochastProperties> stochast)
        {
            std::vector<double> specialPoints{ stochast->Shift };
            return specialPoints;
        }
    }
}



