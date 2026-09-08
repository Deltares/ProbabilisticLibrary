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
#include "../../Uncertainty/UncertaintyResult.h"

namespace Deltares::Server
{
    /**
     * \brief Handles properties and methods of class ModelInputParameter
     */
    class UncertaintyResultHandler : public StoredObjectHandler<Uncertainty::UncertaintyResult>
    {
    public:

        ObjectType GetObjectType() override
        {
            return ObjectType::UncertaintyResult;
        }

        int GetIntValue(const std::shared_ptr<Uncertainty::UncertaintyResult>& result, const std::string& property_) override
        {
            if (property_ == "evaluations_count") return static_cast<int>(result->evaluations.size());
            else if (property_ == "quantile_evaluations_count") return static_cast<int>(result->quantileEvaluations.size());
            else if (property_ == "messages_count") return static_cast<int>(result->messages.size());
            else return StoredObjectHandler::GetIntValue(result, property_);
        }

        int GetIdValue(const std::shared_ptr<Uncertainty::UncertaintyResult>& result, const std::string& property_) override
        {
            if (property_ == "variable") return stochastHandler->GetObjectId(result->stochast);
            else return StoredObjectHandler::GetIdValue(result, property_);
        }

        std::string GetStringValue(const std::shared_ptr<Uncertainty::UncertaintyResult>& result, const std::string& property_) override
        {
            if (property_ == "identifier") return result->getIdentifier();
            else return StoredObjectHandler::GetStringValue(result, property_);
        }

        int GetIndexedIdValue(const std::shared_ptr<Uncertainty::UncertaintyResult>& result, const std::string& property_, int index) override
        {
            if (property_ == "evaluations") return evaluationHandler->GetObjectId(result->evaluations[index]);
            else if (property_ == "quantile_evaluations") return evaluationHandler->GetObjectId(result->quantileEvaluations[index]);
            else if (property_ == "messages") return messageHandler->GetObjectId(result->messages[index]);
            else return StoredObjectHandler::GetIndexedIdValue(result, property_, index);
        }

        StochastHandler* stochastHandler = nullptr;
        EvaluationHandler* evaluationHandler = nullptr;
        MessageHandler* messageHandler = nullptr;
    };
}

