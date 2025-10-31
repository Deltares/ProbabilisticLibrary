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
#include "BetaDistribution.h"
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
        void BetaDistribution::initialize(std::shared_ptr<StochastProperties> stochast, std::vector<double> values)
        {
            this->setMeanAndDeviation(*stochast, values[0], values[1]);
        }

        bool BetaDistribution::isVarying(std::shared_ptr<StochastProperties> stochast)
        {
            return stochast->Shape > 0 && stochast->ShapeB > 0;
        }

        double BetaDistribution::getMean(StochastProperties& stochast)
        {
            return stochast.Shape / (stochast.Shape + stochast.ShapeB);
        }

        double BetaDistribution::getDeviation(StochastProperties& stochast)
        {
            const double sum = stochast.Shape + stochast.ShapeB;
            const double variance = stochast.Shape * stochast.ShapeB / (sum * sum * (sum + 1.0));
            return sqrt(variance);
        }

        void BetaDistribution::setMeanAndDeviation(StochastProperties& stochast, double mean, double deviation)
        {
            // see https://stats.stackexchange.com/questions/12232/calculating-the-parameters-of-a-beta-distribution-using-the-mean-and-variance

            double sigma = deviation;
            double variance = sigma * sigma;

            stochast.Shape = ((1 - mean) / variance - 1 / mean) * mean * mean;
            stochast.ShapeB = stochast.Shape * (1 / mean - 1);
        }

        double BetaDistribution::getPDF(StochastProperties& stochast, double x)
        {
            if (!isValid(stochast))
            {
                return x == 0.0 ? 1.0 : 0.0;
            }
            else if (x < 0.0 || x > 1.0)
            {
                return 0.0;
            }
            else
            {
                return std::pow(x, stochast.Shape - 1.0) * std::pow(1.0 - x, stochast.ShapeB - 1.0)
                / Numeric::SpecialFunctions::getBeta (stochast.Shape, stochast.ShapeB);
            }
        }

        double BetaDistribution::getCDF(std::shared_ptr<StochastProperties> stochast, double x)
        {
            if (!isValid(stochast))
            {
                return x <= 0 ? 0 : 1;
            }
            else if (x <= 0)
            {
                return 0;
            }
            else if (x >= 1)
            {
                return 1;
            }
            else
            {
                try
                {
                    double cdf = Numeric::SpecialFunctions::getBetaIncomplete(stochast->Shape, stochast->ShapeB, x);
                    return cdf;
                }
                catch (const std::exception&)
                {
                    return x <= 0 ? 0 : 1;
                }
            }
        }

        double BetaDistribution::getXFromU(StochastProperties& stochast, double u)
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

        double BetaDistribution::getUFromX(StochastProperties& stochast, double x)
        {
            if (x <= stochast.Scale)
            {
                return -StandardNormal::UMax;
            }
            else
            {
                const double cdf = getCDF(stochast.clone(), x); // TODO
                return StandardNormal::getUFromP(cdf);
            }
        }

        void BetaDistribution::setXAtU(std::shared_ptr<StochastProperties> stochast, double x, double u, ConstantParameterType constantType)
        {
            DistributionSupport::setXAtUByIteration(*this, stochast, x, u, constantType);
        }

        void BetaDistribution::fit(std::shared_ptr<StochastProperties> stochast, std::vector<double>& values, const double shift)
        {
            double mean = Numeric::NumericSupport::getMean(values);
            double sigma = Numeric::NumericSupport::getStandardDeviation(mean, values);

            setMeanAndDeviation(*stochast, mean, sigma);

            stochast->Observations = static_cast<int>(values.size());
        }

        std::vector<double> BetaDistribution::getSpecialPoints(std::shared_ptr<StochastProperties> stochast)
        {
            std::vector<double> specialPoints;

            specialPoints.push_back(0.0);
            specialPoints.push_back(1.0);

            return specialPoints;
        }
    }
}





