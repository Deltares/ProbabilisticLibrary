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
#include "DesignPointHandler.h"
#include "SensitivityValueHandler.h"
#include "../../Sensitivity/SensitivityResult.h"

namespace Deltares::Server
{
    /**
     * \brief Handles properties and methods of class SensitivityResult
     */
    class SensitivityResultHandler : public StoredObjectHandler<Sensitivity::SensitivityResult>
    {
    public:

        ObjectType GetObjectType() override
        {
            return ObjectType::SensitivityResult;
        }

        int GetIntValue(const std::shared_ptr<Sensitivity::SensitivityResult>& result, const std::string& property_) override
        {
            if (property_ == "values_count") return static_cast<int>(result->values.size());
            else if (property_ == "evaluations_count") return static_cast<int>(result->evaluations.size());
            else if (property_ == "messages_count") return static_cast<int>(result->messages.size());
            else return StoredObjectHandler::GetIntValue(result, property_);
        }

        std::string GetStringValue(const std::shared_ptr<Sensitivity::SensitivityResult>& result, const std::string& property_) override
        {
            if (property_ == "identifier") return result->identifier;
            else return StoredObjectHandler::GetStringValue(result, property_);
        }

        void SetStringValue(const std::shared_ptr<Sensitivity::SensitivityResult>& result, const std::string& property_, const std::string& value) override
        {
            if (property_ == "identifier") result->identifier = value;
            else return StoredObjectHandler::SetStringValue(result, property_, value);
        }

        int GetIndexedIdValue(const std::shared_ptr<Sensitivity::SensitivityResult>& result, const std::string& property_, int index) override
        {
            if (property_ == "values") return sensitivityValueHandler->GetObjectId(result->values[index]);
            else if (property_ == "evaluations") return evaluationHandler->GetObjectId(result->evaluations[index]);
            else if (property_ == "messages") return messageHandler->GetObjectId(result->messages[index]);
            else return StoredObjectHandler::GetIndexedIdValue(result, property_, index);
        }

        SensitivityValueHandler* sensitivityValueHandler = nullptr;
        EvaluationHandler* evaluationHandler = nullptr;
        MessageHandler* messageHandler = nullptr;
    };
}

