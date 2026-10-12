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

#include "EvaluationHandler.h"
#include "MessageHandler.h"
#include "StoredObjectHandler.h"
#include "../../Optimization/OptimizationResult.h"

namespace Deltares::Server
{
    /**
     * \brief Handles properties and methods of class OptimizationResult
     */
    class OptimizationResultHandler : public StoredObjectHandler<Optimization::OptimizationResult>
    {
    public:

        ObjectType GetObjectType() override
        {
            return ObjectType::OptimizationResult;
        }

        double GetValue(const std::shared_ptr<Optimization::OptimizationResult>& result, const std::string& property_) override
        {
            if (property_ == "minimum_value") return result->minimumValue;
            return StoredObjectHandler::GetValue(result, property_);
        }

        void SetValue(const std::shared_ptr<Optimization::OptimizationResult>& result, const std::string& property_, double value) override
        {
            if (property_ == "minimum_value") result->minimumValue = value;
            else StoredObjectHandler::SetValue(result, property_, value);
        }

        int GetIntValue(const std::shared_ptr<Optimization::OptimizationResult>& result, const std::string& property_) override
        {
            if (property_ == "values_count") return static_cast<int>(result->values.size());
            if (property_ == "evaluations_count") return static_cast<int>(result->evaluations.size());
            if (property_ == "messages_count") return static_cast<int>(result->messages.size());
            if (property_ == "model_runs") return result->modelRuns;
            return StoredObjectHandler::GetIntValue(result, property_);
        }

        void SetIntValue(const std::shared_ptr<Optimization::OptimizationResult>& result, const std::string& property_, int value) override
        {
            if (property_ == "model_runs") result->modelRuns = value;
            else StoredObjectHandler::SetIntValue(result, property_, value);
        }

        bool GetBoolValue(const std::shared_ptr<Optimization::OptimizationResult>& result, const std::string& property_) override
        {
            if (property_ == "succeeded") return result->succeeded;
            return StoredObjectHandler::GetBoolValue(result, property_);
        }

        void SetBoolValue(const std::shared_ptr<Optimization::OptimizationResult>& result, const std::string& property_, bool value) override
        {
            if (property_ == "succeeded") result->succeeded = value;
            else StoredObjectHandler::SetBoolValue(result, property_, value);
        }

        int GetIndexedIdValue(const std::shared_ptr<Optimization::OptimizationResult>& result, const std::string& property_, int index) override
        {
            if (property_ == "evaluations") return evaluationHandler->GetObjectId(result->evaluations[index]);
            if (property_ == "messages") return messageHandler->GetObjectId(result->messages[index]);
            return StoredObjectHandler::GetIndexedIdValue(result, property_, index);
        }

        double GetIndexedValue(const std::shared_ptr<Optimization::OptimizationResult>& result, const std::string& property_, int index) override
        {
            if (property_ == "values") return result->values[index];
            return StoredObjectHandler::GetIndexedValue(result, property_, index);
        }

        EvaluationHandler* evaluationHandler = nullptr;
        MessageHandler* messageHandler = nullptr;
    };
}

