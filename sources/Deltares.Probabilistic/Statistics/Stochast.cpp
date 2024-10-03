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
#include "Stochast.h"

#include "StandardNormal.h"
#include "Distributions/DistributionLibrary.h"
#include "Distributions/InvertedDistribution.h"
#include "Distributions/ExternalDistribution.h"

namespace Deltares
{
    namespace Statistics
    {
        Stochast::Stochast() { };

        Stochast::Stochast(DistributionType distributionType, std::vector<double> values)
        {
            setDistributionType(distributionType);
            distribution->initialize(properties, values);
            this->properties->dirty = true;
        }

        Stochast::Stochast(DistributionType distributionType, std::shared_ptr<StochastProperties> properties)
        {
            setDistributionType(distributionType);
            this->properties = properties;
            this->properties->dirty = true;
        }

        void Stochast::SetDirty() const
        {
            this->properties->dirty = true;
        }

        double Stochast::getPDF(double x)
        {
            return this->distribution->getPDF(properties, x);
        }

        double Stochast::getCDF(double x)
        {
            return this->distribution->getCDF(properties, x);
        }

        double Stochast::getQuantile(double quantile)
        {
            double u = StandardNormal::getUFromP(quantile);
            return this->distribution->getXFromU(properties, u);
        }

        double Stochast::getXFromU(double u)
        {
            return this->distribution->getXFromU(properties, u);
        }

        double Stochast::getUFromX(double x)
        {
            return this->distribution->getUFromX(properties, x);
        }

        double Stochast::getXFromUAndSource(double xSource, double u)
        {
            if (this->IsVariableStochast)
            {
                std::shared_ptr<StochastProperties> valueSetProperties = this->ValueSet->getInterpolatedStochast(xSource);
                return this->distribution->getXFromU(valueSetProperties, u);
            }
            else
            {
                return this->getXFromU(u);
            }
        }

        void Stochast::setXAtU(double x, double u, ConstantParameterType constantType)
        {
            this->distribution->setXAtU(properties, x, u, constantType);
        }

        void Stochast::setDistributionType(DistributionType distributionType)
        {
            if (this->distributionType != distributionType)
            {
                double oldMean = 0;
                double oldDeviation = 0;

                DistributionChangeType distributionChangeType = this->distributionChangeType;
                if (this->distributionType == DistributionType::Table)
                {
                    distributionChangeType = DistributionChangeType::FitFromHistogramValues;
                }

                if (distributionChangeType == DistributionChangeType::MaintainMeanAndDeviation)
                {
                    oldMean = this->getMean();
                    oldDeviation = this->getDistributionType() == Deterministic ? this->getProperties()->Scale : this->getDeviation();
                }

                this->distributionType = distributionType;
                this->distribution = DistributionLibrary::getDistribution(this->distributionType, truncated, inverted);
                this->properties->dirty = true;

                if (this->distribution->maintainMeanAndDeviation(this->properties))
                {
                    if (distributionChangeType == DistributionChangeType::MaintainMeanAndDeviation)
                    {
                        if (oldMean != 0 || oldDeviation != 0)
                        {
                            this->setMeanAndDeviation(oldMean, oldDeviation);
                        }
                    }
                    else if (distributionChangeType == DistributionChangeType::FitFromHistogramValues)
                    {
                        if (this->canFit() && !this->getProperties()->HistogramValues.empty())
                        {
                            this->fitFromHistogramValues();
                        }
                    }
                }
            }
        }

        bool Stochast::hasParameter(DistributionPropertyType distributionPropertyType)
        {
            for (DistributionPropertyType availableDistributionPropertyType : this->distribution->getParameters())
            {
                if (availableDistributionPropertyType == distributionPropertyType)
                {
                    return true;
                }
            }

            return false;
        }

        void Stochast::setExternalDistribution(UXLambda externalFunction)
        {
            this->distributionType = DistributionType::External;

            std::shared_ptr<ExternalDistribution> externalDistribution = std::make_shared<ExternalDistribution>();
            externalDistribution->setExternalFunction(externalFunction);

            this->distribution = externalDistribution;
        }

        DistributionType Stochast::getDistributionType()
        {
            return this->distributionType;
        }

        bool Stochast::canInvert()
        {
            return this->distribution->canInvert();
        }

        bool Stochast::isInverted()
        {
            return inverted;
        }

        void Stochast::setInverted(bool inverted)
        {
            this->inverted = inverted;

            if (this->distributionType != DistributionType::External)
            {
                this->distribution = DistributionLibrary::getDistribution(this->distributionType, truncated, inverted);
            }
        }

        bool Stochast::canTruncate()
        {
            return this->distribution->canTruncate();
        }

        bool Stochast::isTruncated()
        {
            return truncated;
        }

        void Stochast::setTruncated(bool truncated)
        {
            this->truncated = truncated;

            if (this->distributionType != DistributionType::External)
            {
                this->distribution = DistributionLibrary::getDistribution(this->distributionType, truncated, inverted);
            }
        }

        bool Stochast::isVarying()
        {
            if (this->IsVariableStochast)
            {
                return this->ValueSet->isVarying(this->distributionType);
            }
            else
            {
                return this->distribution->isVarying(properties);
            }
        }

        bool Stochast::isValid()
        {
            return this->distribution->isValid(properties);
        }

        bool Stochast::isQualitative()
        {
            return this->distribution->isQualitative();
        }

        double Stochast::getRepresentativeU(double u)
        {
            return this->distribution->getRepresentativeU(properties, u);
        }

        double Stochast::getMean()
        {
            return distribution->getMean(properties);
        }

        void Stochast::setMean(double mean)
        {
            double deviation = this->getDistributionType() == Deterministic ? this->getProperties()->Scale : this->getDeviation();
            distribution->setMeanAndDeviation(properties, mean, deviation);
        }

        double Stochast::getDeviation()
        {
            return distribution->getDeviation(properties);
        }

        void Stochast::setDeviation(double deviation)
        {
            double mean = distribution->getMean(properties);
            distribution->setMeanAndDeviation(properties, mean, deviation);
        }

        void Stochast::setShift(double shift)
        {
            distribution->setShift(properties, shift, false);
        }

        void Stochast::setMeanAndDeviation(double mean, double deviation)
        {
            distribution->setMeanAndDeviation(properties, mean, deviation);
        }

        void Stochast::initializeForRun()
        {
            distribution->initializeForRun(properties);

            if (this->IsVariableStochast)
            {
                this->ValueSet->initializeForRun();
            }
        }

        bool Stochast::canFit()
        {
            return distribution->canFit();
        }

        void Stochast::fit(std::vector<double> values)
        {
            distribution->fit(properties, values);
        }

        void Stochast::fitWeighted(std::vector<double> values, std::vector<double> weights)
        {
            distribution->fitWeighted(properties, values, weights);
        }

        void Stochast::fitFromHistogramValues()
        {
            std::vector<double> values;

            // TODO: PROBL-42 Use fitWeighted when this has been implemented for all distributions
            for (std::shared_ptr<HistogramValue> bin : this->properties->HistogramValues)
            {
                for (int i = 0; i < bin->Amount; i++)
                {
                    values.push_back(bin->getCenter());
                }
            }

            distribution->fit(properties, values);
        }

        std::vector<double> Stochast::getSpecialXValues()
        {
            return distribution->getSpecialPoints(properties);
        }

        void Stochast::copyFrom(std::shared_ptr<Stochast> source)
        {
            this->distributionChangeType = DistributionChangeType::Nothing;

            this->getProperties()->copyFrom(source->getProperties());
            this->inverted = source->inverted;
            this->truncated = source->truncated;

            this->setDistributionType(source->getDistributionType());

            this->IsVariableStochast = source->IsVariableStochast;
            this->ValueSet->copyFrom(source->ValueSet);

            this->distributionChangeType = source->distributionChangeType;

            this->SetDirty();
        }

        Statistics::DistributionType Stochast::getDistributionType(std::string distributionType)
        {
            if (distributionType == "deterministic") return Statistics::DistributionType::Deterministic;
            else if (distributionType == "normal") return Statistics::DistributionType::Normal;
            else if (distributionType == "log_normal") return Statistics::DistributionType::LogNormal;
            else if (distributionType == "student_t") return Statistics::DistributionType::StudentT;
            else if (distributionType == "uniform") return Statistics::DistributionType::Uniform;
            else if (distributionType == "exponential") return Statistics::DistributionType::Exponential;
            else if (distributionType == "triangular") return Statistics::DistributionType::Triangular;
            else if (distributionType == "trapezoidal") return Statistics::DistributionType::Trapezoidal;
            else if (distributionType == "gumbel") return Statistics::DistributionType::Gumbel;
            else if (distributionType == "weibull") return Statistics::DistributionType::Weibull;
            else if (distributionType == "conditional_weibull") return Statistics::DistributionType::ConditionalWeibull;
            else if (distributionType == "frechet") return Statistics::DistributionType::Frechet;
            else if (distributionType == "generalized_extreme_value") return Statistics::DistributionType::GeneralizedExtremeValue;
            else if (distributionType == "rayleigh") return Statistics::DistributionType::Rayleigh;
            else if (distributionType == "rayleigh_n") return Statistics::DistributionType::RayleighN;
            else if (distributionType == "pareto") return Statistics::DistributionType::Pareto;
            else if (distributionType == "generalized_pareto") return Statistics::DistributionType::GeneralizedPareto;
            else if (distributionType == "beta") return Statistics::DistributionType::Beta;
            else if (distributionType == "gamma") return Statistics::DistributionType::Gamma;
            else if (distributionType == "bernoulli") return Statistics::DistributionType::Bernoulli;
            else if (distributionType == "poisson") return Statistics::DistributionType::Poisson;
            else if (distributionType == "histogram") return Statistics::DistributionType::Table;
            else if (distributionType == "cdf_curve") return Statistics::DistributionType::CDFCurve;
            else if (distributionType == "discrete") return Statistics::DistributionType::Discrete;
            else if (distributionType == "qualitative") return Statistics::DistributionType::Qualitative;
            else if (distributionType == "composite") return Statistics::DistributionType::Composite;
            else if (distributionType == "standard_normal") return Statistics::DistributionType::StandardNormal;
            else throw Reliability::probLibException("distribution type");
        }

        std::string Stochast::getDistributionTypeString(Statistics::DistributionType distributionType)
        {
            switch (distributionType)
            {
            case Statistics::DistributionType::Deterministic: return "deterministic";
            case Statistics::DistributionType::Normal: return "normal";
            case Statistics::DistributionType::LogNormal: return "log_normal";
            case Statistics::DistributionType::StudentT: return "student_t";
            case Statistics::DistributionType::Uniform: return "uniform";
            case Statistics::DistributionType::Exponential: return "exponential";
            case Statistics::DistributionType::Triangular: return "triangular";
            case Statistics::DistributionType::Trapezoidal: return "trapezoidal";
            case Statistics::DistributionType::Gumbel: return "gumbel";
            case Statistics::DistributionType::Weibull: return "weibull";
            case Statistics::DistributionType::ConditionalWeibull: return "conditional_weibull";
            case Statistics::DistributionType::Frechet: return "frechet";
            case Statistics::DistributionType::GeneralizedExtremeValue: return "generalized_extreme_value";
            case Statistics::DistributionType::Rayleigh: return "rayleigh";
            case Statistics::DistributionType::RayleighN: return "rayleigh_n";
            case Statistics::DistributionType::Pareto: return "pareto";
            case Statistics::DistributionType::GeneralizedPareto: return "generalized_pareto";
            case Statistics::DistributionType::Beta: return "beta";
            case Statistics::DistributionType::Gamma: return "gamma";
            case Statistics::DistributionType::Bernoulli: return "bernoulli";
            case Statistics::DistributionType::Poisson: return "poisson";
            case Statistics::DistributionType::Table: return "histogram";
            case Statistics::DistributionType::CDFCurve: return "cdf_curve";
            case Statistics::DistributionType::Discrete: return "discrete";
            case Statistics::DistributionType::Qualitative: return "qualitative";
            case Statistics::DistributionType::Composite: return "composite";
            case Statistics::DistributionType::StandardNormal: return "standard_normal";
            default: throw Reliability::probLibException("distribution type");
            }
        }


    }
}



