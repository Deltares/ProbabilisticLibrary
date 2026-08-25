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

namespace Deltares::Server
{
    /**
     * \brief Handles properties and methods of class ModelInputParameter
     */
    class ModelParameterHandler : public StoredObjectHandler<Models::ModelInputParameter>
    {
    public:

        ObjectType GetObjectType() override
        {
            return ObjectType::ModelParameter;
        }

        double GetValue(const std::shared_ptr<Models::ModelInputParameter>& modelParameter, const std::string& property_) override
        {
            if (property_ == "default_value") return modelParameter->defaultValue;
            else return StoredObjectHandler::GetValue(modelParameter, property_);
        }

        void SetValue(const std::shared_ptr<Models::ModelInputParameter>& modelParameter, const std::string& property_, double value) override
        {
            if (property_ == "default_value") modelParameter->defaultValue = value;
            else StoredObjectHandler::SetValue(modelParameter, property_, value);
        }

        int GetIntValue(const std::shared_ptr<Models::ModelInputParameter>& modelParameter, const std::string& property_) override
        {
            if (property_ == "index") return modelParameter->index;
            else if (property_ == "array_size") return modelParameter->arraySize;
            else return StoredObjectHandler::GetIntValue(modelParameter, property_);
        }

        void SetIntValue(const std::shared_ptr<Models::ModelInputParameter>& modelParameter, const std::string& property_, int value) override
        {
            if (property_ == "index") modelParameter->index = value;
            else if (property_ == "array_size") modelParameter->arraySize = value;
            else StoredObjectHandler::SetIntValue(modelParameter, property_, value);
        }

        bool GetBoolValue(const std::shared_ptr<Models::ModelInputParameter>& modelParameter, const std::string& property_) override
        {
            if (property_ == "is_array") return modelParameter->isArray;
            else return StoredObjectHandler::GetBoolValue(modelParameter, property_);
        }

        void SetBoolValue(const std::shared_ptr<Models::ModelInputParameter>& modelParameter, const std::string& property_, bool value) override
        {
            if (property_ == "is_array") modelParameter->isArray = value;
            else StoredObjectHandler::SetBoolValue(modelParameter, property_, value);
        }

        std::string GetStringValue(const std::shared_ptr<Models::ModelInputParameter>& modelParameter, const std::string& property_) override
        {
            if (property_ == "name") return modelParameter->name;
            else return StoredObjectHandler::GetStringValue(modelParameter, property_);
        }

        void SetStringValue(const std::shared_ptr<Models::ModelInputParameter>& modelParameter, const std::string& property_, const std::string& value) override
        {
            if (property_ == "name") modelParameter->name = value;
            else StoredObjectHandler::SetStringValue(modelParameter, property_, value);
        }
    };
}

