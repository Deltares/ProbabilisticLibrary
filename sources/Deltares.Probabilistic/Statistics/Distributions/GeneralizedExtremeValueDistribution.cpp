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
#include "GeneralizedExtremeValueDistribution.h"

#include "KSCalculator.h"
#include "../StochastProperties.h"

namespace Deltares
{
    namespace Statistics
    {
        DistributionType GeneralizedExtremeValueDistribution::getExtremeDistributionType(const StochastProperties& stochast)
        {
            if (stochast.Shape == 0.0)
            {
                return DistributionType::Gumbel;
            }
            else if (stochast.Shape > 0.0)
            {
                return DistributionType::Frechet;
            }
            else
            {
                return DistributionType::Weibull;
            }
        }

        std::shared_ptr<Distribution> GeneralizedExtremeValueDistribution::getDistribution(const StochastProperties& stochast) const
        {
            switch (getExtremeDistributionType(stochast))
            {
            case DistributionType::Gumbel: return gumbelDistribution;
            case DistributionType::Frechet: return frechetDistribution;
            case DistributionType::Weibull: return weibullDistribution;
            default: throw Reliability::probLibException("Extreme distribution type not supported");
            }
        }

        StochastProperties GeneralizedExtremeValueDistribution::getStochast(StochastProperties& stochast)
        {
            switch (getExtremeDistributionType(stochast))
            {
            case DistributionType::Gumbel:
            {
                return stochast;
            }
            case DistributionType::Frechet:
            {
                auto frechetStochast = stochast;
                frechetStochast.Shape = 1.0 / stochast.Shape;
                frechetStochast.Scale = stochast.Scale / stochast.Shape;
                frechetStochast.Shift = stochast.Shift - stochast.Scale / stochast.Shape;
                return frechetStochast;
            }
            case DistributionType::Weibull:
            {
                auto weibullStochast = stochast;
                weibullStochast.Shape = -1.0 / stochast.Shape;
                weibullStochast.Scale = -stochast.Scale / stochast.Shape;
                weibullStochast.Shift = stochast.Shift - stochast.Scale / stochast.Shape;
                return weibullStochast;
            }
            default:
                throw Reliability::probLibException("Extreme distribution type not supported");
            }
        }

        void GeneralizedExtremeValueDistribution::initialize(StochastProperties& stochast, const std::vector<double>& values)
        {
            setMeanAndDeviation(stochast, values[0], values[1]);
        }

        void GeneralizedExtremeValueDistribution::validate(Logging::ValidationReport& report, StochastProperties& stochast, std::string& subject)
        {
            auto stochastInstance = getStochast(stochast);
            this->getDistribution(stochast)->validate(report, stochastInstance, subject);
        }

        bool GeneralizedExtremeValueDistribution::isVarying(StochastProperties& stochast)
        {
            auto stochastInstance = getStochast(stochast);
            return this->getDistribution(stochast)->isVarying(stochastInstance);
        }

        double GeneralizedExtremeValueDistribution::getMean(StochastProperties& stochast)
        {
            auto stochastInstance = getStochast(stochast);
            return this->getDistribution(stochast)->getMean(stochastInstance);
        }

        double GeneralizedExtremeValueDistribution::getDeviation(StochastProperties& stochast)
        {
            auto stochastInstance = getStochast(stochast);
            return this->getDistribution(stochast)->getDeviation(stochastInstance);
        }

        double GeneralizedExtremeValueDistribution::getXFromU(StochastProperties& stochast, double u)
        {
            auto stochastInstance = getStochast(stochast);
            return this->getDistribution(stochast)->getXFromU(stochastInstance, u);
        }

        double GeneralizedExtremeValueDistribution::getUFromX(StochastProperties& stochast, double x)
        {
            auto stochastInstance = getStochast(stochast);
            return this->getDistribution(stochast)->getUFromX(stochastInstance, x);
        }

        double GeneralizedExtremeValueDistribution::getPDF(StochastProperties& stochast, double x)
        {
            auto stochastInstance = getStochast(stochast);
            return this->getDistribution(stochast)->getPDF(stochastInstance, x);
        }

        double GeneralizedExtremeValueDistribution::getCDF(StochastProperties& stochast, double x)
        {
            auto stochastInstance = getStochast(stochast);
            return this->getDistribution(stochast)->getCDF(stochastInstance, x);
        }

        void GeneralizedExtremeValueDistribution::setMeanAndDeviation(StochastProperties& stochast, double mean, double deviation)
        {
            auto gevStochast = getStochast(stochast);
            this->getDistribution(stochast)->setMeanAndDeviation(gevStochast, mean, deviation);
            this->assign(gevStochast, stochast);
        }

        void GeneralizedExtremeValueDistribution::setXAtU(StochastProperties& stochast, double x, double u, ConstantParameterType constantType)
        {
            auto gevStochast = getStochast(stochast);
            this->getDistribution(stochast)->setXAtU(gevStochast, x, u, constantType);
            this->assign(gevStochast, stochast);
        }

        void GeneralizedExtremeValueDistribution::fit(StochastProperties& stochast, const std::vector<double>& values, const double shift)
        {
            double bestGoodness = std::numeric_limits<double>::max();
            StochastProperties bestStochast;
            double bestShape = -1;

            for (auto shape : std::vector<double>{ -1, 0, 1 })
            {
                auto dStochast = StochastProperties();
                dStochast.Shape = shape;

                const std::shared_ptr<Distribution> distribution = getDistribution(dStochast);

                distribution->fit(dStochast, values, shift);

                if (!std::isnan(dStochast.Shape) && !std::isnan(dStochast.Scale))
                {
                    double goodness = KSCalculator::getGoodnessOfFit(values, *distribution, dStochast);

                    if (goodness < bestGoodness)
                    {
                        bestGoodness = goodness;
                        bestStochast = dStochast;
                        bestShape = shape;
                    }
                }
            }

            stochast.Shape = bestShape;

            this->assign(bestStochast, stochast);
        }

        double GeneralizedExtremeValueDistribution::getMaxShiftValue(std::vector<double>& values)
        {
            return *std::ranges::min_element(values);
        }

        std::vector<double> GeneralizedExtremeValueDistribution::getSpecialPoints(StochastProperties& stochast)
        {
            auto gevStochast = this->getStochast(stochast);
            return this->getDistribution(stochast)->getSpecialPoints(gevStochast);
        }

        void GeneralizedExtremeValueDistribution::assign(const StochastProperties& source, StochastProperties& target)
        {
            switch (getExtremeDistributionType(target))
            {
            case DistributionType::Gumbel:
                target.Shift = source.Shift;
                target.Shape = source.Shape;
                target.Scale = source.Scale;
                target.Observations = source.Observations;
                break;
            case DistributionType::Frechet:
                target.Shift = source.Shift + source.Scale;
                target.Shape = 1.0 / source.Shape;
                target.Scale = source.Scale / source.Shape;
                target.Observations = source.Observations;
                break;
            case DistributionType::Weibull:
                target.Shift = source.Shift - source.Scale;
                target.Shape = -1.0 / source.Shape;
                target.Scale = source.Scale / source.Shape;
                target.Observations = source.Observations;
                break;
            default: throw Reliability::probLibException("Extreme distribution type not supported");
            }
        }
    }
}



