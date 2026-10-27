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
#include "../../Model/ConvergenceResult.h"

namespace Deltares::Server
{
    /**
     * \brief Handles properties and methods of class ConvergenceResult
     */
    class ConvergenceResultHandler : public StoredObjectHandler<Models::ConvergenceResult>
    {
    public:

        ObjectType GetObjectType() override
        {
            return ObjectType::ConvergenceResult;
        }

        double GetValue(const std::shared_ptr<Models::ConvergenceResult>& result, const std::string& property_) override
        {
            if (property_ == "difference") return result->Difference;
            else return StoredObjectHandler::GetValue(result, property_);
        }

        void SetValue(const std::shared_ptr<Models::ConvergenceResult>& result, const std::string& property_, double value) override
        {
            if (property_ == "difference") result->Difference = value;
            else StoredObjectHandler::SetValue(result, property_, value);
        }

        int GetIntValue(const std::shared_ptr<Models::ConvergenceResult>& result, const std::string& property_) override
        {
            if (property_ == "index") return result->Index;
            else if (property_ == "samples") return result->Samples;
            else return StoredObjectHandler::GetIntValue(result, property_);
        }

        void SetIntValue(const std::shared_ptr<Models::ConvergenceResult>& result, const std::string& property_, int value) override
        {
            if (property_ == "index") result->Index = value;
            else if (property_ == "samples") result->Samples = value;
            else StoredObjectHandler::SetIntValue(result, property_, value);
        }
    };
}

