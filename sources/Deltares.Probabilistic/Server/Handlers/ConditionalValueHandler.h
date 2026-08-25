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
#include "StoredObjectHandler.h"
#include "../../Model/ModelInputParameter.h"
#include "../../Statistics/VariableStochastValue.h"

namespace Deltares::Server
{
    /**
     * \brief Handles properties and methods of class VariableStochastValue
     */
    class ConditionalValueHandler : public StoredObjectHandler<Statistics::VariableStochastValue>
    {
    public:

        ObjectType GetObjectType() override
        {
            return ObjectType::ConditionalValue;
        }

        double GetValue(const std::shared_ptr<Statistics::VariableStochastValue>& conditionalValue, const std::string& property_) override
        {
            if (property_ == "x") return conditionalValue->X;
            else if (property_ == "location") return conditionalValue->Stochast->Location;
            else if (property_ == "scale") return conditionalValue->Stochast->Scale;
            else if (property_ == "shape") return conditionalValue->Stochast->Shape;
            else if (property_ == "shape_b") return conditionalValue->Stochast->ShapeB;
            else if (property_ == "shift") return conditionalValue->Stochast->Shift;
            else if (property_ == "shift_b") return conditionalValue->Stochast->ShiftB;
            else if (property_ == "minimum") return conditionalValue->Stochast->Minimum;
            else if (property_ == "maximum") return conditionalValue->Stochast->Maximum;
            else if (property_ == "mean") return conditionalValue->mean;
            else if (property_ == "deviation") return conditionalValue->deviation;
            else return StoredObjectHandler::GetValue(conditionalValue, property_);
        }

        void SetValue(const std::shared_ptr<Statistics::VariableStochastValue>& conditionalValue, const std::string& property_, double value) override
        {
            if (property_ == "x") conditionalValue->X = value;
            else if (property_ == "location") conditionalValue->Stochast->Location = value;
            else if (property_ == "scale") conditionalValue->Stochast->Scale = value;
            else if (property_ == "shape") conditionalValue->Stochast->Shape = value;
            else if (property_ == "shape_b") conditionalValue->Stochast->ShapeB = value;
            else if (property_ == "shift") conditionalValue->Stochast->Shift = value;
            else if (property_ == "shift_b") conditionalValue->Stochast->ShiftB = value;
            else if (property_ == "minimum") conditionalValue->Stochast->Minimum = value;
            else if (property_ == "maximum") conditionalValue->Stochast->Maximum = value;
            else if (property_ == "mean") conditionalValue->mean = value;
            else if (property_ == "deviation") conditionalValue->deviation = value;
            else StoredObjectHandler::SetValue(conditionalValue, property_, value);
        }

        int GetIntValue(const std::shared_ptr<Statistics::VariableStochastValue>& conditionalValue, const std::string& property_) override
        {
            if (property_ == "observations") return conditionalValue->Stochast->Observations;
            else return StoredObjectHandler::GetIntValue(conditionalValue, property_);
        }

        void SetIntValue(const std::shared_ptr<Statistics::VariableStochastValue>& conditionalValue, const std::string& property_, int value) override
        {
            if (property_ == "observations") conditionalValue->Stochast->Observations = value;
            else StoredObjectHandler::SetIntValue(conditionalValue, property_, value);
        }

        bool GetBoolValue(const std::shared_ptr<Statistics::VariableStochastValue>& conditionalValue, const std::string& property_) override
        {
            if (property_ == "is_array") return conditionalValue->isArray;
            else return StoredObjectHandler::GetBoolValue(conditionalValue, property_);
        }

        void SetBoolValue(const std::shared_ptr<Statistics::VariableStochastValue>& conditionalValue, const std::string& property_, bool value) override
        {
            if (property_ == "is_array") conditionalValue->isArray = value;
            else StoredObjectHandler::SetBoolValue(conditionalValue, property_, value);
        }

        std::string GetStringValue(const std::shared_ptr<Statistics::VariableStochastValue>& conditionalValue, const std::string& property_) override
        {
            if (property_ == "name") return conditionalValue->name;
            else return StoredObjectHandler::GetStringValue(conditionalValue, property_);
        }

        void SetStringValue(const std::shared_ptr<Statistics::VariableStochastValue>& conditionalValue, const std::string& property_, const std::string& value) override
        {
            if (property_ == "name") conditionalValue->name = value;
            else StoredObjectHandler::SetStringValue(conditionalValue, property_, value);
        }
    };
}

