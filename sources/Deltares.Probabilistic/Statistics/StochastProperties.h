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

#include <functional>

#include "DiscreteValue.h"
#include "HistogramValue.h"
#include "FragilityValue.h"
#include "../Utils/probLibException.h"
#include "DistributionPropertyType.h"

#include <vector>
#include <memory>

#include "ContributingStochast.h"

namespace Deltares
{
    namespace Statistics
    {
        /**
         * \brief Defines the stochastic parameters of a stochastic variable
         * \remark Mean and standard deviation are not stochastic parameters, but are derived based on these stochastic parameters and a distribution type
         */
        class StochastProperties
        {

        public:
            /**
             * \brief Location, for normal distribution equal to the mean
             */
            double Location = 0;

            /**
             * \brief Scale, for normal distribution equal to the standard deviation
             */
            double Scale = 0;

            /**
             * \brief Minimum allowed value
             */
            double Minimum = 0;

            /**
             * \brief Maximum allowed value
             */
            double Maximum = 0;

            /**
             * \brief Value by which the stochast is shifted
             */
            double Shift = 0;

            /**
             * \brief Secondary value by which the stochast is shifted
             */
            double ShiftB = 0;

            /**
             * \brief Shape, defines the shape of the distribution
             */
            double Shape = 1;

            /**
             * \brief Secondary shape, defines the shape of the distribution
             */
            double ShapeB = 1;

            /**
             * \brief Number of observations 
             */
            int Observations = 2;

            /**
             * \brief Collection of discrete values
             */
            std::vector<std::shared_ptr<DiscreteValue>> DiscreteValues;

            /**
             * \brief Collection of bins in a histogram distribution
             */
            std::vector<std::shared_ptr<HistogramValue>> HistogramValues;

            /**
             * \brief Collection of CDF values
             */
            std::vector<std::shared_ptr<FragilityValue>> FragilityValues;

            /**
             * \brief Collection of contributing stochasts
             */
            std::vector<std::shared_ptr<ContributingStochast>> ContributingStochasts;

            /**
             * \brief General method to apply a value to one of the stochastic parameters
             * \param property Stochastic parameter type
             * \param value Value to be applied
             */
            void applyValue(DistributionPropertyType property, double value)
            {
                switch (property)
                {
                    case DistributionPropertyType::Location: this->Location = value; break;
                    case DistributionPropertyType::Scale: this->Scale = value; break;
                    case DistributionPropertyType::Minimum:    this->Minimum = value; break;
                    case DistributionPropertyType::Maximum:    this->Maximum = value; break;
                    case DistributionPropertyType::Shift: this->Shift = value; break;
                    case DistributionPropertyType::ShiftB: this->ShiftB = value; break;
                    case DistributionPropertyType::Shape: this->Shape = value; break;
                    case DistributionPropertyType::ShapeB: this->ShapeB = value; break;
                    case DistributionPropertyType::Observations: this->Observations = (int)value; break;
                    default: throw Reliability::probLibException("Property not supported");
                }
            }

            /**
             * \brief Makes a copy of the stochastic properties
             * \remark Discrete values, histogram values and CDF values are omitted
             * \return Copy
             */
            std::shared_ptr<StochastProperties> clone()
            {
                std::shared_ptr<StochastProperties> clone = std::make_shared<StochastProperties>();

                clone->Location = this->Location;
                clone->Scale = this->Scale;
                clone->Minimum = this->Minimum;
                clone->Maximum = this->Maximum;
                clone->Shift = this->Shift;
                clone->ShiftB = this->ShiftB;
                clone->Shape = this->Shape;
                clone->ShapeB = this->ShapeB;
                clone->Observations = this->Observations;

                return clone;
            }

            void copyFrom(const std::shared_ptr<StochastProperties> source)
            {
                this->Location = source->Location;
                this->Scale = source->Scale;
                this->Minimum = source->Minimum;
                this->Maximum = source->Maximum;
                this->Shift = source->Shift;
                this->ShiftB = source->ShiftB;
                this->Shape = source->Shape;
                this->ShapeB = source->ShapeB;
                this->Observations = source->Observations;

                this->HistogramValues.clear();
                for (std::shared_ptr<HistogramValue> histogramValue : source->HistogramValues)
                {
                    this->HistogramValues.push_back(histogramValue->clone());
                }

                this->FragilityValues.clear();
                for (std::shared_ptr<FragilityValue> fragilityValue : source->FragilityValues)
                {
                    this->FragilityValues.push_back(fragilityValue->clone());
                }

                this->DiscreteValues.clear();
                for (std::shared_ptr<DiscreteValue> discreteValue : source->DiscreteValues)
                {
                    this->DiscreteValues.push_back(discreteValue->clone());
                }

                this->ContributingStochasts.clear();
                for (std::shared_ptr<ContributingStochast> contributingStochast : source->ContributingStochasts)
                {
                    this->ContributingStochasts.push_back(contributingStochast->clone());
                }
            }

            /**
             * \brief Indicates whether InitializeForRun should be invoked before performing other methods
             * \return Indication
             */
            bool dirty = false;

            /**
             * \brief Indicates whether InitializeForRun should be invoked before performing other methods
             * \return Indication
             */
            void setDirty()
            {
                dirty = true;

                for (std::shared_ptr<Statistics::HistogramValue> histogramValue : this->HistogramValues)
                {
                    histogramValue->setDirtyFunction(nullptr);
                }

                for (std::shared_ptr<Statistics::DiscreteValue> discreteValue : this->DiscreteValues)
                {
                    discreteValue->setDirtyFunction(nullptr);
                }

                for (std::shared_ptr<Statistics::FragilityValue> fragilityValue : this->FragilityValues)
                {
                    fragilityValue->setDirtyFunction(nullptr);
                }
            }
        };
    }
}
