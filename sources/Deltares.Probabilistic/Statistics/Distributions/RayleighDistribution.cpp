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
#include "RayleighDistribution.h"

#include "../StandardNormal.h"
#include "../StochastProperties.h"
#include "../../Math/NumericSupport.h"
#include <cmath>
#include <numbers>

#include "DistributionSupport.h"

namespace Deltares
{
    namespace Statistics
    {
        void RayleighDistribution::initialize(std::shared_ptr<StochastProperties> stochast, std::vector<double> values)
        {
            setMeanAndDeviation(*stochast, values[0], values[1]);
        }

        bool RayleighDistribution::isVarying(std::shared_ptr<StochastProperties> stochast)
        {
            return stochast->Scale > 0;
        }

        double RayleighDistribution::getMean(StochastProperties& stochast)
        {
            return stochast.Shift + stochast.Scale * std::sqrt(std::numbers::pi / 2.0);
        }

        double RayleighDistribution::getDeviation(StochastProperties& stochast)
        {
            return stochast.Scale * std::sqrt((4.0 - std::numbers::pi) / 2.0);
        }

        double RayleighDistribution::getXFromU(StochastProperties& stochast, double u)
        {
            double q = StandardNormal::getQFromU(u);

            if (q == 1.0)
            {
                return stochast.Shift;
            }
            else 
            {
                return std::sqrt(-2.0 * stochast.Scale * stochast.Scale * std::log(q)) + stochast.Shift;
            }
        }

        double RayleighDistribution::getUFromX(StochastProperties& stochast, double x)
        {
            if (stochast.Scale == 0.0)
            {
                return x < stochast.Shift ? -StandardNormal::UMax : StandardNormal::UMax;
            }
            else
            {
                const double cdf = getCDF(stochast, x);
                return StandardNormal::getUFromP(cdf);
            }
        }

        double RayleighDistribution::getPDF(StochastProperties& stochast, double x)
        {
            if (stochast.Scale == 0.0)
            {
                return x == stochast.Shift ? 1.0 : 0.0;
            }
            else if (x < stochast.Shift)
            {
                return 0.0;
            }
            else
            {
                return ((x - stochast.Shift) / (stochast.Scale * stochast.Scale)) * (1.0 - getCDF(stochast, x));
            }
        }

        double RayleighDistribution::getCDF(StochastProperties& stochast, double x)
        {
            if (stochast.Scale == 0.0)
            {
                return x < stochast.Shift ? 0.0 : 1.0;
            }
            else if (x < stochast.Shift)
            {
                return 0.0;
            }
            else
            {
                return 1.0 - std::exp(-(x - stochast.Shift) * (x - stochast.Shift) / (2.0 * stochast.Scale * stochast.Scale));
            }
        }

        void RayleighDistribution::setMeanAndDeviation(StochastProperties& stochast, double mean, double deviation)
        {
            stochast.Scale = deviation / std::sqrt((4.0 - std::numbers::pi) / 2.0);
            stochast.Shift = mean - stochast.Scale * std::sqrt(std::numbers::pi / 2.0);
        }

        void RayleighDistribution::setXAtU(std::shared_ptr<StochastProperties> stochast, double x, double u, ConstantParameterType constantType)
        {
            DistributionSupport::setXAtUByIteration(*this, stochast, x, u, constantType);
        }

        void RayleighDistribution::fit(std::shared_ptr<StochastProperties> stochast, std::vector<double>& values, const double shift)
        {
            stochast->Shift = std::isnan(shift) ? getFittedMinimum(values) : shift;

            double sum = Numeric::NumericSupport::sum(values, [stochast](double p) {return (p - stochast->Shift) * (p - stochast->Shift); });
            stochast->Scale = std::sqrt( sum/ (2 * values.size()));
            stochast->Observations = static_cast<int>(values.size());

            // Unbiased estimator, but cannot handle large sets:
            //stochast.Scale = s * SpecialFunctions.Gamma(x.Length) * Math.Sqrt(x.Length) / SpecialFunctions.Gamma(x.Length + 0.5);
        }

        double RayleighDistribution::getMaxShiftValue(std::vector<double>& values)
        {
            return *std::ranges::min_element(values);
        }

        std::vector<double> RayleighDistribution::getSpecialPoints(std::shared_ptr<StochastProperties> stochast)
        {
            std::vector<double> specialPoints{ stochast->Shift };
            return specialPoints;
        }
    }
}



