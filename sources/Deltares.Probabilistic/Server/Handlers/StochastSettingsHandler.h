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
#include "../../Reliability/StochastSettings.h"

namespace Deltares::Server
{
    /**
     * \brief Handles properties and methods of class StochastSettings
     */
    class StochastSettingsHandler : public StoredObjectHandler<Reliability::StochastSettings>
    {
    public:

        ObjectType GetObjectType() override
        {
            return ObjectType::StochastSettings;
        }

        double GetValue(const std::shared_ptr<Reliability::StochastSettings>& stochastSettings, const std::string& property_) override
        {
            if (property_ == "min_value") return stochastSettings->MinValue;
            else if (property_ == "max_value") return stochastSettings->MaxValue;
            else if (property_ == "start_value") return stochastSettings->StartValue;
            else if (property_ == "variance_factor") return stochastSettings->VarianceFactor;
            else return StoredObjectHandler::GetValue(stochastSettings, property_);
        }

        void SetValue(const std::shared_ptr<Reliability::StochastSettings>& stochastSettings, const std::string& property_, double value) override
        {
            if (property_ == "min_value") stochastSettings->MinValue = value;
            else if (property_ == "max_value") stochastSettings->MaxValue = value;
            else if (property_ == "start_value") stochastSettings->StartValue = value;
            else if (property_ == "variance_factor") stochastSettings->VarianceFactor = value;
            else StoredObjectHandler::SetValue(stochastSettings, property_, value);
        }

        int GetIntValue(const std::shared_ptr<Reliability::StochastSettings>& stochastSettings, const std::string& property_) override
        {
            if (property_ == "intervals") return stochastSettings->Intervals;
            else return StoredObjectHandler::GetIntValue(stochastSettings, property_);
        }

        int GetIdValue(const std::shared_ptr<Reliability::StochastSettings>& stochastSettings, const std::string& property_) override
        {
            if (property_ == "variable") return stochastHandler->GetObjectId(stochastSettings->stochast);
            else return StoredObjectHandler::GetIdValue(stochastSettings, property_);
        }

        void SetIntValue(const std::shared_ptr<Reliability::StochastSettings>& stochastSettings, const std::string& property_, int value) override
        {
            if (property_ == "variable") stochastSettings->stochast = stochastHandler->GetObject(value);
            else if (property_ == "intervals") stochastSettings->Intervals = value;
            else StoredObjectHandler::SetIntValue(stochastSettings, property_, value);
        }

        bool GetBoolValue(const std::shared_ptr<Reliability::StochastSettings>& stochastSettings, const std::string& property_) override
        {
            if (property_ == "is_initialization_allowed") return stochastSettings->IsInitializationAllowed;
            else if (property_ == "is_variance_allowed") return stochastSettings->IsVarianceAllowed;
            else return StoredObjectHandler::GetBoolValue(stochastSettings, property_);
        }

        void SetBoolValue(const std::shared_ptr<Reliability::StochastSettings>& stochastSettings, const std::string& property_, bool value) override
        {
            if (property_ == "is_initialization_allowed") stochastSettings->IsInitializationAllowed = value;
            else if (property_ == "is_variance_allowed") stochastSettings->IsVarianceAllowed = value;
            else StoredObjectHandler::SetBoolValue(stochastSettings, property_, value);
        }

        StochastHandler* stochastHandler = nullptr;
    };
}

