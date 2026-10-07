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

#include "ModelParameterHandler.h"
#include "StoredObjectHandler.h"
#include "../../Optimization/SearchParameterSettings.h"

namespace Deltares::Server
{
    /**
     * \brief Handles properties and methods of class StochastSettings
     */
    class SearchParameterSettingsHandler : public StoredObjectHandler<Optimization::SearchParameterSettings>
    {
    public:

        ObjectType GetObjectType() override
        {
            return ObjectType::SearchParameterSettings;
        }

        double GetValue(const std::shared_ptr<Optimization::SearchParameterSettings>& settings, const std::string& property_) override
        {
            if (property_ == "min_value") return settings->MinValue;
            else if (property_ == "max_value") return settings->MaxValue;
            else if (property_ == "start_value") return settings->StartValue;
            else if (property_ == "gradient_step_size") return settings->GradientStepSize;
            else return StoredObjectHandler::GetValue(settings, property_);
        }

        void SetValue(const std::shared_ptr<Optimization::SearchParameterSettings>& settings, const std::string& property_, double value) override
        {
            if (property_ == "min_value") settings->MinValue = value;
            else if (property_ == "max_value") settings->MaxValue = value;
            else if (property_ == "start_value") settings->StartValue = value;
            else if (property_ == "gradient_step_size") settings->GradientStepSize = value;
            else StoredObjectHandler::SetValue(settings, property_, value);
        }

        int GetIntValue(const std::shared_ptr<Optimization::SearchParameterSettings>& settings, const std::string& property_) override
        {
            if (property_ == "number_of_values") return settings->NumberOfValues;
            else if (property_ == "number_of_refinements") return settings->NumberOfRefinements;
            else return StoredObjectHandler::GetIntValue(settings, property_);
        }

        int GetIdValue(const std::shared_ptr<Optimization::SearchParameterSettings>& settings, const std::string& property_) override
        {
            if (property_ == "parameter") return modelParameterHandler->GetObjectId(settings->parameter);
            else return StoredObjectHandler::GetIdValue(settings, property_);
        }

        void SetIntValue(const std::shared_ptr<Optimization::SearchParameterSettings>& settings, const std::string& property_, int value) override
        {
            if (property_ == "parameter") settings->parameter = modelParameterHandler->GetObject(value);
            else if (property_ == "number_of_values") settings->NumberOfValues = value;
            else if (property_ == "number_of_refinements") settings->NumberOfRefinements = value;
            else StoredObjectHandler::SetIntValue(settings, property_, value);
        }

        bool GetBoolValue(const std::shared_ptr<Optimization::SearchParameterSettings>& settings, const std::string& property_) override
        {
            if (property_ == "is_move_allowed") return settings->Move;
            else return StoredObjectHandler::GetBoolValue(settings, property_);
        }

        void SetBoolValue(const std::shared_ptr<Optimization::SearchParameterSettings>& settings, const std::string& property_, bool value) override
        {
            if (property_ == "is_move_allowed") settings->Move = value;
            else StoredObjectHandler::SetBoolValue(settings, property_, value);
        }

        ModelParameterHandler* modelParameterHandler = nullptr;
    };
}

