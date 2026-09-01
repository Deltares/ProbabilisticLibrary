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

#include "CombineSettingsHandler.h"
#include "CopulaCorrelationHandler.h"
#include "CorrelationMatrixHandler.h"
#include "DesignPointHandler.h"
#include "SelfCorrelationMatrixHandler.h"
#include "StoredObjectHandler.h"
#include "../../Combine/CombineProject.h"

namespace Deltares::Server
{
    /**
     * \brief Handles properties and methods of class CombineProject
     */
    class CombineProjectHandler : public StoredObjectHandler<Reliability::CombineProject>
    {
    public:

        ObjectType GetObjectType() override
        {
            return ObjectType::CombineProject;
        }

        bool GetBoolValue(const std::shared_ptr<Reliability::CombineProject>& project, const std::string& property_) override
        {
            if (property_ == "is_valid") return project->is_valid();
            else return StoredObjectHandler::GetBoolValue(project, property_);
        }

        int GetIntValue(const std::shared_ptr<Reliability::CombineProject>& project, const std::string& property_) override
        {
            if (property_ == "design_points_count") return static_cast<int>(project->designPoints.size());
            else return StoredObjectHandler::GetIntValue(project, property_);
        }

        int GetIdValue(const std::shared_ptr<Reliability::CombineProject>& project, const std::string& property_) override
        {
            if (property_ == "settings") return combineSettingsHandler->GetObjectId(project->settings);
            else if (property_ == "design_point") return designPointHandler->GetObjectId(project->designPoint);
            else if (property_ == "design_point_correlation_matrix")
            {
                if (std::dynamic_pointer_cast<Statistics::CopulaCorrelation>(project->correlationMatrix) != nullptr)
                {
                    return copulaCorrelationHandler->GetObjectId(std::dynamic_pointer_cast<Statistics::CopulaCorrelation>(project->correlationMatrix));
                }
                else
                {
                    return correlationMatrixHandler->GetObjectId(std::dynamic_pointer_cast<Statistics::CorrelationMatrix>(project->correlationMatrix));
                }
            }
            else if (property_ == "correlation_matrix") return selfCorrelationMatrixHandler->GetObjectId(project->selfCorrelationMatrix);
            else if (property_ == "validate") return validationReportHandler->GetObjectId(std::make_shared<Logging::ValidationReport>(project->getValidationReport()));
            else return StoredObjectHandler::GetIdValue(project, property_);
        }

        void SetIntValue(const std::shared_ptr<Reliability::CombineProject>& project, const std::string& property_, int value) override
        {
            if (property_ == "settings") project->settings = combineSettingsHandler->GetObject(value);
            else if (property_ == "correlation_matrix") project->selfCorrelationMatrix = selfCorrelationMatrixHandler->GetObject(value);
            else if (property_ == "design_point_correlation_matrix")
            {
                if (copulaCorrelationHandler->Contains(value))
                {
                    project->correlationMatrix = copulaCorrelationHandler->GetObject(value);
                }
                else
                {
                    project->correlationMatrix = correlationMatrixHandler->GetObject(value);
                }
            }
            else StoredObjectHandler::SetIntValue(project, property_, value);
        }

        void SetArrayIntValue(const std::shared_ptr<Reliability::CombineProject>& project, const std::string& property_, int* values, int size) override
        {
            if (property_ == "design_points")
            {
                project->designPoints.clear();

                for (int i = 0; i < size; i++)
                {
                    project->designPoints.push_back(designPointHandler->GetObject(values[i]));
                }
            }
            else StoredObjectHandler::SetArrayIntValue(project, property_, values, size);
        }

        void Execute(const std::shared_ptr<Reliability::CombineProject>& project, const std::string& method_) override
        {
            if (method_ == "run") project->run();
            else StoredObjectHandler::Execute(project, method_);
        }

        void SetProgressCallBacks(const std::shared_ptr<Reliability::CombineProject>& project, Models::ProgressCallBack progress, Models::DetailedProgressCallBack detailed, Models::TextualProgressCallBack textual) override
        {
            project->progressIndicator = std::make_shared<Models::ProgressIndicator>(progress, detailed, textual);
        }

        CombineSettingsHandler* combineSettingsHandler = nullptr;
        DesignPointHandler* designPointHandler = nullptr;
        CorrelationMatrixHandler* correlationMatrixHandler = nullptr;
        CopulaCorrelationHandler* copulaCorrelationHandler = nullptr;
        SelfCorrelationMatrixHandler* selfCorrelationMatrixHandler = nullptr;
        ValidationReportHandler* validationReportHandler = nullptr;
    };
}

