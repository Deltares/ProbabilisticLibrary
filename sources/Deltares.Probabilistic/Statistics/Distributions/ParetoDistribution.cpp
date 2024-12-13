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
#include "ParetoDistribution.h"
#include "../StandardNormal.h"
#include "../StochastProperties.h"
#include "../../Math/NumericSupport.h"
#include "../../Math/RootFinders/BisectionRootFinder.h"

#include <cmath>
#include <algorithm>

namespace Deltares
{
    namespace Statistics
    {
        void ParetoDistribution::initialize(std::shared_ptr<StochastProperties> stochast, std::vector<double> values)
        {
            this->setMeanAndDeviation(stochast, values[0], values[1]);
        }

        bool ParetoDistribution::isValid(std::shared_ptr<StochastProperties> stochast)
        {
            return stochast->Shape > 0 && stochast->Scale >= 0;
        }

        bool ParetoDistribution::isVarying(std::shared_ptr<StochastProperties> stochast)
        {
            return stochast->Scale > 0;
        }

        double ParetoDistribution::getMean(std::shared_ptr<StochastProperties> stochast)
        {
            return stochast->Shape > 1
                ? stochast->Shape * stochast->Scale / (stochast->Shape - 1)
                : std::numeric_limits<double>::infinity();
        }

        double ParetoDistribution::getDeviation(std::shared_ptr<StochastProperties> stochast)
        {
            if (stochast->Shape > 2)
            {
                return std::sqrt(stochast->Shape * stochast->Scale * stochast->Scale / ((stochast->Shape - 1) * (stochast->Shape - 1) * (stochast->Shape - 2)));
            }
            else
            {
                return std::numeric_limits<double>::infinity();
            }
        }

        void ParetoDistribution::setMeanAndDeviation(std::shared_ptr<StochastProperties> stochast, double mean, double deviation)
        {
            if (deviation <= 0)
            {
                stochast->Scale = 0;
            }
            else
            {
                // solve v = m^2/s^2 = k(k-2) => k^2 - 2k - v = 0
                double vc = mean / deviation;
                if (std::fabs(vc) > 1.0E150)
                {
                    stochast->Scale = 0;
                }
                else
                {
                    double v = vc * vc;

                    // abc-formula
                    double d = 4 + 4 * v;

                    stochast->Shape = (2 + std::sqrt(d)) / 2;
                    stochast->Scale = mean * (stochast->Shape - 1) / stochast->Shape;
                }
            }
        }

        double ParetoDistribution::getPDF(std::shared_ptr<StochastProperties> stochast, double x)
        {
            if (x <= stochast->Scale)
            {
                return 0;
            }
            else
            {
                return stochast->Shape * std::pow(stochast->Scale, stochast->Shape) / std::pow(x, stochast->Shape + 1);
            }
        }

        double ParetoDistribution::getCDF(std::shared_ptr<StochastProperties> stochast, double x)
        {
            if (x <= stochast->Scale)
            {
                return 0.0;
            }
            else if (stochast->Scale <= 0)
            {
                return 1.0;
            }
            else
            {
                return 1 - std::pow(stochast->Scale / x, stochast->Shape);
            }
        }

        double ParetoDistribution::getXFromU(std::shared_ptr<StochastProperties> stochast, double u)
        {
            double p = StandardNormal::getPFromU(u);

            if (p == 0)
            {
                return stochast->Scale;
            }
            else
            {
                double q = 1 - p;
                double scaleX = std::pow(q, 1 / stochast->Shape);

                double x = stochast->Scale / scaleX;
                return x;
            }
        }

        double ParetoDistribution::getUFromX(std::shared_ptr<StochastProperties> stochast, double x)
        {
            if (x <= stochast->Scale)
            {
                return -StandardNormal::UMax;
            }
            else
            {
                double cdf = this->getCDF(stochast, x);
                return StandardNormal::getUFromP(cdf);
            }
        }

        void ParetoDistribution::setXAtU(std::shared_ptr<StochastProperties> stochast, double x, double u, ConstantParameterType constantType)
        {
            setXAtUByIteration(stochast, x, u, constantType);
        }

        void ParetoDistribution::fit(std::shared_ptr<StochastProperties> stochast, std::vector<double>& values)
        {
            stochast->Scale = *std::min_element(values.begin(), values.end());

            const double sum = Numeric::NumericSupport::sum(values, [stochast](double p) { return log(p) - log(stochast->Scale); });

            stochast->Shape = values.size() / sum;
        }

        std::vector<double> ParetoDistribution::getSpecialPoints(std::shared_ptr<StochastProperties> stochast)
        {
            std::vector<double> specialPoints;

            specialPoints.push_back(stochast->Scale);

            return specialPoints;
        }
    }
}





