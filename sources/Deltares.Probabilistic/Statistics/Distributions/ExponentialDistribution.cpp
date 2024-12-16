// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of Streams.
//
// Streams is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// All names, logos, and references to "Deltares" are registered trademarks of
// Stichting Deltares and remain full property of Stichting Deltares at all times.
// All rights reserved.
//
#include <cmath>
#include "ExponentialDistribution.h"
#include "../StochastProperties.h"
#include "../StandardNormal.h"
#include "../../Math/NumericSupport.h"

#include <numbers>

namespace Deltares
{
    namespace Statistics
    {
        void ExponentialDistribution::setMeanAndDeviation(std::shared_ptr<StochastProperties> stochast, double mean, double deviation)
        {
            stochast->Scale = deviation;
            stochast->Shift = mean - stochast->Scale;
        }

        void ExponentialDistribution::initialize(std::shared_ptr<StochastProperties> stochast, std::vector<double> values)
        {
            stochast->Scale = 1.0 / values[0];
            if (values.size() > 1)
            {
                stochast->Shift = values[1];
            }
        }

        bool ExponentialDistribution::isVarying(std::shared_ptr<StochastProperties> stochast)
        {
            return stochast->Scale > 0;
        }

        bool ExponentialDistribution::isValid(std::shared_ptr<StochastProperties> stochast)
        {
            return stochast->Scale >= 0;
        }

        double ExponentialDistribution::getMean(std::shared_ptr<StochastProperties> stochast)
        {
            return stochast->Scale + stochast->Shift;
        }

        double ExponentialDistribution::getDeviation(std::shared_ptr<StochastProperties> stochast)
        {
            return stochast->Scale;
        }

        double ExponentialDistribution::getXFromU(std::shared_ptr<StochastProperties> stochast, double u)
        {
            double qu = StandardNormal::getQFromU(u);
            return stochast->Shift - log(qu) * stochast->Scale;
        }

        double ExponentialDistribution::getUFromX(std::shared_ptr<StochastProperties> stochast, double x)
        {
            double q = exp((stochast->Shift - x) / stochast->Scale);
            return StandardNormal::getUFromQ(q);
        }

        double ExponentialDistribution::getPDF(std::shared_ptr<StochastProperties> stochast, double x)
        {
            if (x < stochast->Shift)
            {
                return 0;
            }
            else
            {
                return exp(-(x - stochast->Shift) / stochast->Scale) / stochast->Scale;
            }
        }

        double ExponentialDistribution::getCDF(std::shared_ptr<StochastProperties> stochast, double x)
        {
            if (x < stochast->Shift)
            {
                return 0;
            }
            else
            {
                return 1 - exp(-(x - stochast->Shift) / stochast->Scale);
            }
        }

        void ExponentialDistribution::setXAtU(std::shared_ptr<StochastProperties> stochast, double x, double u, ConstantParameterType constantType)
        {
            if (constantType == ConstantParameterType::Deviation)
            {
                double current = getXFromU(stochast, u);
                double diff = x - current;

                stochast->Shift += diff;
            }
            else if (constantType == ConstantParameterType::VariationCoefficient)
            {
                this->setXAtUByIteration(stochast, x, u, constantType);
            }
        }

        void ExponentialDistribution::fit(std::shared_ptr<StochastProperties> stochast, std::vector<double>& values)
        {
            stochast->Shift = this->getFittedMinimum(values);
            stochast->Scale = Numeric::NumericSupport::getMean(values) - stochast->Shift;
        }

        std::vector<double> ExponentialDistribution::getSpecialPoints(std::shared_ptr<StochastProperties> stochast)
        {
            std::vector<double> specialPoints{ stochast->Shift };
            return specialPoints;
        }
    }
}




