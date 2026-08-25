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
#include "../../Logging/ValidationReport.h"

namespace Deltares::Server
{
    /**
     * \brief Handles properties and methods of class ValidationReport
     */
    class ValidationReportHandler : public StoredObjectHandler<Logging::ValidationReport>
    {
    public:

        ObjectType GetObjectType() override
        {
            return ObjectType::ValidationReport;
        }

        int GetIntValue(const std::shared_ptr<Logging::ValidationReport>& validationReport, const std::string& property_) override
        {
            if (property_ == "messages_count") return static_cast<int>(validationReport->messages.size());
            else return StoredObjectHandler::GetIntValue(validationReport, property_);
        }

        int GetIndexedIdValue(const std::shared_ptr<Logging::ValidationReport>& validationReport, const std::string& property_, int index) override
        {
            if (property_ == "messages") return messageHandler->GetObjectId(validationReport->messages[index]);
            else return StoredObjectHandler::GetIndexedIdValue(validationReport, property_);
        }

        bool GetBoolValue(const std::shared_ptr<Logging::ValidationReport>& validationReport, const std::string& property_) override
        {
            if (property_ == "is_valid") return validationReport->isValid();
            else return StoredObjectHandler::GetBoolValue(validationReport, property_);
        }

        MessageHandler* messageHandler = nullptr;

    };
}

