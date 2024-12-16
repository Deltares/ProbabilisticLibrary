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
#include "GeneralizedExtremeValueDistribution.h"

#include "KSCalculator.h"
#include "../StochastProperties.h"

namespace Deltares
{
    namespace Statistics
    {
        DistributionType GeneralizedExtremeValueDistribution::getExtremeDistributionType(std::shared_ptr<StochastProperties> stochast)
        {
            if (stochast->Shape == 0)
            {
                return DistributionType::Gumbel;
            }
            else if (stochast->Shape > 0)
            {
                return DistributionType::Frechet;
            }
            else
            {
                return DistributionType::Weibull;
            }
        }

        std::shared_ptr<Distribution> GeneralizedExtremeValueDistribution::getDistribution(std::shared_ptr<StochastProperties> stochast)
        {
            switch (this->getExtremeDistributionType(stochast))
            {
            case DistributionType::Gumbel: return gumbelDistribution;
            case DistributionType::Frechet: return frechetDistribution;
            case DistributionType::Weibull: return weibullDistribution;
            default: throw Reliability::probLibException("Extreme distribution type not supported");
            }
        }

        std::shared_ptr<StochastProperties> GeneralizedExtremeValueDistribution::getStochast(std::shared_ptr<StochastProperties> stochast)
        {
            switch (this->getExtremeDistributionType(stochast))
            {
            case DistributionType::Gumbel:
            {
                return stochast->clone();
            }
            case DistributionType::Frechet:
            {
                std::shared_ptr<StochastProperties> frechetStochast = stochast->clone();
                frechetStochast->Shape = 1 / stochast->Shape;
                frechetStochast->Scale = stochast->Scale / stochast->Shape;
                frechetStochast->Shift = stochast->Shift - stochast->Scale / stochast->Shape;
                return frechetStochast;
            }
            case DistributionType::Weibull:
            {
                std::shared_ptr<StochastProperties> weibullStochast = stochast->clone();
                weibullStochast->Shape = -1 / stochast->Shape;
                weibullStochast->Scale = -stochast->Scale / stochast->Shape;
                weibullStochast->Shift = stochast->Shift - stochast->Scale / stochast->Shape;
                return weibullStochast;
            }
            default:
                throw Reliability::probLibException("Extreme distribution type not supported");
            }
        }

        void GeneralizedExtremeValueDistribution::initialize(std::shared_ptr<StochastProperties> stochast, std::vector<double> values)
        {
            setMeanAndDeviation(stochast, values[0], values[1]);
        }

        bool GeneralizedExtremeValueDistribution::isValid(std::shared_ptr<StochastProperties> stochast)
        {
            return this->getDistribution(stochast)->isValid(getStochast(stochast));
        }

        bool GeneralizedExtremeValueDistribution::isVarying(std::shared_ptr<StochastProperties> stochast)
        {
            return this->getDistribution(stochast)->isVarying(getStochast(stochast));
        }

        double GeneralizedExtremeValueDistribution::getMean(std::shared_ptr<StochastProperties> stochast)
        {
            return this->getDistribution(stochast)->getMean(getStochast(stochast));
        }

        double GeneralizedExtremeValueDistribution::getDeviation(std::shared_ptr<StochastProperties> stochast)
        {
            return this->getDistribution(stochast)->getDeviation(getStochast(stochast));
        }

        double GeneralizedExtremeValueDistribution::getXFromU(std::shared_ptr<StochastProperties> stochast, double u)
        {
            return this->getDistribution(stochast)->getXFromU(getStochast(stochast), u);
        }

        double GeneralizedExtremeValueDistribution::getUFromX(std::shared_ptr<StochastProperties> stochast, double x)
        {
            return this->getDistribution(stochast)->getUFromX(getStochast(stochast), x);
        }

        double GeneralizedExtremeValueDistribution::getPDF(std::shared_ptr<StochastProperties> stochast, double x)
        {
            return this->getDistribution(stochast)->getPDF(getStochast(stochast), x);
        }

        double GeneralizedExtremeValueDistribution::getCDF(std::shared_ptr<StochastProperties> stochast, double x)
        {
            return this->getDistribution(stochast)->getCDF(getStochast(stochast), x);
        }

        void GeneralizedExtremeValueDistribution::setMeanAndDeviation(std::shared_ptr<StochastProperties> stochast, double mean, double deviation)
        {
            std::shared_ptr<StochastProperties> gevStochast = this->getStochast(stochast);
            this->getDistribution(stochast)->setMeanAndDeviation(gevStochast, mean, deviation);
            this->assign(gevStochast, stochast);
        }

        void GeneralizedExtremeValueDistribution::setXAtU(std::shared_ptr<StochastProperties> stochast, double x, double u, ConstantParameterType constantType)
        {
            std::shared_ptr<StochastProperties> gevStochast = getStochast(stochast);
            this->getDistribution(stochast)->setXAtU(gevStochast, x, u, constantType);
            this->assign(gevStochast, stochast);
        }

        void GeneralizedExtremeValueDistribution::fit(std::shared_ptr<StochastProperties> stochast, std::vector<double>& values)
        {
            double bestGoodness = std::numeric_limits<double>::max();
            std::shared_ptr<StochastProperties> bestStochast = nullptr;
            double bestShape = -1;

            for (auto shape : std::vector<double>{ -1, 0, 1 })
            {
                std::shared_ptr<StochastProperties> dStochast = std::make_shared<StochastProperties>();
                dStochast->Shape = shape;

                const std::shared_ptr<Distribution> distribution = getDistribution(dStochast);

                distribution->fit(dStochast, values);

                if (!std::isnan(dStochast->Shape) && !std::isnan(dStochast->Scale))
                {
                    double goodness = KSCalculator::getGoodnessOfFit(values, distribution, dStochast);

                    if (goodness < bestGoodness)
                    {
                        bestGoodness = goodness;
                        bestStochast = dStochast;
                        bestShape = shape;
                    }
                }
            }

            stochast->Shape = bestShape;

            this->assign(bestStochast, stochast);
        }

        std::vector<double> GeneralizedExtremeValueDistribution::getSpecialPoints(std::shared_ptr<StochastProperties> stochast)
        {
            return this->getDistribution(stochast)->getSpecialPoints(getStochast(stochast));
        }

        void GeneralizedExtremeValueDistribution::assign(std::shared_ptr<StochastProperties> source, std::shared_ptr<StochastProperties> target)
        {
            if (source != target)
            {
                switch (this->getExtremeDistributionType(target))
                {
                case DistributionType::Gumbel:
                    target->Shift = source->Shift;
                    target->Shape = source->Shape;
                    target->Scale = source->Scale;
                    break;
                case DistributionType::Frechet:
                    target->Shift = source->Shift + source->Scale;
                    target->Shape = 1 / source->Shape;
                    target->Scale = source->Scale / source->Shape;
                    break;
                case DistributionType::Weibull:
                    target->Shift = source->Shift - source->Scale;
                    target->Shape = -1 / source->Shape;
                    target->Scale = source->Scale / source->Shape;
                    break;
                default: throw Reliability::probLibException("Extreme distribution type not supported");
                }
            }
        }
    }
}



