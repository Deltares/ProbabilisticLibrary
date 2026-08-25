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

#include "LimitStateFunctionHandler.h"
#include "StoredObjectHandler.h"
#include "../../Server/ProjectEntries.h"
#include "../../Reliability/CombinedLimitStateFunction.h"


namespace Deltares::Server
{
    /**
     * \brief Handles properties and methods of class LimitStateFunction
     */
    class CombinedLimitStateFunctionHandler : public StoredObjectHandler<Reliability::CombinedLimitStateFunction>
    {
    public:
        ObjectType GetObjectType() override
        {
            return ObjectType::CombinedLimitStateFunction;
        }

        int GetIntValue(const std::shared_ptr<Reliability::CombinedLimitStateFunction>& limitStateFunction, const std::string& property_) override
        {
            if (property_ == "limit_state_functions_count") return static_cast<int>(limitStateFunction->limitStateFunctions.size());
            else return StoredObjectHandler::GetIntValue(limitStateFunction, property_);
        }

        bool GetBoolValue(const std::shared_ptr<Reliability::CombinedLimitStateFunction>& limitStateFunction, const std::string& property_) override
        {
            if (property_ == "normalize") return limitStateFunction->normalize;
            else return StoredObjectHandler::GetBoolValue(limitStateFunction, property_);
        }

        void SetBoolValue(const std::shared_ptr<Reliability::CombinedLimitStateFunction>& limitStateFunction, const std::string& property_, bool value) override
        {
            if (property_ == "normalize") limitStateFunction->normalize = value;
            else StoredObjectHandler::SetBoolValue(limitStateFunction, property_, value);
        }

        std::string GetStringValue(const std::shared_ptr<Reliability::CombinedLimitStateFunction>& limitStateFunction, const std::string& property_) override
        {
            if (property_ == "combine_type") return Reliability::DesignPointCombiner::getCombineTypeString(limitStateFunction->combineType);
            else return StoredObjectHandler::GetStringValue(limitStateFunction, property_);
        }

        void SetStringValue(const std::shared_ptr<Reliability::CombinedLimitStateFunction>& limitStateFunction, const std::string& property_, const std::string& value) override
        {
            if (property_ == "combine_type") limitStateFunction->combineType = Reliability::DesignPointCombiner::getCombineType(value);
            else StoredObjectHandler::SetStringValue(limitStateFunction, property_, value);
        }

        void SetArrayIntValue(const std::shared_ptr<Reliability::CombinedLimitStateFunction>& limitStateFunction, const std::string& property_, int* values, int size) override
        {
            if (property_ == "limit_state_functions")
            {
                limitStateFunction->limitStateFunctions.clear();
                for (int i = 0; i < size; i++)
                {
                    if (this->Contains(values[i]))
                    {
                        limitStateFunction->limitStateFunctions.push_back(this->GetObject(values[i]));
                    }
                    else if (limitStateFunctionHandler->Contains(values[i]))
                    {
                        limitStateFunction->limitStateFunctions.push_back(limitStateFunctionHandler->GetObject(values[i]));
                    }
                }
            }
            else StoredObjectHandler::SetArrayIntValue(limitStateFunction, property_, values, size);
        }

        int GetIndexedIdValue(const std::shared_ptr<Reliability::CombinedLimitStateFunction>& limitStateFunction, const std::string& property_, int index) override
        {
            if (property_ == "limit_state_functions") return limitStateFunctionHandler->GetObjectId(limitStateFunction->limitStateFunctions[index]);
            else return StoredObjectHandler::GetIndexedIdValue(limitStateFunction, property_, index);
        }

        LimitStateFunctionHandler* limitStateFunctionHandler = nullptr;
    };
}

