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
#include "../../Statistics/ProbabilityValue.h"

namespace Deltares::Server
{
    /**
     * \brief Handles properties and methods of class ProbabilityValue
     */
    class ProbabilityValueHandler : public StoredObjectHandler<Statistics::ProbabilityValue>
    {
    public:
        ObjectType GetObjectType() override
        {
            return ObjectType::ProbabilityValue;
        }

        double GetValue(const std::shared_ptr<Statistics::ProbabilityValue>& probabilityValue, const std::string& property_) override
        {
            if (property_ == "reliability_index") return probabilityValue->Reliability;
            else if (property_ == "probability_of_failure") return probabilityValue->getProbabilityOfFailure();
            else if (property_ == "probability_of_non_failure") return probabilityValue->getProbabilityOfNonFailure();
            else if (property_ == "return_period") return probabilityValue->getReturnPeriod();
            else return StoredObjectHandler::GetValue(probabilityValue, property_);
        }

        void SetValue(const std::shared_ptr<Statistics::ProbabilityValue>& probabilityValue, const std::string& property_, double value) override
        {
            if (property_ == "reliability_index") probabilityValue->Reliability = value;
            else if (property_ == "probability_of_failure") probabilityValue->setProbabilityOfFailure(value);
            else if (property_ == "probability_of_non_failure") probabilityValue->setProbabilityOfNonFailure(value);
            else if (property_ == "return_period") probabilityValue->setReturnPeriod(value);
            else StoredObjectHandler::SetValue(probabilityValue, property_, value);
        }
    };
}

