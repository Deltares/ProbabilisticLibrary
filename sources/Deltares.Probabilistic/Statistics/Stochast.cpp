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
            std::shared_ptr<StochastProperties> valueSetProperties = this->ValueSet->getInterpolatedStochast(xSource);

            return this->distribution->getXFromU(valueSetProperties, u);
        }

        void Stochast::setXAtU(double x, double u, ConstantParameterType constantType)
        {
            this->distribution->setXAtU(properties, x, u, constantType);
        }

        void Stochast::setDistributionType(DistributionType distributionType)
        {
            double oldMean = this->getMean();
            double oldDeviation = this->getDistributionType() == Deterministic ? this->getProperties()->Scale : this->getDeviation();

            DistributionType oldDistributionType = this->distributionType;
            bool canSetOldValues = oldMean != 0 || oldDeviation != 0;

            this->distributionType = distributionType;
            this->distribution = DistributionLibrary::getDistribution(this->distributionType, truncated, inverted);
            this->properties->dirty = true;

            if (canSetOldValues && this->distribution->maintainMeanAndDeviation(this->properties))
            {
                if (oldDistributionType == DistributionType::Table && this->canFit())
                {
                    this->fitFromHistogramValues();
                }
                else
                {
                    this->setMeanAndDeviation(oldMean, oldDeviation);
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
            default: throw Reliability::probLibException("distribution type");
            }
        }


    }
}



