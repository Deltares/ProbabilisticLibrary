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
#include "../../Reliability/FragilityCurveIntegrationSettings.h"

namespace Deltares::Server
{
    /**
     * \brief Handles properties and methods of class FragilityCurveIntegrationSettings
     */
    class FragilityCurveSettingsHandler : public StoredObjectHandler<Reliability::FragilityCurveIntegrationSettings>
    {
    public:
        ObjectType GetObjectType() override
        {
            return ObjectType::FragilityCurveSettings;
        }

        double GetValue(const std::shared_ptr<Reliability::FragilityCurveIntegrationSettings>& settings, const std::string& property_) override
        {
            if (property_ == "step_size") return settings->StepSize;
            else return StoredObjectHandler::GetValue(settings, property_);
        }

        void SetValue(const std::shared_ptr<Reliability::FragilityCurveIntegrationSettings>& settings, const std::string& property_, double value) override
        {
            if (property_ == "step_size") settings->StepSize = value;
            else StoredObjectHandler::SetValue(settings, property_, value);
        }

        std::string GetStringValue(const std::shared_ptr<Reliability::FragilityCurveIntegrationSettings>& settings, const std::string& property_) override
        {
            if (property_ == "design_point_method") return Reliability::DesignPointBuilder::getDesignPointMethodString(settings->designPointMethod);
            else return StoredObjectHandler::GetStringValue(settings, property_);
        }

        void SetStringValue(const std::shared_ptr<Reliability::FragilityCurveIntegrationSettings>& settings, const std::string& property_, const std::string& value) override
        {
            if (property_ == "design_point_method") settings->designPointMethod = Reliability::DesignPointBuilder::getDesignPointMethod(value);
            else StoredObjectHandler::SetStringValue(settings, property_, value);
        }
    };
}

