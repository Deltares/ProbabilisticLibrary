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
#include "../../Statistics/Stochast.h"
#include "../../Sensitivity/SensitivityValue.h"

namespace Deltares::Server
{
    /**
     * \brief Handles properties and methods of class FragilityValue
     */
    class SensitivityValueHandler : public StoredObjectHandler<Sensitivity::SensitivityValue>
    {
    public:
        ObjectType GetObjectType() override
        {
            return ObjectType::SensitivityValue;
        }

        double GetValue(const std::shared_ptr<Sensitivity::SensitivityValue>& sensitivityValue, const std::string& property_) override
        {
            if (property_ == "low") return sensitivityValue->low;
            else if (property_ == "medium") return sensitivityValue->medium;
            else if (property_ == "high") return sensitivityValue->high;
            else if (property_ == "first_order_index") return sensitivityValue->firstOrderIndex;
            else if (property_ == "total_index") return sensitivityValue->totalIndex;
            else return StoredObjectHandler::GetValue(sensitivityValue, property_);
        }

        void SetValue(const std::shared_ptr<Sensitivity::SensitivityValue>& sensitivityValue, const std::string& property_, double value) override
        {
            if (property_ == "low") sensitivityValue->low = value;
            else if (property_ == "medium") sensitivityValue->medium = value;
            else if (property_ == "high") sensitivityValue->high = value;
            else if (property_ == "first_order_index") sensitivityValue->firstOrderIndex = value;
            else if (property_ == "total_index") sensitivityValue->totalIndex = value;
            else StoredObjectHandler::SetValue(sensitivityValue, property_, value);
        }

        int GetIdValue(const std::shared_ptr<Sensitivity::SensitivityValue>& sensitivityValue, const std::string& property_) override
        {
            if (property_ == "variable") return stochastHandler->GetObjectId(sensitivityValue->stochast);
            else return StoredObjectHandler::GetIdValue(sensitivityValue, property_);
        }

        void SetIntValue(const std::shared_ptr<Sensitivity::SensitivityValue>& sensitivityValue, const std::string& property_, int value) override
        {
            if (property_ == "variable") sensitivityValue->stochast = stochastHandler->GetObject(value);
            else StoredObjectHandler::SetIntValue(sensitivityValue, property_, value);
        }

        StochastHandler* stochastHandler = nullptr;
    };
}

