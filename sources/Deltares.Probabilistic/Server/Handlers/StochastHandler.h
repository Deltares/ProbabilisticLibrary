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

#include <string>

#include "ConditionalValueHandler.h"
#include "ContributingStochastHandler.h"
#include "DiscreteValueHandler.h"
#include "FragilityValueHandler.h"
#include "HistogramValueHandler.h"
#include "StoredObjectHandler.h"
#include "ValidationReportHandler.h"
#include "../../Statistics/Stochast.h"

namespace Deltares::Server
{
    /**
     * \brief Handles properties and methods of class Stochast
     */
    class StochastHandler : public StoredObjectHandler<Statistics::Stochast>
    {
    public:

        ObjectType GetObjectType() override
        {
            return ObjectType::Stochast;
        }

        double GetValue(const std::shared_ptr<Statistics::Stochast>& stochast, const std::string& property_) override
        {
            if (property_ == "location") return stochast->getProperties()->Location;
            else if (property_ == "scale") return stochast->getProperties()->Scale;
            else if (property_ == "shape") return stochast->getProperties()->Shape;
            else if (property_ == "shape_b") return stochast->getProperties()->ShapeB;
            else if (property_ == "shift") return stochast->getProperties()->Shift;
            else if (property_ == "shift_b") return stochast->getProperties()->ShiftB;
            else if (property_ == "minimum") return stochast->getProperties()->Minimum;
            else if (property_ == "maximum") return stochast->getProperties()->Maximum;
            else if (property_ == "mean") return stochast->getMean();
            else if (property_ == "deviation") return stochast->getDeviation();
            else if (property_ == "variation") return stochast->getVariation();
            else if (property_ == "design_quantile") return stochast->designQuantile;
            else if (property_ == "design_factor") return stochast->designFactor;
            else if (property_ == "design_value") return stochast->getDesignValue();
            else if (property_ == "ks_test") return stochast->getKSTest(tempValues["data"]);
            else if (property_ == "x_from_u_and_source") return stochast->getXFromUAndSource(tempValues["u_and_x"][1], tempValues["u_and_x"][0]);
            else if (property_ == "u_from_x_and_source") return stochast->getUFromXAndSource(tempValues["x_and_source"][1], tempValues["x_and_source"][0]);
            else return StoredObjectHandler::GetValue(stochast, property_);
        }

        void SetValue(const std::shared_ptr<Statistics::Stochast>& stochast, const std::string& property_, double value) override
        {
            if (property_ == "location") stochast->getProperties()->Location = value;
            else if (property_ == "scale") stochast->getProperties()->Scale = value;
            else if (property_ == "shape") stochast->getProperties()->Shape = value;
            else if (property_ == "shape_b") stochast->getProperties()->ShapeB = value;
            else if (property_ == "shift") stochast->getProperties()->Shift = value;
            else if (property_ == "set_shift") stochast->setShift(value);
            else if (property_ == "shift_b") stochast->getProperties()->ShiftB = value;
            else if (property_ == "minimum") stochast->getProperties()->Minimum = value;
            else if (property_ == "maximum") stochast->getProperties()->Maximum = value;
            else if (property_ == "mean") stochast->setMean(value);
            else if (property_ == "deviation") stochast->setDeviation(value);
            else if (property_ == "variation") stochast->setVariation(value);
            else if (property_ == "design_quantile") stochast->designQuantile = value;
            else if (property_ == "design_factor") stochast->designFactor = value;
            else if (property_ == "design_value") stochast->setDesignValue(value);
            else if (property_ == "shift_for_fit") argValue = value;
            else if (property_ == "design_point_x") argValue = value;
            else if (property_ == "conditional_x") argValue = value;
            else StoredObjectHandler::SetValue(stochast, property_, value);
        }

        int GetIntValue(const std::shared_ptr<Statistics::Stochast>& stochast, const std::string& property_) override
        {
            if (property_ == "observations") return stochast->getProperties()->Observations;
            else if (property_ == "array_size") return stochast->modelParameter->arraySize;
            else if (property_ == "histogram_values_count") return static_cast<int>(stochast->getProperties()->HistogramValues.size());
            else if (property_ == "discrete_values_count") return static_cast<int>(stochast->getProperties()->DiscreteValues.size());
            else if (property_ == "fragility_values_count") return static_cast<int>(stochast->getProperties()->FragilityValues.size());
            else if (property_ == "contributing_stochasts_count") return static_cast<int>(stochast->getProperties()->ContributingStochasts.size());
            else if (property_ == "conditional_values_count") return static_cast<int>(stochast->ValueSet->StochastValues.size());
            else if (property_ == "array_variables_count") return static_cast<int>(stochast->ArrayVariables.size());
            else if (property_ == "special_values_count")
            {
                tempValues["special_values"] = stochast->getSpecialXValues();
                return static_cast<int>(tempValues["special_values"].size());
            }
            else return StoredObjectHandler::GetIntValue(stochast, property_);
        }

        int GetIdValue(const std::shared_ptr<Statistics::Stochast>& stochast, const std::string& property_) override
        {
            if (property_ == "conditional_source") return GetObjectId(stochast->VariableSource);
            else if (property_ == "validate") return validationReportHandler->GetObjectId(std::make_shared<Logging::ValidationReport>(stochast->getValidationReport()));
            else if (property_ == "validate_fit")
            {
                std::shared_ptr<Statistics::Stochast> prior = GetObject(tempIntValue);
                Logging::ValidationReport report = stochast->getFitValidationReport(tempValues["data"], prior, argValue);
                if (!report.isValid())
                {
                    tempValues.erase("data");
                    tempIntValue = 0;
                    argValue = std::nan("");
                }
                return validationReportHandler->GetObjectId(std::make_shared<Logging::ValidationReport>(report));
            }
            else if (property_ == "conditional_x")
            {
                double x = argValue;
                argValue = std::nan("");

                std::shared_ptr<Statistics::Stochast> conditionalStochast = stochast->getVariableStochast(x);
                return GetObjectId(conditionalStochast);
            }
            else return StoredObjectHandler::GetIdValue(stochast, property_);
        }

        void SetIntValue(const std::shared_ptr<Statistics::Stochast>& stochast, const std::string& property_, int value) override
        {
            if (property_ == "observations") stochast->getProperties()->Observations = value;
            else if (property_ == "array_size") stochast->modelParameter->arraySize = value;
            else if (property_ == "copy_from") stochast->copyFrom(GetObject(value));
            else if (property_ == "conditional_source") stochast->VariableSource = GetObject(value);
            else if (property_ == "add_histogram_value") stochast->getProperties()->HistogramValues.push_back(histogramValueHandler->GetObject(value));
            else if (property_ == "add_fragility_value") stochast->getProperties()->FragilityValues.push_back(fragilityValueHandler->GetObject(value));
            else if (property_ == "add_discrete_value") stochast->getProperties()->DiscreteValues.push_back(discreteValueHandler->GetObject(value));
            else if (property_ == "add_contributing_stochast") stochast->getProperties()->ContributingStochasts.push_back(contributingStochastHandler->GetObject(value));
            else if (property_ == "add_conditional_value") stochast->ValueSet->StochastValues.push_back(conditionalValueHandler->GetObject(value));
            else if (property_ == "prior") tempIntValue = value;
            else StoredObjectHandler::SetIntValue(stochast, property_, value);
        }

        bool GetBoolValue(const std::shared_ptr<Statistics::Stochast>& stochast, const std::string& property_) override
        {
            if (property_ == "inverted") return stochast->isInverted();
            else if (property_ == "truncated") return stochast->isTruncated();
            else if (property_ == "conditional") return stochast->IsVariableStochast;
            else if (property_ == "can_fit") return stochast->canFit(false, false);
            else if (property_ == "can_fit_prior") return stochast->canFit(false, true);
            else if (property_ == "can_truncate") return stochast->canTruncate();
            else if (property_ == "can_invert") return stochast->canInvert();
            else if (property_ == "is_array") return stochast->modelParameter->isArray;
            else if (property_ == "is_varying") return stochast->isVarying();
            else if (property_ == "is_qualitative") return stochast->isQualitative();
            else if (property_ == "is_valid") return stochast->isValid();
            else if (property_ == "is_used_mean") return true;
            else if (property_ == "is_used_deviation") return stochast->getDistributionType() != Statistics::DistributionType::Deterministic;
            else if (property_ == "is_used_location") return stochast->hasParameter(Statistics::DistributionPropertyType::Location);
            else if (property_ == "is_used_scale") return stochast->hasParameter(Statistics::DistributionPropertyType::Scale);
            else if (property_ == "is_used_minimum") return stochast->hasParameter(Statistics::DistributionPropertyType::Minimum);
            else if (property_ == "is_used_maximum") return stochast->hasParameter(Statistics::DistributionPropertyType::Maximum);
            else if (property_ == "is_used_shift") return stochast->hasParameter(Statistics::DistributionPropertyType::Shift);
            else if (property_ == "is_used_shift_b") return stochast->hasParameter(Statistics::DistributionPropertyType::ShiftB);
            else if (property_ == "is_used_shape") return stochast->hasParameter(Statistics::DistributionPropertyType::Shape);
            else if (property_ == "is_used_shape_b") return stochast->hasParameter(Statistics::DistributionPropertyType::ShapeB);
            else if (property_ == "is_used_observations") return stochast->hasParameter(Statistics::DistributionPropertyType::Observations);
            else return StoredObjectHandler::GetBoolValue(stochast, property_);
        }

        void SetBoolValue(const std::shared_ptr<Statistics::Stochast>& stochast, const std::string& property_, bool value) override
        {
            if (property_ == "inverted") stochast->setInverted(value);
            else if (property_ == "truncated") stochast->setTruncated(value);
            else if (property_ == "conditional") stochast->IsVariableStochast = value;
            else if (property_ == "is_array") stochast->modelParameter->isArray = value;
            else StoredObjectHandler::SetBoolValue(stochast, property_, value);
        }

        std::string GetStringValue(const std::shared_ptr<Statistics::Stochast>& stochast, const std::string& property_) override
        {
            if (property_ == "distribution") return Statistics::Stochast::getDistributionTypeString(stochast->getDistributionType());
            else if (property_ == "constant_parameter") return Statistics::Stochast::getConstantParameterTypeString(stochast->constantParameterType);
            else if (property_ == "name") return stochast->name;
            else return StoredObjectHandler::GetStringValue(stochast, property_);
        }

        void SetStringValue(const std::shared_ptr<Statistics::Stochast>& stochast, const std::string& property_, const std::string& value) override
        {
            if (property_ == "distribution") stochast->setDistributionType(Statistics::Stochast::getDistributionType(value));
            else if (property_ == "constant_parameter") stochast->constantParameterType = Statistics::Stochast::getConstantParameterType(value);
            else if (property_ == "name") stochast->name = value;
            else StoredObjectHandler::SetStringValue(stochast, property_, value);
        }

        void SetArrayValue(const std::shared_ptr<Statistics::Stochast>& stochast, const std::string& property_, double* values, int size) override
        {
            std::vector<double> dataValues(size);
            for (int i = 0; i < size; i++)
            {
                dataValues[i] = values[i];
            }

            if (property_ == "fit") tempValues["data"] = dataValues;
            else if (property_ == "data") tempValues["data"] = dataValues;
            else if (property_ == "weights") tempValues["weights"] = dataValues;
            else if (property_ == "u_and_x") tempValues["u_and_x"] = dataValues;
            else if (property_ == "x_and_source") tempValues["x_and_source"] = dataValues;
            else StoredObjectHandler::SetArrayValue(stochast, property_, values, size);
        }

        void SetArrayIntValue(const std::shared_ptr<Statistics::Stochast>& stochast, const std::string& property_, int* values, int size) override
        {
            if (property_ == "discrete_values")
            {
                stochast->getProperties()->setDirty();
                stochast->getProperties()->DiscreteValues.clear();
                for (int i = 0; i < size; i++)
                {
                    stochast->getProperties()->DiscreteValues.push_back(discreteValueHandler->GetObject(values[i]));
                }
            }
            else if (property_ == "histogram_values")
            {
                stochast->getProperties()->setDirty();
                stochast->getProperties()->HistogramValues.clear();
                for (int i = 0; i < size; i++)
                {
                    stochast->getProperties()->HistogramValues.push_back(histogramValueHandler->GetObject(values[i]));
                }
            }
            else if (property_ == "fragility_values")
            {
                stochast->getProperties()->setDirty();
                stochast->getProperties()->FragilityValues.clear();
                for (int i = 0; i < size; i++)
                {
                    stochast->getProperties()->FragilityValues.push_back(fragilityValueHandler->GetObject(values[i]));
                }
            }
            else if (property_ == "contributing_stochasts")
            {
                stochast->getProperties()->setDirty();
                stochast->getProperties()->ContributingStochasts.clear();
                for (int i = 0; i < size; i++)
                {
                    stochast->getProperties()->ContributingStochasts.push_back(contributingStochastHandler->GetObject(values[i]));
                }
            }
            else if (property_ == "conditional_values")
            {
                stochast->ValueSet->StochastValues.clear();
                for (int i = 0; i < size; i++)
                {
                    stochast->ValueSet->StochastValues.push_back(conditionalValueHandler->GetObject(values[i]));
                }
            }
            else if (property_ == "array_variables")
            {
                stochast->ArrayVariables.clear();
                for (int i = 0; i < size; i++)
                {
                    stochast->ArrayVariables.push_back(GetObject(values[i]));
                }
            }
            else StoredObjectHandler::SetArrayIntValue(stochast, property_, values, size);
        }

        double GetArgValue(const std::shared_ptr<Statistics::Stochast>& stochast, const std::string& property_, double argument) override
        {
            if (property_ == "quantile") return stochast->getQuantile(argument);
            else if (property_ == "x_from_u") return stochast->getXFromU(argument);
            else if (property_ == "u_from_x") return stochast->getUFromX(argument);
            else if (property_ == "x_from_p") return stochast->getXFromP(argument);
            else if (property_ == "p_from_x") return stochast->getPFromX(argument);
            else if (property_ == "pdf") return stochast->getPDF(argument);
            else if (property_ == "cdf") return stochast->getCDF(argument);
            else return StoredObjectHandler::GetArgValue(stochast, property_, argument);
        }

        void SetArgValue(const std::shared_ptr<Statistics::Stochast>& stochast, const std::string& property_, double argument, double value) override
        {
            if (property_ == "x_at_u") stochast->setXAtU(value, argument, Statistics::ConstantParameterType::VariationCoefficient);
        }

        double GetIndexedValue(const std::shared_ptr<Statistics::Stochast>& stochast, const std::string& property_, int index) override
        {
            if (property_ == "special_values") return tempValues["special_values"][index];
            else return StoredObjectHandler::GetIndexedValue(stochast, property_, index);
        }

        int GetIndexedIdValue(const std::shared_ptr<Statistics::Stochast>& stochast, const std::string& property_, int index) override
        {
            if (property_ == "histogram_values") return histogramValueHandler->GetObjectId(stochast->getProperties()->HistogramValues[index]);
            else if (property_ == "discrete_values") return discreteValueHandler->GetObjectId(stochast->getProperties()->DiscreteValues[index]);
            else if (property_ == "fragility_values") return fragilityValueHandler->GetObjectId(stochast->getProperties()->FragilityValues[index]);
            else if (property_ == "contributing_stochasts") return contributingStochastHandler->GetObjectId(stochast->getProperties()->ContributingStochasts[index]);
            else if (property_ == "conditional_values") return conditionalValueHandler->GetObjectId(stochast->ValueSet->StochastValues[index]);
            else if (property_ == "array_variables") return GetObjectId(stochast->ArrayVariables[index]);
            else return StoredObjectHandler::GetIndexedIdValue(stochast, property_, index);
        }

        void Execute(const std::shared_ptr<Statistics::Stochast>& stochast, const std::string& method_) override
        {
            if (method_ == "initialize_for_run") stochast->initializeForRun();
            else if (method_ == "initialize_conditional_values") stochast->initializeConditionalValues();
            else if (method_ == "set_x_at_u_dev") stochast->setXAtU(tempValues["u_and_x"][1], tempValues["u_and_x"][0], Statistics::ConstantParameterType::Deviation);
            else if (method_ == "set_x_at_u_var") stochast->setXAtU(tempValues["u_and_x"][1], tempValues["u_and_x"][0], Statistics::ConstantParameterType::VariationCoefficient);
            else if (method_ == "fit")
            {
                double shift = argValue;

                stochast->fit(tempValues["data"], shift);

                argValue = nan("");
                tempValues.erase("data");
            }
            else if (method_ == "fit_weighted")
            {
                stochast->fitWeighted(tempValues["data"], tempValues["weights"]);

                tempValues.erase("data");
                tempValues.erase("weights");
            }
            else if (method_ == "fit_prior")
            {
                double shift = argValue;

                stochast->fitPrior(tempValues["data"], GetObject(tempIntValue), shift);

                tempIntValue = 0;
                argValue = nan("");
                tempValues.erase("data");
            }
            else
            {
                StoredObjectHandler::Execute(stochast, method_);
            }
        }

        ValidationReportHandler* validationReportHandler = nullptr;
        HistogramValueHandler* histogramValueHandler = nullptr;
        DiscreteValueHandler* discreteValueHandler = nullptr;
        FragilityValueHandler* fragilityValueHandler = nullptr;
        ConditionalValueHandler* conditionalValueHandler = nullptr;
        ContributingStochastHandler* contributingStochastHandler = nullptr;

    private:
        std::unordered_map <std::string, std::vector<double>> tempValues;
        double argValue = nan("");
        int tempIntValue = 0;
    };
}

