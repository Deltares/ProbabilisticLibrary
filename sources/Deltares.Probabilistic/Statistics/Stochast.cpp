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
#include "Stochast.h"

#include "StandardNormal.h"
#include "Distributions/DistributionLibrary.h"
#include "Distributions/InvertedDistribution.h"
#include "Distributions/ExternalDistribution.h"
#include "Distributions/KSCalculator.h"
#include "../Math/NumericSupport.h"

namespace Deltares
{
    namespace Statistics
    {
        using enum DistributionType;

        Stochast::Stochast() = default;

        Stochast::Stochast(DistributionType distributionType, const std::vector<double>& values)
        {
            setDistributionType(distributionType);
            distribution->initialize(*properties, values);
            properties->dirty = true;
        }

        Stochast::Stochast(DistributionType distributionType, std::shared_ptr<StochastProperties> newProperties)
        {
            setDistributionType(distributionType);
            properties = newProperties;
            properties->dirty = true;
        }

        void Stochast::SetDirty() const
        {
            properties->dirty = true;
        }

        std::string Stochast::getIndexedStochastName(int index) const
        {
            if (this->modelParameter->isArray)
            {
                return name + "[" + std::to_string(index) + "]";
            }
            else
            {
                return name;
            }
        }

        double Stochast::getPDF(double x)
        {
            return distribution->getPDF(*properties, x);
        }

        double Stochast::getCDF(double x)
        {
            return distribution->getCDF(*properties, x);
        }

        double Stochast::getQuantile(double quantile) const
        {
            double u = StandardNormal::getUFromP(quantile);
            return distribution->getXFromU(*properties, u);
        }

        double Stochast::getXFromU(double u)
        {
            return distribution->getXFromU(*properties, u);
        }

        double Stochast::getUFromX(double x)
        {
            return distribution->getUFromX(*properties, x);
        }

        double Stochast::getXFromUAndSource(double xSource, double u)
        {
            if (isVariable())
            {
                std::shared_ptr<StochastProperties> valueSetProperties = getInterpolatedProperties(xSource);
                return distribution->getXFromU(*valueSetProperties, u);
            }
            else
            {
                return getXFromU(u);
            }
        }

        double Stochast::getUFromXAndSource(double xSource, double x)
        {
            if (isVariable())
            {
                std::shared_ptr<StochastProperties> valueSetProperties = getInterpolatedProperties(xSource);
                return distribution->getUFromX(*valueSetProperties, x);
            }
            else
            {
                return getUFromX(x);
            }
        }

        void Stochast::setXAtU(double x, double u, ConstantParameterType constantType) const
        {
            distribution->setXAtU(*properties, x, u, constantType);
        }

        double Stochast::getXFromType(RunValuesType type)
        {
            switch (type)
            {
            case RunValuesType::MedianValues: return getXFromU(0);
            case RunValuesType::MeanValues: return getMean();
            case RunValuesType::DesignValues: return getDesignValue();
            default: throw Reliability::probLibException("Value type not supported");
            }
        }

        double Stochast::getXFromTypeAndSource(double xSource, RunValuesType type)
        {
            if (isVariable())
            {
                std::shared_ptr<StochastProperties> valueSetProperties = getInterpolatedProperties(xSource);
                switch (type)
                {
                case RunValuesType::MedianValues: return distribution->getXFromU(*valueSetProperties, 0);
                case RunValuesType::MeanValues: return distribution->getMean(*valueSetProperties);
                case RunValuesType::DesignValues:
                    return distribution->getXFromU(*valueSetProperties, StandardNormal::getUFromP(designQuantile)) / designFactor;
                default: throw Reliability::probLibException("Value type not supported");
                }
            }
            else
            {
                return getXFromType(type);
            }
        }

        bool Stochast::isVariable()
        {
            if (distributionType == DistributionType::Composite)
            {
                for (const auto& contributingStochast : properties->ContributingStochasts)
                {
                    if (contributingStochast->Probability > 0 && contributingStochast->Stochast->isVariable())
                    {
                        return true;
                    }
                }

                return false;
            }
            else
            {
                return IsVariableStochast;
            }
        }

        std::shared_ptr<Stochast> Stochast::getVariableSource()
        {
            if (distributionType == DistributionType::Composite)
            {
                for (const auto& contributingStochast : properties->ContributingStochasts)
                {
                    if (contributingStochast->Probability > 0 && contributingStochast->Stochast->isVariable())
                    {
                        std::shared_ptr<Stochast> stochast = std::static_pointer_cast<Stochast>(contributingStochast->Stochast);
                        return stochast->VariableSource;
                    }
                }

                return nullptr;
            }
            else
            {
                return VariableSource;
            }
        }

        std::shared_ptr<StochastProperties> Stochast::getInterpolatedProperties(double xSource)
        {
            if (distributionType == DistributionType::Composite)
            {
                std::shared_ptr<StochastProperties> compositeProperties = std::make_shared<StochastProperties>();
                for (const auto& compositeStochast : properties->ContributingStochasts)
                {
                    if (compositeStochast->Stochast->isVariable())
                    {
                        std::shared_ptr<Stochast> stochast = std::static_pointer_cast<Stochast>(compositeStochast->Stochast);

                        std::shared_ptr<StochastProperties> interpolatedProperties = stochast->getInterpolatedProperties(xSource);

                        std::shared_ptr<Stochast> interpolatedStochast = std::make_shared<Stochast>(stochast->distributionType, interpolatedProperties);
                        interpolatedStochast->setTruncated(stochast->isTruncated());
                        interpolatedStochast->setInverted(stochast->isInverted());

                        std::shared_ptr<ContributingStochast> interpolatedContributingStochast =
                            std::make_shared<ContributingStochast>(compositeStochast->Probability, interpolatedStochast);

                        compositeProperties->ContributingStochasts.push_back(interpolatedContributingStochast);
                    }
                    else
                    {
                        compositeProperties->ContributingStochasts.push_back(compositeStochast);
                    }
                }

                return compositeProperties;
            }
            else if (IsVariableStochast)
            {
                return ValueSet->getInterpolatedStochast(xSource);
            }
            else
            {
                return properties;
            }
        }

        void Stochast::setDistributionType(DistributionType newDistributionType)
        {
            if (distributionType != newDistributionType)
            {
                double oldMean = 0;
                double oldDeviation = 0;

                DistributionChangeType distributionChangingType = distributionChangeType;

                if (isInitial())
                {
                    distributionChangingType = DistributionChangeType::Nothing;
                }
                else if (distributionType == DistributionType::Table)
                {
                    distributionChangingType = DistributionChangeType::FitFromHistogramValues;
                }

                if (distributionChangingType == DistributionChangeType::MaintainMeanAndDeviation)
                {
                    oldMean = getMean();
                    oldDeviation = getDistributionType() == Deterministic ? getProperties()->Scale : getDeviation();
                }

                distributionType = newDistributionType;
                distribution = DistributionLibrary::getDistribution(distributionType, truncated, inverted);
                properties->dirty = true;

                if (distribution->maintainMeanAndDeviation(*properties))
                {
                    if (distributionChangingType == DistributionChangeType::Nothing)
                    {
                        // nothing to do
                    }
                    else if (distributionChangingType == DistributionChangeType::MaintainMeanAndDeviation)
                    {
                        setMeanAndDeviation(oldMean, oldDeviation);
                    }
                    else if (distributionChangingType == DistributionChangeType::FitFromHistogramValues)
                    {
                        if (canFit() && !getProperties()->HistogramValues.empty())
                        {
                            fitFromHistogramValues();
                        }
                    }
                }
            }
        }

        bool Stochast::isInitial() const
        {
            return distributionType == Deterministic && properties->Location == 0.0 && properties->Scale == 0.0;
        }

        bool Stochast::hasParameter(DistributionPropertyType distributionPropertyType) const
        {
            for (DistributionPropertyType availableDistributionPropertyType : distribution->getParameters())
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
            distributionType = DistributionType::External;

            std::shared_ptr<ExternalDistribution> externalDistribution = std::make_shared<ExternalDistribution>();
            externalDistribution->setExternalFunction(externalFunction);

            distribution = externalDistribution;
        }

        DistributionType Stochast::getDistributionType() const
        {
            return distributionType;
        }

        bool Stochast::canInvert() const
        {
            return distribution->canInvert();
        }

        bool Stochast::isInverted() const
        {
            return inverted;
        }

        void Stochast::setInverted(bool newInverted)
        {
            inverted = newInverted;

            if (distributionType != DistributionType::External)
            {
                distribution = DistributionLibrary::getDistribution(distributionType, truncated, newInverted);
            }
        }

        bool Stochast::canTruncate() const
        {
            return distribution->canTruncate();
        }

        bool Stochast::isTruncated() const
        {
            return truncated;
        }

        void Stochast::setTruncated(bool newTruncated)
        {
            truncated = newTruncated;

            if (distributionType != DistributionType::External)
            {
                distribution = DistributionLibrary::getDistribution(distributionType, truncated, inverted);
            }
        }

        bool Stochast::isVarying()
        {
            if (IsVariableStochast && distributionType != DistributionType::Composite)
            {
                return ValueSet->isVarying(distributionType, properties);
            }
            else if (modelParameter->isArray && !ArrayVariables.empty())
            {
                for (const std::shared_ptr<Stochast>& arrayVariable : ArrayVariables)
                {
                    if (arrayVariable->isVarying())
                    {
                        return true;
                    }
                }

                return false;
            }
            else
            {
                return distribution->isVarying(*properties);
            }
        }

        void Stochast::validate(Logging::ValidationReport& report)
        {
            if (IsVariableStochast)
            {
                initializeConditionalValues();
                ValueSet->validate(report, distributionType, truncated, inverted, name);
            }
            else
            {
                distribution->validate(report, *properties, name);
            }
        }

        Logging::ValidationReport Stochast::getValidationReport()
        {
            Logging::ValidationReport report;
            validate(report);

            return report;
        }

        bool Stochast::isValid()
        {
            return getValidationReport().isValid();
        }

        bool Stochast::isQualitative() const
        {
            return distribution->isQualitative();
        }

        double Stochast::getRepresentativeU(double u) const
        {
            return distribution->getRepresentativeU(*properties, u);
        }

        double Stochast::getMean()
        {
            return distribution->getMean(*properties);
        }

        void Stochast::setMean(double mean)
        {
            double deviation = getDistributionType() == Deterministic ? getProperties()->Scale : getDeviation();
            if (!isValid())
            {
                deviation = 0;
            }

            if (constantParameterType == ConstantParameterType::Deviation)
            {
                distribution->setMeanAndDeviation(*properties, mean, deviation);
            }
            else if (constantParameterType == ConstantParameterType::VariationCoefficient)
            {
                double currentMean = getMean();
                if (currentMean == 0.0)
                {
                    distribution->setMeanAndDeviation(*properties, mean, mean * lastVariation);
                }
                else
                {
                    double variation = deviation / currentMean;
                    distribution->setMeanAndDeviation(*properties, mean, std::abs(mean * variation));
                }
            }
        }

        double Stochast::getDeviation() const
        {
            return distribution->getDeviation(*properties);
        }

        void Stochast::setDeviation(double deviation)
        {
            double mean = getMean();
            distribution->setMeanAndDeviation(*properties, mean, deviation);
        }

        double Stochast::getVariation()
        {
            double mean = getMean();

            if (mean == 0.0)
            {
                return std::numeric_limits<double>::infinity();
            }
            else
            {
                return getDeviation() / std::abs(mean);
            }
        }

        void Stochast::setVariation(double variation)
        {
            double mean = getMean();
            distribution->setMeanAndDeviation(*properties, mean, variation * std::abs(mean));
            lastVariation = variation;
        }

        void Stochast::setMeanAndDeviation(double mean, double deviation) const
        {
            distribution->setMeanAndDeviation(*properties, mean, deviation);
        }

        void Stochast::setShift(double shift) const
        {
            distribution->setShift(*properties, shift, false);
        }

        void Stochast::initializeForRun()
        {
            distribution->initializeForRun(*properties);

            if (IsVariableStochast)
            {
                initializeConditionalValues();
            }

            if (distributionType == DistributionType::Composite)
            {
                for (const auto& contributingStochast : properties->ContributingStochasts)
                {
                    contributingStochast->Stochast->initializeForRun();
                }
            }
        }

        void Stochast::initializeConditionalValues()
        {
            if (distributionType == DistributionType::Composite)
            {
                for (const auto& contributingStochast : properties->ContributingStochasts)
                {
                    if (contributingStochast->Stochast->isVariable())
                    {
                        contributingStochast->Stochast->initializeConditionalValues();
                    }
                }
            }
            else
            {
                ValueSet->initializeForRun(properties, distributionType, truncated, inverted);
            }
        }

        void Stochast::updateFromConditionalValues(double xSource) const
        {
            if (IsVariableStochast)
            {
                ValueSet->updateProperties(properties, xSource);
            }
        }

        bool Stochast::canFit() const
        {
            return distribution->canFit();
        }

        bool Stochast::canFitPrior() const
        {
            return distribution->canFitPrior();
        }

        Logging::ValidationReport Stochast::getFitValidationReport(std::vector<double>& values, const std::shared_ptr<Stochast>& prior, const double shift) const
        {
            Logging::ValidationReport report;

            Logging::ValidationSupport::checkNotEmpty(report, values.size(), "values");

            if (!this->canFit())
            {
                Logging::ValidationSupport::add(report, "Fit is not supported for distribution type " + Stochast::getDistributionTypeString(distributionType) + ".", name);
            }

            if (!values.empty() && !std::isnan(shift))
            {
                double minValue = distribution->getMaxShiftValue(values);

                if (inverted)
                {
                    Logging::ValidationSupport::checkMinimumNonInclusive(report, minValue, shift, "shift");
                }
                else
                {
                    Logging::ValidationSupport::checkMaximumNonInclusive(report, minValue, shift, "shift");
                }
            }

            if (prior != nullptr)
            {
                if (!this->canFitPrior())
                {
                    Logging::ValidationSupport::add(report, "Fit with prior is not supported for distribution type " + Stochast::getDistributionTypeString(distributionType) + ".", name);
                }

                if (distributionType != prior->distributionType)
                {
                    Logging::ValidationSupport::add(report, "Fit from prior with other distribution type is not supported.", name);
                }
            }

            return report;
        }

        void Stochast::fit(std::vector<double>& values, const double shift) const
        {
            Logging::ValidationReport report = getFitValidationReport(values, nullptr, shift);
            if (!report.isValid())
            {
                throw Reliability::probLibException("Can not fit with given Values should not be empty");
            }

            distribution->fit(*properties, values, shift);
        }

        void Stochast::fitPrior(std::vector<double>& values, std::shared_ptr<Stochast> prior, const double shift) const
        {
            Logging::ValidationReport report = getFitValidationReport(values, prior, shift);
            if (!report.isValid())
            {
                throw Reliability::probLibException("Can not fit with given Values should not be empty");
            }

            distribution->fitPrior(*properties, values, *prior->getProperties(), shift);
        }

        void Stochast::fitWeighted(const std::vector<double>& values, std::vector<double> weights) const
        {
            distribution->fitWeighted(*properties, values, weights);
        }

        void Stochast::fitFromHistogramValues() const
        {
            std::vector<double> values;
            std::vector<double> weights;

            for (std::shared_ptr<HistogramValue> bin : properties->HistogramValues)
            {
                values.push_back(bin->getCenter());
                weights.push_back(bin->Amount);
            }

            distribution->fitWeighted(*properties, values, weights);
        }

        double Stochast::getKSTest(const std::vector<double>& values) const
        {
            return KSCalculator::getGoodnessOfFit(values, *distribution, *properties);
        }

        std::shared_ptr<Stochast> Stochast::getVariableStochast(double x)
        {
            std::shared_ptr<Stochast> stochast = std::make_shared<Stochast>(distributionType, ValueSet->getInterpolatedStochast(x));
            stochast->name = name;
            return stochast;
        }

        double Stochast::getDesignValue()
        {
            double u = StandardNormal::getUFromP(designQuantile);
            return getXFromU(u) / designFactor;
        }

        void Stochast::setDesignValue(double designValue)
        {
            if (designFactor >= 0 && designQuantile >= 0 && designQuantile <= 1)
            {
                if (getMean() == 0.0 && lastVariation > 0)
                {
                    constexpr double disturbedMeanValue = 0.1;
                    setMeanAndDeviation(disturbedMeanValue, lastVariation * disturbedMeanValue);
                }
                setXAtU(designValue * designFactor, StandardNormal::getUFromP(designQuantile), ConstantParameterType::VariationCoefficient);
            }
        }

        std::vector<double> Stochast::getSpecialXValues()
        {
            return distribution->getSpecialPoints(*properties);
        }

        std::vector<double> Stochast::getDiscontinuityPoints()
        {
            return distribution->getDiscontinuityPoints(*properties);
        }

        void Stochast::copyFrom(std::shared_ptr<Stochast> source)
        {
            distributionChangeType = DistributionChangeType::Nothing;

            properties->copyFrom(source->getProperties());
            inverted = source->inverted;
            truncated = source->truncated;

            setDistributionType(source->getDistributionType());

            IsVariableStochast = source->IsVariableStochast;
            ValueSet->copyFrom(source->ValueSet);

            distributionChangeType = source->distributionChangeType;

            SetDirty();
        }

        Statistics::ConstantParameterType Stochast::getConstantParameterType(std::string distributionType)
        {
            if (distributionType == "deviation") return Statistics::ConstantParameterType::Deviation;
            else if (distributionType == "variation") return Statistics::ConstantParameterType::VariationCoefficient;
            else throw Reliability::probLibException("constant parameter type");
        }

        std::string Stochast::getConstantParameterTypeString(Statistics::ConstantParameterType distributionType)
        {
            switch (distributionType)
            {
            case Statistics::ConstantParameterType::Deviation: return "deviation";
            case Statistics::ConstantParameterType::VariationCoefficient: return "variation";
            default:  throw Reliability::probLibException("constant parameter type");
            }
        }

        DistributionType Stochast::getDistributionType(const std::string& distributionType)
        {
            if (distributionType == "deterministic") return Deterministic;
            else if (distributionType == "normal") return Normal;
            else if (distributionType == "log_normal") return LogNormal;
            else if (distributionType == "student_t") return StudentT;
            else if (distributionType == "uniform") return Uniform;
            else if (distributionType == "exponential") return Exponential;
            else if (distributionType == "triangular") return Triangular;
            else if (distributionType == "trapezoidal") return Trapezoidal;
            else if (distributionType == "gumbel") return Gumbel;
            else if (distributionType == "weibull") return Weibull;
            else if (distributionType == "conditional_weibull") return ConditionalWeibull;
            else if (distributionType == "frechet") return Frechet;
            else if (distributionType == "generalized_extreme_value") return GeneralizedExtremeValue;
            else if (distributionType == "rayleigh") return Rayleigh;
            else if (distributionType == "rayleigh_n") return RayleighN;
            else if (distributionType == "pareto") return Pareto;
            else if (distributionType == "generalized_pareto") return GeneralizedPareto;
            else if (distributionType == "beta") return Beta;
            else if (distributionType == "gamma") return Gamma;
            else if (distributionType == "bernoulli") return Bernoulli;
            else if (distributionType == "poisson") return Poisson;
            else if (distributionType == "histogram") return Table;
            else if (distributionType == "cdf_curve") return CDFCurve;
            else if (distributionType == "discrete") return Discrete;
            else if (distributionType == "qualitative") return Qualitative;
            else if (distributionType == "composite") return Composite;
            else if (distributionType == "standard_normal") return DistributionType::StandardNormal;
            else throw Reliability::probLibException("distribution type");
        }

        std::string Stochast::getDistributionTypeString(DistributionType distributionType)
        {
            switch (distributionType)
            {
            case Deterministic: return "deterministic";
            case Normal: return "normal";
            case LogNormal: return "log_normal";
            case StudentT: return "student_t";
            case Uniform: return "uniform";
            case Exponential: return "exponential";
            case Triangular: return "triangular";
            case Trapezoidal: return "trapezoidal";
            case Gumbel: return "gumbel";
            case Weibull: return "weibull";
            case ConditionalWeibull: return "conditional_weibull";
            case Frechet: return "frechet";
            case GeneralizedExtremeValue: return "generalized_extreme_value";
            case Rayleigh: return "rayleigh";
            case RayleighN: return "rayleigh_n";
            case Pareto: return "pareto";
            case GeneralizedPareto: return "generalized_pareto";
            case Beta: return "beta";
            case Gamma: return "gamma";
            case Bernoulli: return "bernoulli";
            case Poisson: return "poisson";
            case Table: return "histogram";
            case CDFCurve: return "cdf_curve";
            case Discrete: return "discrete";
            case Qualitative: return "qualitative";
            case Composite: return "composite";
            case DistributionType::StandardNormal: return "standard_normal";
            default: throw Reliability::probLibException("distribution type");
            }
        }


    }
}



