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

namespace Deltares
{
    namespace Statistics
    {
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
            Stochast(DistributionType distributionType, std::vector<double> values);

            /**
             * \brief Constructor with object containing the stochastic parameters
             * \param distributionType Distribution type
             * \param properties Object containing stochastic parameters
             */
            Stochast(DistributionType distributionType, std::shared_ptr<StochastProperties> properties);

            /**
             * \brief identifying string for the user
             */
            std::string name = "";

            /**
             * \brief Gets the stochastic properties
             * \return Object containing the stochastic properties
             */
            std::shared_ptr<StochastProperties> getProperties()    { return properties; }

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
            double getQuantile(double quantile);

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
             * \param source Other stochast
             * \param u Given u-value
             * \return x-value
             */
            double getXFromUAndSource(double source, double u);

            /**
             * \brief Modifies the stochast so that a given x-value is returned for a given u-value
             * \param x Given x-value
             * \param u Given u-value
             * \param constantType Characteristic value of a stochast, which should be kept constant while applying this value. Can be variation coefficient or standard deviation.
             */
            void setXAtU(double x, double u, ConstantParameterType constantType);

            /**
             * \brief Sets the distribution type
             * \remark Internally, the Distribution field is updated
             * \param distributionType Distribution type
             */
            void setDistributionType(DistributionType distributionType);

            /**
             * \brief Gets the distribution type
             * \return Distribution type
             */
            DistributionType getDistributionType();

            /**
             * \brief Sets a function which provides the u->x conversion. Overrules the distribution type
             * \param externalFunction Function
             */
            void setExternalDistribution(UXLambda externalFunction);

            /**
             * \brief Indicates whether the distribution can be inverted
             * \return Indication
             */
            bool canInvert();

            /**
             * \brief Indicates whether the distribution is inverted
             * \return Indication
             */
            bool isInverted();

            /**
             * \brief Inverts or non-inverts the distribution
             * \param inverted Inverts (true) or non-inverts (false) the distribution
             */
            void setInverted(bool inverted);

            /**
             * \brief Indicates whether the distribution can be truncated
             * \return Indication
             */
            bool canTruncate();

            /**
             * \brief Indicates whether the distribution is truncated
             * \return Indication
             */
            bool isTruncated();

            /**
             * \brief Truncates or non-truncates the distribution
             * \param truncated Truncates (true) or non-truncates (false) the distribution
             */
            void setTruncated(bool truncated);

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
            bool isQualitative();

            /**
             * \brief Gets the representative u-value for a given u-value
             * \remark When several u-values lead to the same x-value, their representative u-value is the same
             * \param u Given u-value
             * \return Representative u-value
             */
            double getRepresentativeU(double u);

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
            double getDeviation();

            /**
             * \brief Sets the standard deviation of a stochast
             * \remark This will lead to a modification of the stochast parameters, such as the scale. The mean value will not be changed.
             * \param deviation Mean value
             */
            void setDeviation(double deviation);

            /**
             * \brief Sets the mean value and standard deviation of a stochast
             * \remark This will lead to a modification of the stochast parameters, such as the location and scale
             * \param mean Given mean value
             * \param deviation Given standard deviation
             */
            void setMeanAndDeviation(double mean, double deviation);

            /**
             * \brief Sets the shift value of a stochast
             * \remark Keeps mean and standard deviation constant when applying it to a log normal distribution
             * \param shift Shift value
             */
            void setShift(double shift);

            /**
             * \brief Prepares the stochast for fast response of u->x conversion
             */
            void initializeForRun();

            /**
             * \brief Indicates whether stochastic parameters can be estimated for a given set of x-values
             * \return Indication
             */
            bool canFit();

            /**
             * \brief Estimates stochastic parameters for a given set of x-values
             * \param values Given set of x-values
             */
            void fit(std::vector<double> values);

            /**
             * \brief Estimates stochastic parameters for a given set of x-values and their weights
             * \param values Given set of x-values
             * \param weights Given weights
             */
            void fitWeighted(std::vector<double> values, std::vector<double> weights);

            /**
             * \brief Fits the distribution properties from the histogram values in the properties
             */
            void fitFromHistogramValues();

            /**
             * \brief Gets a number of interesting x-values
             * \remark This method is used when plotting PDF and CDF
             * \return Interesting x-values
             */
            std::vector<double> getSpecialXValues() override;

            /**
             * \brief Indicates whether the stochastic parameter have valid values for the current distribution type
             * \return Indication
             */
            bool isValid() override;

            /**
             * \brief Indicates whether the stochast uses a given parameter type
             * \param distributionPropertyType Given parameter type
             * \return Indication
             */
            bool hasParameter(DistributionPropertyType distributionPropertyType);

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
             * \brief In case of a variable stochast, the interpolation table to convert from x-value of the other stochast to the stochastic parameters of this stochast
             */
            std::shared_ptr<VariableStochastValuesSet> ValueSet = std::make_shared<VariableStochastValuesSet>();

            /**
             * \brief Indicates that internally an update should take place before other methods are invoked
             */
            void SetDirty() const;

            /**
             * \brief Copies all values from another stochast
             */
            void copyFrom(std::shared_ptr<Stochast> source);

            static Statistics::DistributionType getDistributionType(std::string distributionType);
            static std::string getDistributionTypeString(Statistics::DistributionType distributionType);
        };
    }
}

