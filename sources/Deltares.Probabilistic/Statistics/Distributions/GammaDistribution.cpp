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
#include "GammaDistribution.h"
#include "../StandardNormal.h"
#include "../StochastProperties.h"
#include "../../Math/NumericSupport.h"
#include "../../Math/SpecialFunctions.h"

#include <cmath>
#include <algorithm>

#include "DistributionSupport.h"

namespace Deltares
{
    namespace Statistics
    {
        void GammaDistribution::initialize(StochastProperties& stochast, const std::vector<double>& values)
        {
            this->setMeanAndDeviation(stochast, values[0], values[1]);
        }

        bool GammaDistribution::isVarying(StochastProperties& stochast)
        {
            return stochast.Scale > 0.0;
        }

        double GammaDistribution::getMean(StochastProperties& stochast)
        {
            return stochast.Scale * stochast.Shape;
        }

        double GammaDistribution::getDeviation(StochastProperties& stochast)
        {
            return stochast.Scale * sqrt(stochast.Shape);
        }

        void GammaDistribution::setMeanAndDeviation(StochastProperties& stochast, double mean, double deviation)
        {
            stochast.Scale = deviation * deviation / mean;
            stochast.Shape = mean / stochast.Scale;
        }

        double GammaDistribution::getPDF(StochastProperties& stochast, double x)
        {
            if (stochast.Shape <= 0.0 || stochast.Scale <= 0.0)
            {
                return x == 0.0 ? 1.0 : 0.0;
            }

            if (x >= 0.0)
            {
                const double denominator = std::pow(stochast.Scale, stochast.Shape) *
                    Numeric::SpecialFunctions::getGamma(stochast.Shape);
                const double pdf = std::pow(x, stochast.Shape - 1.0) * std::exp(-x / stochast.Scale) / denominator;
                return std::max(0.0, pdf);
            }

            return 0.0;
        }

        double GammaDistribution::getCDF(StochastProperties& stochast, double x)
        {
            if (x <= 0.0)
            {
                return 0.0;
            }
            else if (stochast.Shape <= 0.0 || stochast.Scale <= 0.0)
            {
                return 1.0;
            }
            else
            {
                try
                {
                    return Numeric::SpecialFunctions::getGammaLowerRegularized(stochast.Shape, x / stochast.Scale);
                }
                catch (const std::exception&)
                {
                    return std::nan("");
                }
            }
        }

        double GammaDistribution::getXFromU(StochastProperties& stochast, double u)
        {
            if (!isValid(stochast))
            {
                return 0.0;
            }
            else
            {
                return DistributionSupport::getXFromUByIteration(*this, stochast, u);
            }
        }

        double GammaDistribution::getUFromX(StochastProperties& stochast, double x)
        {
            const double cdf = getCDF(stochast, x);
            return StandardNormal::getUFromP(cdf);
        }

        void GammaDistribution::setXAtU(StochastProperties& stochast, double x, double u, ConstantParameterType constantType)
        {
            DistributionSupport::setXAtUByIteration(*this, stochast, x, u, constantType);
        }

        void GammaDistribution::fit(StochastProperties& stochast, std::vector<double>& values, const double shift)
        {
            double mean = Numeric::NumericSupport::getMean(values);

            std::vector<double> logValues = Numeric::NumericSupport::select(values, [](double p) { return std::log(p); });
            double meanLog = Numeric::NumericSupport::getMean(logValues);

            double s = std::log(mean) - meanLog;

            stochast.Shape = (3 - s + std::sqrt((s - 3) * (s - 3) + 24 * s)) / (12 * s);
            stochast.Scale = mean / stochast.Shape;
            stochast.Observations = static_cast<int>(values.size());
        }
    }
}





