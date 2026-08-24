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
#include "ObjectHandler.h"
#include "StoredObjectHandler.h"
#include "../../Server/ProjectEntries.h"
#include "../../Statistics/DiscreteValue.h"

namespace Deltares::Server
{
    /**
     * \brief Handles properties and methods of class DiscreteValue
     */
    class DiscreteValueHandler : public StoredObjectHandler<Statistics::DiscreteValue>
    {
    public:

        ObjectType GetObjectType() override
        {
            return ObjectType::DiscreteValue;
        }

        double GetValue(int id, const std::string& property_) override
        {
            std::shared_ptr<Statistics::DiscreteValue> discreteValue = GetObject(id);

            if (property_ == "x") return discreteValue->X;
            else if (property_ == "amount") return discreteValue->Amount;
            else if (property_ == "normalized_amount") return discreteValue->NormalizedAmount;
            else if (property_ == "cumulative_amount") return discreteValue->CumulativeNormalizedAmount;
            else return ObjectHandler::GetValue(id, property_);
        }

        void SetValue(int id, const std::string& property_, double value) override
        {
            std::shared_ptr<Statistics::DiscreteValue> discreteValue = GetObject(id);

            if (property_ == "x") discreteValue->X = value;
            else if (property_ == "amount") discreteValue->Amount = value;
            else ObjectHandler::SetValue(id, property_, value);

            discreteValue->setDirty();
        }
    };
}

