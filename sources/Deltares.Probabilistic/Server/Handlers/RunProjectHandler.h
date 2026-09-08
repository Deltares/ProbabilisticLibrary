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

#include "DerivedObjectHandler.h"
#include "EvaluationHandler.h"
#include "RunProjectSettingsHandler.h"
#include "StoredObjectHandler.h"
#include "../../Server/ProjectEntries.h"
#include "../../Model/RunProject.h"

namespace Deltares::Server
{
    /**
     * \brief Handles properties and methods of class RunProject
     */
    class RunProjectHandler : public DerivedObjectHandler<Models::RunProject, Models::ModelProject>
    {
    public:
        ObjectType GetObjectType() override
        {
            return ObjectType::RunProject;
        }

        int GetIdValue(const std::shared_ptr<Models::RunProject>& project, const std::string& property_) override
        {
            if (property_ == "settings") return runProjectSettingsHandler->GetObjectId(project->settings);
            else if (property_ == "realization") return evaluationHandler->GetObjectId(project->evaluation);
            else return DerivedObjectHandler::GetIdValue(project, property_);
        }

        void SetIntValue(const std::shared_ptr<Models::RunProject>& project, const std::string& property_, int value) override
        {
            if (property_ == "settings") project->setSettings(runProjectSettingsHandler->GetObject(value));
            else DerivedObjectHandler::SetIntValue(project, property_, value);
        }

        RunProjectSettingsHandler* runProjectSettingsHandler = nullptr;
        EvaluationHandler* evaluationHandler = nullptr;
    };
}

