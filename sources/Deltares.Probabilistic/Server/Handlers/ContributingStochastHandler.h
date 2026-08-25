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

#include "BaseStochastHandler.h"
#include "StoredObjectHandler.h"
#include "../../Statistics/Stochast.h"
#include "../../Statistics/ContributingStochast.h"

namespace Deltares::Server
{
    /**
     * \brief Handles properties and methods of class ContributingStochast
     */
    class ContributingStochastHandler : public StoredObjectHandler<Statistics::ContributingStochast>
    {
    public:

        ObjectType GetObjectType() override
        {
            return ObjectType::ContributingStochast;
        }

        double GetValue(const std::shared_ptr<Statistics::ContributingStochast>& contributingStochast, const std::string& property_) override
        {
            if (property_ == "probability") return contributingStochast->Probability;
            else return StoredObjectHandler::GetValue(contributingStochast, property_);
        }

        void SetValue(const std::shared_ptr<Statistics::ContributingStochast>& contributingStochast, const std::string& property_, double value) override
        {
            if (property_ == "probability") contributingStochast->Probability = value;
            else StoredObjectHandler::SetValue(contributingStochast, property_, value);
        }

        int GetIdValue(const std::shared_ptr<Statistics::ContributingStochast>& contributingStochast, const std::string& property_) override
        {
            if (property_ == "variable") return stochastHandler->GetObjectId(std::static_pointer_cast<Statistics::Stochast>(contributingStochast->Stochast));
            else return StoredObjectHandler::GetIdValue(contributingStochast, property_);
        }

        void SetIntValue(const std::shared_ptr<Statistics::ContributingStochast>& contributingStochast, const std::string& property_, int value) override
        {
            if (property_ == "variable") contributingStochast->Stochast = stochastHandler->GetObject(value);
            else StoredObjectHandler::SetIntValue(contributingStochast, property_, value);
        }

        BaseStochastHandler* stochastHandler = nullptr;
    };
}

