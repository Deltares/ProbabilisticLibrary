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

#include "ModelProjectSettingsHandler.h"
#include "StoredObjectHandler.h"
#include "../../Server/ProjectEntries.h"
#include "../../Sensitivity/SensitivitySettings.h"

namespace Deltares::Server
{
    /**
     * \brief Handles properties and methods of class SensitivitySettings
     */
    class SensitivitySettingsHandler : public StoredObjectHandler<Sensitivity::SensitivitySettings>
    {
    public:
        ObjectType GetObjectType() override
        {
            return ObjectType::SensitivitySettings;
        }

        double GetValue(const std::shared_ptr<Sensitivity::SensitivitySettings>& settings, const std::string& property_) override
        {
            if (property_ == "low_value") return settings->LowValue;
            else if (property_ == "high_value") return settings->HighValue;
            else return StoredObjectHandler::GetValue(settings, property_);
        }

        void SetValue(const std::shared_ptr<Sensitivity::SensitivitySettings>& settings, const std::string& property_, double value) override
        {
            if (property_ == "low_value") settings->LowValue = value;
            else if (property_ == "high_value") settings->HighValue = value;
            else StoredObjectHandler::SetValue(settings, property_, value);
        }

        int GetIntValue(const std::shared_ptr<Sensitivity::SensitivitySettings>& settings, const std::string& property_) override
        {
            if (property_ == "iterations") return settings->Iterations;
            else return modelProjectSettingsHandler->GetIntValue(settings, property_);
        }

        void SetIntValue(const std::shared_ptr<Sensitivity::SensitivitySettings>& settings, const std::string& property_, int value) override
        {
            if (property_ == "iterations") settings->Iterations = value;
            else modelProjectSettingsHandler->SetIntValue(settings, property_, value);
        }

        int GetIdValue(const std::shared_ptr<Sensitivity::SensitivitySettings>& settings, const std::string& property_) override
        {
            return modelProjectSettingsHandler->GetIdValue(settings, property_);
        }

        bool GetBoolValue(const std::shared_ptr<Sensitivity::SensitivitySettings>& settings, const std::string& property_) override
        {
            return modelProjectSettingsHandler->GetBoolValue(settings, property_);
        }

        void SetBoolValue(const std::shared_ptr<Sensitivity::SensitivitySettings>& settings, const std::string& property_, bool value) override
        {
            modelProjectSettingsHandler->SetBoolValue(settings, property_, value);
        }

        std::string GetStringValue(const std::shared_ptr<Sensitivity::SensitivitySettings>& settings, const std::string& property_) override
        {
            if (property_ == "sensitivity_method") return Sensitivity::SensitivitySettings::getSensitivityMethodTypeString(settings->SensitivityMethod);
            else return modelProjectSettingsHandler->GetStringValue(settings, property_);
        }

        void SetStringValue(const std::shared_ptr<Sensitivity::SensitivitySettings>& settings, const std::string& property_, const std::string& value) override
        {
            if (property_ == "sensitivity_method") settings->SensitivityMethod = Sensitivity::SensitivitySettings::getSensitivityMethodType(value);
            else modelProjectSettingsHandler->SetStringValue(settings, property_, value);
        }

        ModelProjectSettingsHandler* modelProjectSettingsHandler = nullptr;
        StochastSettingsHandler* stochastSettingsHandler = nullptr;
    };
}

