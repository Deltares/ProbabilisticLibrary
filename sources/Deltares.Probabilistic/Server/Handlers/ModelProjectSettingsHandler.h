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
#include "ValidationReportHandler.h"
#include "../../Server/ProjectEntries.h"
#include "../../Model/ModelProjectSettings.h"

namespace Deltares::Server
{
    /**
     * \brief Handles properties and methods of class ModelProjectSettings
     */
    class ModelProjectSettingsHandler : public StoredObjectHandler<Models::ModelProjectSettings>
    {
    public:
        ObjectType GetObjectType() override
        {
            return ObjectType::Settings;
        }

        int GetIdValue(const std::shared_ptr<Models::ModelProjectSettings>& settings, const std::string& property_) override
        {
            if (property_ == "validate") return validationReportHandler->GetObjectId(std::make_shared<Logging::ValidationReport>(settings->getValidationReport()));
            else return StoredObjectHandler::GetIdValue(settings, property_);
        }

        int GetIntValue(const std::shared_ptr<Models::ModelProjectSettings>& settings, const std::string& property_) override
        {
            if (property_ == "max_parallel_processes") return settings->RunSettings->MaxParallelProcesses;
            else if (property_ == "max_chunk_size") return settings->RunSettings->MaxChunkSize;
            else if (property_ == "max_messages") return settings->RunSettings->MaxMessages;
            else return StoredObjectHandler::GetIntValue(settings, property_);
        }

        void SetIntValue(const std::shared_ptr<Models::ModelProjectSettings>& settings, const std::string& property_, int value) override
        {
            if (property_ == "max_parallel_processes") settings->RunSettings->MaxParallelProcesses = value;
            else if (property_ == "max_chunk_size") settings->RunSettings->MaxChunkSize = value;
            else if (property_ == "max_messages") settings->RunSettings->MaxMessages = value;
            else StoredObjectHandler::SetIntValue(settings, property_, value);
        }

        bool GetBoolValue(const std::shared_ptr<Models::ModelProjectSettings>& settings, const std::string& property_) override
        {
            if (property_ == "is_valid") return settings->isValid();
            else if (property_ == "save_realizations") return settings->RunSettings->SaveEvaluations;
            else if (property_ == "save_convergence") return settings->RunSettings->SaveConvergence;
            else if (property_ == "save_messages") return settings->RunSettings->SaveMessages;
            else if (property_ == "reuse_calculations") return settings->RunSettings->ReuseCalculations;
            else if (property_ == "allow_repository") return settings->RunSettings->AllowRepository;
            else if (property_ == "use_z_from_sample") return settings->RunSettings->UseZFromSample;
            else return StoredObjectHandler::GetBoolValue(settings, property_);
        }

        void SetBoolValue(const std::shared_ptr<Models::ModelProjectSettings>& settings, const std::string& property_, bool value) override
        {
            if (property_ == "save_realizations") settings->RunSettings->SaveEvaluations = value;
            else if (property_ == "save_convergence") settings->RunSettings->SaveConvergence = value;
            else if (property_ == "save_messages") settings->RunSettings->SaveMessages = value;
            else if (property_ == "reuse_calculations") settings->RunSettings->ReuseCalculations = value;
            else if (property_ == "allow_repository") settings->RunSettings->AllowRepository = value;
            else if (property_ == "use_z_from_sample") settings->RunSettings->UseZFromSample = value;
            else if (property_ == "use_openmp_in_reliability") settings->RunSettings->UseOpenMPinReliability = value;
            else StoredObjectHandler::SetBoolValue(settings, property_, value);
        }

        std::string GetStringValue(const std::shared_ptr<Models::ModelProjectSettings>& settings, const std::string& property_) override
        {
            if (property_ == "lowest_message_type") return Logging::Message::getMessageTypeString(settings->RunSettings->LowestMessageType);
            else return StoredObjectHandler::GetStringValue(settings, property_);
        }

        void SetStringValue(const std::shared_ptr<Models::ModelProjectSettings>& settings, const std::string& property_, const std::string& value) override
        {
            if (property_ == "lowest_message_type") settings->RunSettings->LowestMessageType = Logging::Message::getMessageType(value);
            else StoredObjectHandler::SetStringValue(settings, property_, value);
        }

        ValidationReportHandler* validationReportHandler = nullptr;
    };
}

