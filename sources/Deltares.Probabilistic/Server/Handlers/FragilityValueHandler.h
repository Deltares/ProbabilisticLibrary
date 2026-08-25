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
#include "../../Statistics/FragilityValue.h"

namespace Deltares::Server
{
    /**
     * \brief Handles properties and methods of class FragilityValue
     */
    class FragilityValueHandler : public StoredObjectHandler<Statistics::FragilityValue>
    {
    public:
        ObjectType GetObjectType() override
        {
            return ObjectType::FragilityValue;
        }

        double GetValue(const std::shared_ptr<Statistics::FragilityValue>& fragilityValue, const std::string& property_) override
        {
            if (property_ == "x") return fragilityValue->X;
            else if (property_ == "reliability_index") return fragilityValue->Reliability;
            else if (property_ == "probability_of_failure") return fragilityValue->getProbabilityOfFailure();
            else if (property_ == "probability_of_non_failure") return fragilityValue->getProbabilityOfNonFailure();
            else if (property_ == "return_period") return fragilityValue->getReturnPeriod();
            else return StoredObjectHandler::GetValue(fragilityValue, property_);
        }

        void SetValue(const std::shared_ptr<Statistics::FragilityValue>& fragilityValue, const std::string& property_, double value) override
        {
            if (property_ == "x") fragilityValue->X = value;
            else if (property_ == "reliability_index") fragilityValue->Reliability = value;
            else if (property_ == "probability_of_failure") fragilityValue->setProbabilityOfFailure(value);
            else if (property_ == "probability_of_non_failure") fragilityValue->setProbabilityOfNonFailure(value);
            else if (property_ == "return_period") fragilityValue->setReturnPeriod(value);
            else StoredObjectHandler::SetValue(fragilityValue, property_, value);

            fragilityValue->setDirty();
        }
    };
}

