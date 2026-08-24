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

#include "StochastHandler.h"
#include "StoredObjectHandler.h"
#include "../../Statistics/Scenario.h"

namespace Deltares::Server
{
    /**
     * \brief Handles properties and methods of class HistogramValue
     */
    class ScenarioHandler : public StoredObjectHandler<Statistics::Scenario>
    {
    public:

        ObjectType GetObjectType() override
        {
            return ObjectType::Scenario;
        }

        double GetValue(int id, const std::string& property_) override
        {
            std::shared_ptr<Statistics::Scenario> scenario = GetObject(id);

            if (property_ == "probability") return scenario->probability;
            else if (property_ == "physical_value") return scenario->parameterValue;
            else return ObjectHandler::GetValue(id, property_);
        }

        void SetValue(int id, const std::string& property_, double value) override
        {
            std::shared_ptr<Statistics::Scenario> scenario = GetObject(id);

            if (property_ == "probability") scenario->probability = value;
            else if (property_ == "physical_value") scenario->parameterValue = value;
            else ObjectHandler::SetValue(id, property_, value);
        }

        int GetIdValue(int id, const std::string& property_) override
        {
            std::shared_ptr<Statistics::Scenario> scenario = GetObject(id);

            if (property_ == "parameter") return stochastHandler->GetObjectId(scenario->parameter);
            else return ObjectHandler::GetIdValue(id, property_);
        }

        void SetIntValue(int id, const std::string& property_, int value) override
        {
            std::shared_ptr<Statistics::Scenario> scenario = GetObject(id);

            if (property_ == "parameter") scenario->parameter = stochastHandler->GetObject(value);
            else ObjectHandler::SetIntValue(id, property_, value);
        }

        std::string GetStringValue(int id, const std::string& property_) override
        {
            std::shared_ptr<Statistics::Scenario> scenario = GetObject(id);

            if (property_ == "name") return scenario->name;
            else return ObjectHandler::GetStringValue(id, property_);
        }

        void SetStringValue(int id, const std::string& property_, const std::string& value) override
        {
            std::shared_ptr<Statistics::Scenario> scenario = GetObject(id);

            if (property_ == "name") scenario->name = value;
            else ObjectHandler::SetStringValue(id, property_, value);
        }

        StochastHandler* stochastHandler = nullptr;
    };
}

