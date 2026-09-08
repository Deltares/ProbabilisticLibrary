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
#include "LimitStateFunctionHandler.h"
#include "DesignPointHandler.h"
#include "ReliabilitySettingsHandler.h"
#include "../../Server/ProjectEntries.h"
#include "../../Reliability/ReliabilityProject.h"

namespace Deltares::Server
{
    /**
     * \brief Handles properties and methods of class RunProject
     */
    class ReliabilityProjectHandler : public DerivedObjectHandler<Reliability::ReliabilityProject, Models::ModelProject>
    {
    public:
        ObjectType GetObjectType() override
        {
            return ObjectType::Project;
        }

        int GetIdValue(const std::shared_ptr<Reliability::ReliabilityProject>& project, const std::string& property_) override
        {
            if (property_ == "limit_state_function") return limitStateFunctionHandler->GetObjectId(project->limitStateFunction);
            else if (property_ == "design_point") return designPointHandler->GetObjectId(project->designPoint);
            else if (property_ == "settings") return reliabilitySettingsHandler->GetObjectId(project->settings);
            else return DerivedObjectHandler::GetIdValue(project, property_);
        }

        void SetIntValue(const std::shared_ptr<Reliability::ReliabilityProject>& project, const std::string& property_, int value) override
        {
            if (property_ == "settings") project->setSettings(reliabilitySettingsHandler->GetObject(value));
            else if (property_ == "limit_state_function") project->limitStateFunction = limitStateFunctionHandler->GetObject(value);
            else DerivedObjectHandler::SetIntValue(project, property_, value);
        }

        ReliabilitySettingsHandler* reliabilitySettingsHandler = nullptr;
        DesignPointHandler* designPointHandler = nullptr;
        LimitStateFunctionHandler* limitStateFunctionHandler = nullptr;
    };
}

