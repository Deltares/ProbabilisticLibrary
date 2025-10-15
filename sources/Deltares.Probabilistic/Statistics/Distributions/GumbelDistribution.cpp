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
#include "GumbelDistribution.h"

#include "../StandardNormal.h"
#include "../StochastProperties.h"
#include "../../Math/NumericSupport.h"
#include "../../Math/RootFinders/BisectionRootFinder.h"
#include <cmath>
#include <numbers>

#include "DistributionSupport.h"

namespace Deltares
{
    namespace Statistics
    {
        void GumbelDistribution::setMeanAndDeviation(std::shared_ptr<StochastProperties> stochast, double mean, double deviation)
        {
            stochast->Scale = sqrt(6) * deviation / std::numbers::pi;
            stochast->Shift = mean - stochast->Scale * std::numbers::egamma;
        }

        void GumbelDistribution::initialize(std::shared_ptr<StochastProperties> stochast, std::vector<double> values)
        {
            setMeanAndDeviation(stochast, values[0], values[1]);
        }

        bool GumbelDistribution::isVarying(std::shared_ptr<StochastProperties> stochast)
        {
            return stochast->Scale > 0;
        }

        double GumbelDistribution::getMean(std::shared_ptr<StochastProperties> stochast)
        {
            return stochast->Shift + stochast->Scale * std::numbers::egamma;
        }

        double GumbelDistribution::getDeviation(std::shared_ptr<StochastProperties> stochast)
        {
            return std::numbers::pi * stochast->Scale / sqrt(6);
        }

        double GumbelDistribution::getXFromU(std::shared_ptr<StochastProperties> stochast, double u)
        {
            double p = StandardNormal::getPFromU(u);

            if (p == 0)
            {
                return stochast->Shift;
            }
            else
            {
                double logp = -log(p);
                double xscale = -log(logp);
                double x = xscale * stochast->Scale;

                return x + stochast->Shift;
            }
        }

        double GumbelDistribution::getUFromX(std::shared_ptr<StochastProperties> stochast, double x)
        {
            if (stochast->Scale == 0)
            {
                return x < stochast->Shift ? -StandardNormal::UMax : StandardNormal::UMax;
            }
            else
            {
                x -= stochast->Shift;
                double cdf = exp(-exp(-x / stochast->Scale));

                return StandardNormal::getUFromP(cdf);
            }
        }

        double GumbelDistribution::getPDF(std::shared_ptr<StochastProperties> stochast, double x)
        {
            if (stochast->Scale == 0)
            {
                return x == stochast->Shift ? 1 : 0;
            }
            else
            {
                x = (x - stochast->Shift) / stochast->Scale;

                return (1 / stochast->Scale) * exp(-(x + exp(-x)));
            }
        }

        double GumbelDistribution::getCDF(std::shared_ptr<StochastProperties> stochast, double x)
        {
            if (stochast->Scale == 0)
            {
                return x < stochast->Shift ? 0 : 1;
            }
            else
            {
                x -= stochast->Shift;
                return exp(-exp(-x / stochast->Scale));
            }
        }

        void GumbelDistribution::setXAtU(std::shared_ptr<StochastProperties> stochast, double x, double u, ConstantParameterType constantType)
        {
            if (constantType == ConstantParameterType::Deviation)
            {
                double current = this->getXFromU(stochast, u);
                double diff = x - current;

                stochast->Shift += diff;
            }
            else if (constantType == ConstantParameterType::VariationCoefficient)
            {
                DistributionSupport::setXAtUByIteration(*this, stochast, x, u, constantType);
            }
        }

        void GumbelDistribution::fit(std::shared_ptr<StochastProperties> stochast, std::vector<double>& values, const double shift)
        {
            // https://stats.stackexchange.com/questions/71197/usable-estimators-for-parameters-in-gumbel-distribution
            double mean = Numeric::NumericSupport::getMean(values);

            auto bisection = Numeric::BisectionRootFinder(0.001);

            Numeric::RootFinderMethod method = [mean, &values](double x)
            {
                double counter = Numeric::NumericSupport::sum(values, [x](double p) {return p * exp(-p / x); });
                double denominator = Numeric::NumericSupport::sum(values, [x](double p) {return exp(-p / x); });
                return mean - x - counter / denominator;
            };

            double minStart = Numeric::NumericSupport::getMinValidValue(method);
            double maxStart = Numeric::NumericSupport::getMaxValidValue(method);

            stochast->Scale = bisection.CalculateValue(minStart, maxStart, 0, method);

            double sum = Numeric::NumericSupport::sum(values, [stochast](double p) {return exp(-p / stochast->Scale); });

            stochast->Shift = -stochast->Scale * log(sum / values.size());
            stochast->Observations = static_cast<int>(values.size());
        }

        double GumbelDistribution::getLogLikelihood(std::shared_ptr<StochastProperties> stochast, double x)
        {
            x = (x - stochast->Shift) / stochast->Scale;

            return -(log(stochast->Scale) + (x + exp(-x)));
        }

        std::vector<double> GumbelDistribution::getSpecialPoints(std::shared_ptr<StochastProperties> stochast)
        {
            std::vector<double> specialPoints{ stochast->Shift };
            return specialPoints;
        }
    }
}



