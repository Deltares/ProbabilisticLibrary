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
#pragma once
#include <memory>
#include <vector>

#include "BaseStochast.h"
#include "StochastProperties.h"
#include "Distributions/DeterministicDistribution.h"
#include "Distributions/Distribution.h"
#include "DistributionType.h"
#include "VariableStochastValueSet.h"
#include "DistributionChangeType.h"
#include "../Logging/ValidationReport.h"
#include "../Model/ModelInputParameter.h"

namespace Deltares
{
    namespace Statistics
    {
        enum RunValuesType { MeanValues, MedianValues, DesignValues };

        /**
         * \brief Defines a stochastic variable
         * \remark Contains parameters and a distribution type which describe the stochastic behaviour
         */
        class Stochast : public BaseStochast
        {
        private:
            DistributionType distributionType = DistributionType::Deterministic;
            std::shared_ptr<StochastProperties> properties = std::make_shared<StochastProperties>();

            bool inverted = false;
            bool truncated = false;
            double lastVariation = 0;

            /**
             * \brief Indicates whether the stochast is only initialized and nothing has been changed yet
             */
            bool isInitial() const;

            /**
             * \brief Gets the interpolated properties in case of a variable stochast
             */
            std::shared_ptr<StochastProperties> getInterpolatedProperties(double xSource);

            /**
             * \brief Validates the stochastic parameters and puts the results in a report
             * \param report The validation report containing the validation results
             * \param arrayIndex The index where this stochast inthe ArrayVariables of another staocht, -1 if not present
             */
            void validate(Logging::ValidationReport& report, int arrayIndex);



            static const std::vector<std::pair<const char*, DistributionType>> allDistributions;

        protected:
            std::shared_ptr<Distribution> distribution = std::make_shared<DeterministicDistribution>();

        public:
            /**
             * \brief Default constructor
             */
            Stochast();

            /**
             * \brief Constructor with distribution type and initial values
             * \remark The method initialize of the distribution is used to populate the stochastic parameters
             * \param distributionType Distribution type
             * \param values Initial values
             */
            Stochast(DistributionType distributionType, const std::vector<double>& values);

            /**
             * \brief Constructor with object containing the stochastic parameters
             * \param distributionType Distribution type
             * \param newProperties Object containing stochastic parameters
             */
            Stochast(DistributionType distributionType, std::shared_ptr<StochastProperties> newProperties);

            /**
             * \brief Reference to input parameter of a model
             */
            std::shared_ptr<Models::ModelInputParameter> modelParameter = std::make_shared<Models::ModelInputParameter>();

            /**
             * \brief identifying string for the user
             */
            std::string name = "";

            /**
             * \brief Gets the stochastic properties
             * \return Object containing the stochastic properties
             */
            std::shared_ptr<StochastProperties> getProperties() { return properties; }

            /**
             * \brief Gets the Probability Density Function (PDF) for a given x-value
             * \param x Given x-value
             * \return PDF
             */
            double getPDF(double x) override;

            /**
             * \brief Gets the Cumulative Density Function (CDF) for a given x-value
             * \param x Given x-value
             * \return CDF
             */
            double getCDF(double x) override;

            /**
             * \brief Gets the x-value corresponding to a quantile (non-exceeding probability)
             * \param quantile Given quantile
             * \return x-value
             */
            double getQuantile(double quantile) const;

            /**
             * \brief Gets the x-value corresponding to a certain type
             * \param type the kind of x-value to be returned
             * \return x-value
             */
            double getXFromType(RunValuesType type);

            /**
             * \brief Gets the x-value corresponding to a given u-value
             * \param u Given u-value
             * \return x-value
             */
            double getXFromU(double u) override;

            /**
             * \brief Gets the u-value corresponding to a given x-value
             * \param x Given x-value
             * \return u-value
             */
            double getUFromX(double x) override;

            /**
             * \brief Gets the x-value for a given u-value for variable stochasts, i.e. stochasts where the stochastic parameters depend on the value of another stochast
             * \param xSource Other stochast
             * \param u Given u-value
             * \return x-value
             */
            double getXFromUAndSource(double xSource, double u);

            /**
             * \brief Gets the x-value for a given type for variable stochasts, i.e. stochasts where the stochastic parameters depend on the value of another stochast
             * \param xSource Other stochast
             * \param type The kind of type
             * \return x-value
             */
            double getXFromTypeAndSource(double xSource, RunValuesType type);

            /**
             * \brief Gets the u-value for a given x-value for variable stochasts, i.e. stochasts where the stochastic parameters depend on the value of another stochast
             * \param xSource Other stochast
             * \param x Given x-value
             * \return u-value
             */
            double getUFromXAndSource(double xSource, double x);

            /**
             * \brief Modifies the stochast so that a given x-value is returned for a given u-value
             * \param x Given x-value
             * \param u Given u-value
             * \param constantType Characteristic value of a stochast, which should be kept constant while applying this value. Can be variation coefficient or standard deviation.
             */
            void setXAtU(double x, double u, ConstantParameterType constantType) const;

            /**
             * \brief Sets the distribution type
             * \remark Internally, the Distribution field is updated
             * \param newDistributionType Distribution type
             */
            virtual void setDistributionType(DistributionType newDistributionType);

            /**
             * \brief Gets the distribution type
             * \return Distribution type
             */
            DistributionType getDistributionType() const;

            /**
             * \brief Sets a function which provides the u->x conversion. Overrules the distribution type
             * \param externalFunction Function
             */
            void setExternalDistribution(UXLambda externalFunction);

            /**
             * \brief Indicates whether the distribution can be inverted
             * \return Indication
             */
            bool canInvert() const;

            /**
             * \brief Indicates whether the distribution is inverted
             * \return Indication
             */
            bool isInverted() const;

            /**
             * \brief Inverts or non-inverts the distribution
             * \param newInverted Inverts (true) or non-inverts (false) the distribution
             */
            void setInverted(bool newInverted);

            /**
             * \brief Indicates whether the distribution can be truncated
             * \return Indication
             */
            bool canTruncate() const;

            /**
             * \brief Indicates whether the distribution is truncated
             * \return Indication
             */
            bool isTruncated() const;

            /**
             * \brief Truncates or non-truncates the distribution
             * \param newTruncated Truncates (true) or non-truncates (false) the distribution
             */
            void setTruncated(bool newTruncated);

            /**
             * \brief Indicates whether different u-values can lead to different x-values
             * \return Indication
             */
            bool isVarying() override;

            /**
             * \brief Indicates whether it is useful (false) or not (true) to compare x-values to each other.
             * \remark When x-values indicate the index of an input file, it is not useful to compare x-values
             * \return Indication
             */
            bool isQualitative() const;

            /**
             * \brief Gets the representative u-value for a given u-value
             * \remark When several u-values lead to the same x-value, their representative u-value is the same
             * \param u Given u-value
             * \return Representative u-value
             */
            double getRepresentativeU(double u) const;

            /**
             * \brief Gets the mean value of a stochast
             * \return Mean value
             */
            double getMean() override;

            /**
             * \brief Sets the mean value of a stochast
             * \remark This will lead to a modification of the stochast parameters, such as the location. The standard deviation will not be changed.
             * \param mean Mean value
             */
            void setMean(double mean);

            /**
             * \brief Gets the standard deviation of a stochast
             * \return Standard deviation
             */
            double getDeviation() const;

            /**
             * \brief Sets the standard deviation of a stochast
             * \remark This will lead to a modification of the stochast parameters, such as the scale. The mean value will not be changed.
             * \param deviation Standard deviation
             */
            void setDeviation(double deviation);

            /**
             * \brief Sets the mean value and standard deviation of a stochast
             * \remark This will lead to a modification of the stochast parameters, such as the location and scale
             * \param mean Given mean value
             * \param deviation Given standard deviation
             */
            void setMeanAndDeviation(double mean, double deviation) const;

            /**
             * \brief Gets the variation coefficient of a stochast
             * \return Variation coefficient
             */
            double getVariation();

            /**
             * \brief Sets the variation coefficient of a stochast
             * \remark This will lead to a modification of the stochast parameters, such as the scale. The mean value will not be changed.
             * \param variation Variation coefficient
             */
            void setVariation(double variation);

            /**
             * \brief Indicates whether this stochast is a fragility curve
             * \return Indication
             */
            virtual bool isFragilityCurve() { return false; }

            /**
             * \brief Gets the name of the stochast followed by an index if the stochast is an array, otherwise just the stochast name
             * \return Stochast name, possibly indexed
             */
            std::string getIndexedStochastName(int index) const;

            /**
             * \brief Indicates which parameter should be kept constant when the mean value is changed (by setMean())
             * \return Constant parameter type
             */
            ConstantParameterType constantParameterType = ConstantParameterType::Deviation;

            /**
             * \brief Sets the shift value of a stochast
             * \remark Keeps mean and standard deviation constant when applying it to a log normal distribution
             * \param shift Shift value
             */
            void setShift(double shift) const;

            /**
             * \brief Prepares the stochast for fast response of u->x conversion
             */
            void initializeForRun() override;

            /**
             * \brief Prepares a conditional stochast for running
             */
            void initializeConditionalValues() override;

            /**
             * \brief Updates the stochast properties with conditional values
             */
            void updateFromConditionalValues(double xSource) const;

            /**
             * \brief Indicates whether stochastic parameters can be estimated for a given set of x-values
             * \param useShift Indicates whether a value for shift is provided for the fit procedure
             * \param usePrior Indicates whether a prior distribution is provided for the fit procedure
             * \return Indication
             */
            bool canFit(const bool useShift = false, const bool usePrior = false) const;

            /**
             * \brief Estimates stochastic parameters for a given set of x-values
             * \param values Given set of x-values
             * \param shift If set, shift is not fitted but uses given shift parameter
             */
            void fit(std::vector<double>& values, const double shift = nan("")) const;

            /**
             * \brief Estimates stochastic parameters for a given set of x-values and a prior variable
             * \param values Given set of x-values
             * \param prior Prior variable
             * \param shift If set, shift is not fitted but uses given shift parameter
             */
            void fitPrior(std::vector<double>& values, std::shared_ptr<Stochast> prior, const double shift = nan("")) const;

            /**
             * \brief Gets a validation report which tells whether a fit operation is allowed
             * \param values Given set of x-values
             * \param prior If set, validates whether a fit with prior is allowed
             * \param shift If set, shift is not fitted but uses given shift parameter
             */
            Logging::ValidationReport getFitValidationReport(std::vector<double>& values, const std::shared_ptr<Stochast>& prior = nullptr, const double shift = nan("")) const;

            /**
             * \brief Estimates stochastic parameters for a given set of x-values and their weights
             * \param values Given set of x-values
             * \param weights Given weights
             */
            void fitWeighted(const std::vector<double>& values, std::vector<double> weights) const;

            /**
             * \brief Fits the distribution properties from the histogram values in the properties
             */
            void fitFromHistogramValues() const;

            /**
             * \brief Calculates the Kolmogorov-Smirnov statistic, which is an indication how well data correspond with the stochast (0 = perfect, 1 = no correspondence at all)
             * \return Kolmogorov-Smirnov statistic
             */
            double getKSTest(const std::vector<double>& values) const;

            /**
             * \brief Gets a number of interesting x-values
             * \remark This method is used when plotting PDF and CDF
             * \return Interesting x-values
             */
            std::vector<double> getSpecialXValues() override;

            /**
             * \brief Gets a list of x-values where the CDF-function is discontinuous
             * \return List of x-values
             */
            std::vector<double> getDiscontinuityPoints() override;

            /**
             * \brief Indicates whether the stochastic parameter have valid values for the current distribution type
             * \return Indication
             */
            bool isValid() override;

            /**
             * \brief Validates the stochastic parameters and puts the results in a report
             * \param report The validation report containing the validation results
             */
            void validate(Logging::ValidationReport& report) override;

            /**
             * \brief Validates the stochast and puts the result in a validation report
             * \returns Validation report
             */
            Logging::ValidationReport getValidationReport();

            /**
             * \brief Indicates whether the stochast uses a given parameter type
             * \param distributionPropertyType Given parameter type
             * \return Indication
             */
            bool hasParameter(DistributionPropertyType distributionPropertyType) const;

            /**
             * \brief Defines what to do when the distribution type changes
             */
            DistributionChangeType distributionChangeType = DistributionChangeType::MaintainMeanAndDeviation;

            /**
             * \brief Indicates whether the stochastic parameters depend on the x-value of another stochast
             */
            bool IsVariableStochast = false;

            /**
             * \brief In case of a variable stochast, the stochast of which the x-value is used to define the stochastic parameters of this stochast
             */
            std::shared_ptr<Stochast> VariableSource = nullptr;

            /**
             * \brief Indicates whether tha variable source is allowed with respect to array definitions
             */
            bool IsVariableSourceAllowed(int arrayIndex);

            /**
             * \brief In case of a variable stochast, the stochast of which the x-value is used to define the stochastic parameters of this stochast
             * \returns Variable source
             */
            std::shared_ptr<Stochast> getVariableSource();

            /**
             * \brief In case of a variable stochast, the interpolation table to convert from x-value of the other stochast to the stochastic parameters of this stochast
             */
            std::shared_ptr<VariableStochastValuesSet> ValueSet = std::make_shared<VariableStochastValuesSet>();

            /**
             * \brief Indicates whether this stochast or, in case of a composite stochast, one of its contributing stochasts is a variable stochast
             */
            bool isVariable() override;

            /**
             * \brief In case of an array, the stochasts in the array
             */
            std::vector<std::shared_ptr<Stochast>> ArrayVariables;

            /**
             * \brief Gets a realization of a variable stochast
             * \param x X-value at which the variable stochast is generated
             */
            std::shared_ptr<Stochast> getVariableStochast(double x);

            /**
             * \brief The design factor used for calculating the design value
             */
            double designFactor = 1.0;

            /**
             * \brief The design fraction used for calculating the design value
             */
            double designQuantile = 0.5;

            /**
             * \brief Calculates the design value
             */
            double getDesignValue();

            /**
             * \brief Updates the stochast properties by setting the design value
             */
            void setDesignValue(double designValue);

            /**
             * \brief Indicates that internally an update should take place before other methods are invoked
             */
            void SetDirty() const;

            /**
             * \brief Copies all values from another stochast
             */
            void copyFrom(std::shared_ptr<Stochast> source);

            static ConstantParameterType getConstantParameterType(std::string distributionType);
            static std::string getConstantParameterTypeString(ConstantParameterType distributionType);
            static DistributionType getDistributionType(const std::string& distributionType);
            static std::string getDistributionTypeString(DistributionType distributionType);
        };
    }
}

