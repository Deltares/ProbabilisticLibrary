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
#include "GeneralizedParetoDistribution.h"
#include "../StandardNormal.h"
#include "../StochastProperties.h"
#include "../../Math/NumericSupport.h"

#include <cmath>
#include <algorithm>

namespace Deltares
{
    namespace Statistics
    {
        bool GeneralizedParetoDistribution::isValid(std::shared_ptr<StochastProperties> stochast)
        {
            return stochast->Shape > 0 && stochast->Scale >= 0;
        }

        bool GeneralizedParetoDistribution::isVarying(std::shared_ptr<StochastProperties> stochast)
        {
            return stochast->Scale > 0;
        }

        double GeneralizedParetoDistribution::getMean(std::shared_ptr<StochastProperties> stochast)
        {
            return getMeanByIteration(stochast);
        }

        double GeneralizedParetoDistribution::getDeviation(std::shared_ptr<StochastProperties> stochast)
        {
            return getDeviationByIteration(stochast);
        }

        double GeneralizedParetoDistribution::getPDF(std::shared_ptr<StochastProperties> stochast, double x)
        {
            double k = stochast->Shape;
            double s = stochast->Scale;
            double m = stochast->Shift;

            if (x >= m && std::fabs(k) <= epsilon && s > 0)
            {
                return std::exp(-(x - m) / s) / s;
            }
            else if (x >= m && (k * x < s + k * m) && k > epsilon)
            {
                return std::pow(1 - k * (x - m) / s, 1 / k - 1) / s;
            }
            else if (x >= m && (k < epsilon) && s > 0)
            {
                return std::pow(1 - k * (x - m) / s, 1 / k - 1) / s;
            }
            else
            {
                return 0;
            }
        }

        double GeneralizedParetoDistribution::getCDF(std::shared_ptr<StochastProperties> stochast, double x)
        {
            double k = stochast->Shape;
            double s = stochast->Scale;
            double m = stochast->Shift;

            if (x >= m && std::fabs(k) <= epsilon && s > 0)
            {
                return std::exp(-(x - m) / s);
            }
            else if (x >= m && (k * x < s + k * m) && std::fabs(k) > epsilon)
            {
                return 1 - std::pow(1 - k * (x - m) / s, 1 / k);
            }
            else if ((k * x >= s + k * m) && k > epsilon)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }

        double GeneralizedParetoDistribution::getXFromU(std::shared_ptr<StochastProperties> stochast, double u)
        {
            double q = StandardNormal::getQFromU(u);

            double k = stochast->Shape;
            double s = stochast->Scale;
            double m = stochast->Shift;

            if (std::fabs(k) <= epsilon && s > 0)
            {
                return m - s * std::log(q);
            }
            else if (std::fabs(k) > epsilon && s > 0)
            {
                return m + s * (1 - std::pow(q, k)) / k;
            }
            else
            {
                return std::nan("");
            }
        }

        double GeneralizedParetoDistribution::getUFromX(std::shared_ptr<StochastProperties> stochast, double x)
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

        void GeneralizedParetoDistribution::setXAtU(std::shared_ptr<StochastProperties> stochast, double x, double u, ConstantParameterType constantType)
        {
            setXAtUByIteration(stochast, x, u, constantType);
        }

        std::vector<double> GeneralizedParetoDistribution::getSpecialPoints(std::shared_ptr<StochastProperties> stochast)
        {
            std::vector<double> specialPoints;

            specialPoints.push_back(stochast->Scale);

            return specialPoints;
        }
    }
}





