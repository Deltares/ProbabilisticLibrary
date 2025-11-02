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
#include <cmath>
#include "ExponentialDistribution.h"
#include "../StochastProperties.h"
#include "../StandardNormal.h"
#include "../../Math/NumericSupport.h"

#include "DistributionSupport.h"

namespace Deltares
{
    namespace Statistics
    {
        void ExponentialDistribution::setMeanAndDeviation(StochastProperties& stochast, double mean, double deviation)
        {
            stochast.Scale = deviation;
            stochast.Shift = mean - stochast.Scale;
        }

        void ExponentialDistribution::setShift(std::shared_ptr<StochastProperties> stochast, const double shift, bool inverted)
        {
            double oldMean = this->getMean(*stochast);

            if (inverted)
            {
                oldMean = 2 * stochast->Shift - oldMean;
            }

            stochast->Shift = shift;

            if (inverted)
            {
                oldMean = 2 * stochast->Shift - oldMean;
            }

            stochast->Scale = oldMean - shift;
        }

        void ExponentialDistribution::initialize(StochastProperties& stochast, const std::vector<double>& values)
        {
            stochast.Scale = 1.0 / values[0];
            if (values.size() > 1)
            {
                stochast.Shift = values[1];
            }
        }

        bool ExponentialDistribution::isVarying(StochastProperties& stochast)
        {
            return stochast.Scale > 0.0;
        }

        double ExponentialDistribution::getMean(StochastProperties& stochast)
        {
            return stochast.Scale + stochast.Shift;
        }

        double ExponentialDistribution::getDeviation(StochastProperties& stochast)
        {
            return stochast.Scale;
        }

        double ExponentialDistribution::getXFromU(StochastProperties& stochast, double u)
        {
            double qu = StandardNormal::getQFromU(u);
            return stochast.Shift - log(qu) * stochast.Scale;
        }

        double ExponentialDistribution::getUFromX(StochastProperties& stochast, double x)
        {
            const double q = exp((stochast.Shift - x) / stochast.Scale);
            return StandardNormal::getUFromQ(q);
        }

        double ExponentialDistribution::getPDF(StochastProperties& stochast, double x)
        {
            if (x < stochast.Shift)
            {
                return 0.0;
            }
            else
            {
                return exp(-(x - stochast.Shift) / stochast.Scale) / stochast.Scale;
            }
        }

        double ExponentialDistribution::getCDF(StochastProperties& stochast, double x)
        {
            if (x < stochast.Shift)
            {
                return 0.0;
            }
            else
            {
                return 1.0 - exp(-(x - stochast.Shift) / stochast.Scale);
            }
        }

        void ExponentialDistribution::setXAtU(std::shared_ptr<StochastProperties> stochast, double x, double u, ConstantParameterType constantType)
        {
            if (constantType == ConstantParameterType::Deviation)
            {
                double current = getXFromU(*stochast, u);
                double diff = x - current;

                stochast->Shift += diff;
            }
            else if (constantType == ConstantParameterType::VariationCoefficient)
            {
                DistributionSupport::setXAtUByIteration(*this, stochast, x, u, constantType);
            }
        }

        void ExponentialDistribution::fit(std::shared_ptr<StochastProperties> stochast, std::vector<double>& values, const double shift)
        {
            stochast->Shift = std::isnan(shift) ? this->getFittedMinimum(values) : shift;
            stochast->Scale = Numeric::NumericSupport::getMean(values) - stochast->Shift;
            stochast->Observations = static_cast<int>(values.size());
        }

        double ExponentialDistribution::getMaxShiftValue(std::vector<double>& values)
        {
            return Numeric::NumericSupport::getMean(values);
        }

        std::vector<double> ExponentialDistribution::getSpecialPoints(StochastProperties& stochast)
        {
            std::vector<double> specialPoints{ stochast.Shift };
            return specialPoints;
        }
    }
}




