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
#include "RayleighNDistribution.h"

#include "../StandardNormal.h"
#include "../StochastProperties.h"
#include "../../Math/NumericSupport.h"
#include "../../Math/RootFinders/BisectionRootFinder.h"
#include "DistributionFitter.h"
#include <cmath>
#include <numbers>

namespace Deltares
{
    namespace Statistics
    {
        void RayleighNDistribution::initialize(std::shared_ptr<StochastProperties> stochast, std::vector<double> values)
        {
            setMeanAndDeviation(stochast, values[0], values[1]);
        }

        bool RayleighNDistribution::isValid(std::shared_ptr<StochastProperties> stochast)
        {
            return stochast->Scale >= 0 && stochast->Shape >= 0;
        }

        bool RayleighNDistribution::isVarying(std::shared_ptr<StochastProperties> stochast)
        {
            return stochast->Scale > 0 && stochast->Shape > 0;
        }

        double RayleighNDistribution::getMean(std::shared_ptr<StochastProperties> stochast)
        {
            return this->getMeanByIteration(stochast);
        }

        double RayleighNDistribution::getDeviation(std::shared_ptr<StochastProperties> stochast)
        {
            return this->getDeviationByIteration(stochast);
        }

        double RayleighNDistribution::getXFromU(std::shared_ptr<StochastProperties> stochast, double u)
        {
            double p; double q;
            StandardNormal::getPQfromU(u, p, q);

            if (stochast->Scale == 0 || stochast->Shape == 0)
            {
                return stochast->Shift;
            }
            else if (p == 0)
            {
                return stochast->Shift;
            }
            else if (q <= 1e-6)
            {
                double qn = std::max(q / stochast->Shape, 1e-300);
                return std::sqrt(-2 * stochast->Scale * stochast->Scale * std::log(qn)) + stochast->Shift;
            }
            else
            {
                double qn = 1 - pow(p, 1 / stochast->Shape);
                return std::sqrt(-2 * stochast->Scale * stochast->Scale * std::log(qn)) + stochast->Shift;
            }
        }

        double RayleighNDistribution::getUFromX(std::shared_ptr<StochastProperties> stochast, double x)
        {
            if (stochast->Scale == 0 || stochast->Shape == 0)
            {
                return x < stochast->Shift ? -StandardNormal::UMax : StandardNormal::UMax;
            }
            else
            {
                double cdf = getCDF(stochast, x);
                return StandardNormal::getUFromP(cdf);
            }
        }

        double RayleighNDistribution::getPDF(std::shared_ptr<StochastProperties> stochast, double x)
        {
            x = x - stochast->Shift;

            if (stochast->Scale == 0 || stochast->Shape == 0)
            {
                return x == 0 ? 1 : 0;
            }
            else if (x <= 0)
            {
                return 0;
            }
            else
            {
                const double rayleighExp = std::exp(-x * x / (2 * stochast->Scale * stochast->Scale));

                return (x * stochast->Shape / (stochast->Scale * stochast->Scale)) * pow(1 - rayleighExp, stochast->Shape - 1) * rayleighExp;
            }
        }

        double RayleighNDistribution::getCDF(std::shared_ptr<StochastProperties> stochast, double x)
        {
            x = x - stochast->Shift;

            if (stochast->Scale == 0 || stochast->Shape == 0)
            {
                return x < 0 ? 0 : 1;
            }
            else if (x <= 0)
            {
                return 0;
            }
            else
            {
                const double rayleighExp = std::exp(-x * x / (2 * stochast->Scale * stochast->Scale));
                return pow(1 - rayleighExp, stochast->Shape);
            }
        }

        void RayleighNDistribution::setMeanAndDeviation(std::shared_ptr<StochastProperties> stochast, double mean, double deviation)
        {
            stochast->Scale = deviation / std::sqrt((4 - std::numbers::pi) / 2);
            stochast->Shift = mean - stochast->Scale * std::sqrt(std::numbers::pi / 2);

            if (stochast->Shape != 1.0) 
            {
                auto bisection = Numeric::BisectionRootFinder(tolBisection);

                Distribution* distribution = this;

                Numeric::RootFinderMethod method = [stochast, distribution](double s)
                {
                    stochast->Scale = s;
                    return distribution->getMean(stochast);
                };

                double minStart = 0.5 * stochast->Scale;
                double maxStart = 1.5 * stochast->Scale;

                stochast->Scale = bisection.CalculateValue(minStart, maxStart, mean, method);
            }
        }

        void RayleighNDistribution::setXAtU(std::shared_ptr<StochastProperties> stochast, double x, double u, ConstantParameterType constantType)
        {
            this->setXAtUByIteration(stochast, x, u, constantType);
        }

        void RayleighNDistribution::fit(std::shared_ptr<StochastProperties> stochast, std::vector<double>& values, double shift)
        {
            // first Rayleigh fit is done

            stochast->Shift = isnan(shift) ? getFittedMinimum(values) : shift;
            stochast->Shape = 1;

            double sum = Numeric::NumericSupport::sum(values, [stochast](double p) {return (p - stochast->Shift) * (p - stochast->Shift); });
            stochast->Scale = std::sqrt(sum / (2 * values.size()));

            std::shared_ptr<DistributionFitter> fitter = std::make_shared<DistributionFitter>();

            std::vector<double> minValues = { 0.5 * stochast->Scale, 0.5 * stochast->Shift, 0.5 };
            std::vector<double> maxValues = { 1.5 * stochast->Scale, 1.5 * stochast->Shift, 1.5 };
            std::vector<double> initValues = { stochast->Scale, stochast->Shift, 1.0 };
            std::vector<DistributionPropertyType> properties = { DistributionPropertyType::Scale, DistributionPropertyType::Shift, DistributionPropertyType::Shape};
            std::vector<double> parameters = fitter->fitByLogLikelihood(values, this, stochast, minValues, maxValues, initValues, properties);

            stochast->Scale = std::max(0.0, parameters[0]);
            stochast->Shift = parameters[1];
            stochast->Shape = std::max(0.0, parameters[2]);
            stochast->Observations = static_cast<int>(values.size());
        }

        std::vector<double> RayleighNDistribution::getSpecialPoints(std::shared_ptr<StochastProperties> stochast)
        {
            std::vector<double> specialPoints{ stochast->Shift };
            return specialPoints;
        }
    }
}



