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

#include "ExcludingCombineSettingsHandler.h"
#include "CopulaCorrelationHandler.h"
#include "CorrelationMatrixHandler.h"
#include "DesignPointHandler.h"
#include "ScenarioHandler.h"
#include "SelfCorrelationMatrixHandler.h"
#include "StoredObjectHandler.h"
#include "../../Combine/ExcludingCombineProject.h"

namespace Deltares::Server
{
    /**
     * \brief Handles properties and methods of class ExcludingCombineProject
     */
    class ExcludingCombineProjectHandler : public StoredObjectHandler<Reliability::ExcludingCombineProject>
    {
    public:

        ObjectType GetObjectType() override
        {
            return ObjectType::ExcludingCombineProject;
        }

        bool GetBoolValue(const std::shared_ptr<Reliability::ExcludingCombineProject>& project, const std::string& property_) override
        {
            if (property_ == "is_valid") return project->is_valid();
            else return StoredObjectHandler::GetBoolValue(project, property_);
        }

        int GetIntValue(const std::shared_ptr<Reliability::ExcludingCombineProject>& project, const std::string& property_) override
        {
            if (property_ == "design_points_count") return static_cast<int>(project->designPoints.size());
            else if (property_ == "scenarios_count") return static_cast<int>(project->scenarios.size());
            else return StoredObjectHandler::GetIntValue(project, property_);
        }

        int GetIdValue(const std::shared_ptr<Reliability::ExcludingCombineProject>& project, const std::string& property_) override
        {
            if (property_ == "design_point") return designPointHandler->GetObjectId(project->designPoint);
            else if (property_ == "validate") return validationReportHandler->GetObjectId(std::make_shared<Logging::ValidationReport>(project->getValidationReport()));
            else return StoredObjectHandler::GetIdValue(project, property_);
        }

        void SetIntValue(const std::shared_ptr<Reliability::ExcludingCombineProject>& project, const std::string& property_, int value) override
        {
            if (property_ == "settings") project->settings = excludingCombineSettingsHandler->GetObject(value);
            else StoredObjectHandler::SetIntValue(project, property_, value);
        }

        void SetArrayIntValue(const std::shared_ptr<Reliability::ExcludingCombineProject>& project, const std::string& property_, int* values, int size) override
        {
            if (property_ == "design_points")
            {
                project->designPoints.clear();

                for (int i = 0; i < size; i++)
                {
                    project->designPoints.push_back(designPointHandler->GetObject(values[i]));
                }
            }
            else if (property_ == "scenarios")
            {
                project->scenarios.clear();

                for (int i = 0; i < size; i++)
                {
                    project->scenarios.push_back(scenarioHandler->GetObject(values[i]));
                }
            }
            else StoredObjectHandler::SetArrayIntValue(project, property_, values, size);
        }

        void Execute(const std::shared_ptr<Reliability::ExcludingCombineProject>& project, const std::string& method_) override
        {
            if (method_ == "run") project->run();
            else StoredObjectHandler::Execute(project, method_);
        }

        ExcludingCombineSettingsHandler* excludingCombineSettingsHandler = nullptr;
        DesignPointHandler* designPointHandler = nullptr;
        ScenarioHandler* scenarioHandler = nullptr;
        ValidationReportHandler* validationReportHandler = nullptr;
    };
}

