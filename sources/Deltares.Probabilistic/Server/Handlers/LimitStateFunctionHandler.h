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
#include "../../Server/ProjectEntries.h"
#include "../../Reliability/LimitStateFunction.h"

namespace Deltares::Server
{
    /**
     * \brief Handles properties and methods of class LimitStateFunction
     */
    class LimitStateFunctionHandler : public StoredObjectHandler<Reliability::LimitStateFunction>
    {
    public:
        ObjectType GetObjectType() override
        {
            return ObjectType::LimitStateFunction;
        }

        double GetValue(const std::shared_ptr<Reliability::LimitStateFunction>& limitStateFunction, const std::string& property_) override
        {
            if (property_ == "critical_value") return limitStateFunction->criticalValue;
            else return StoredObjectHandler::GetValue(limitStateFunction, property_);
        }

        void SetValue(const std::shared_ptr<Reliability::LimitStateFunction>& limitStateFunction, const std::string& property_, double value) override
        {
            if (property_ == "critical_value") limitStateFunction->criticalValue = value;
            else StoredObjectHandler::SetValue(limitStateFunction, property_, value);
        }

        bool GetBoolValue(const std::shared_ptr<Reliability::LimitStateFunction>& limitStateFunction, const std::string& property_) override
        {
            if (property_ == "use_compare_parameter") return limitStateFunction->useCompareParameter;
            else if (property_ == "normalize") return limitStateFunction->normalize;
            else return StoredObjectHandler::GetBoolValue(limitStateFunction, property_);
        }

        void SetBoolValue(const std::shared_ptr<Reliability::LimitStateFunction>& limitStateFunction, const std::string& property_, bool value) override
        {
            if (property_ == "use_compare_parameter") limitStateFunction->useCompareParameter = value;
            else if (property_ == "normalize") limitStateFunction->normalize = value;
            else StoredObjectHandler::SetBoolValue(limitStateFunction, property_, value);
        }

        std::string GetStringValue(const std::shared_ptr<Reliability::LimitStateFunction>& limitStateFunction, const std::string& property_) override
        {
            if (property_ == "parameter") return limitStateFunction->criticalParameter;
            else if (property_ == "compare_parameter") return limitStateFunction->compareParameter;
            else if (property_ == "compare_type") return Reliability::LimitStateFunction::GetCompareTypeString(limitStateFunction->compareType);
            else return StoredObjectHandler::GetStringValue(limitStateFunction, property_);
        }

        void SetStringValue(const std::shared_ptr<Reliability::LimitStateFunction>& limitStateFunction, const std::string& property_, const std::string& value) override
        {
            if (property_ == "parameter") limitStateFunction->criticalParameter = value;
            else if (property_ == "compare_parameter") limitStateFunction->compareParameter = value;
            else if (property_ == "compare_type") limitStateFunction->compareType = Reliability::LimitStateFunction::GetCompareType(value);
            else StoredObjectHandler::SetStringValue(limitStateFunction, property_, value);
        }
    };
}

