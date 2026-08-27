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

#include "SelfCorrelationMatrixHandler.h"
#include "StoredObjectHandler.h"
#include "../../Combine/LengthEffectProject.h"

namespace Deltares::Server
{
    /**
     * \brief Handles properties and methods of class LengthEffectProject
     */
    class LengthEffectProjectHandler : public StoredObjectHandler<Reliability::LengthEffectProject>
    {
    public:

        ObjectType GetObjectType() override
        {
            return ObjectType::LengthEffectProject;
        }

        double GetValue(const std::shared_ptr<Reliability::LengthEffectProject>& project, const std::string& property_) override
        {
            if (property_ == "length") return project->length;
            else return StoredObjectHandler::GetValue(project, property_);
        }

        void SetValue(const std::shared_ptr<Reliability::LengthEffectProject>& project, const std::string& property_, double value) override
        {
            if (property_ == "length") project->length = value;
            else StoredObjectHandler::SetValue(project, property_, value);
        }

        int GetIntValue(const std::shared_ptr<Reliability::LengthEffectProject>& project, const std::string& property_) override
        {
            if (property_ == "correlation_lengths_count") return static_cast<int>(project->correlationLengths.size());
            else return StoredObjectHandler::GetIntValue(project, property_);
        }

        int GetIdValue(const std::shared_ptr<Reliability::LengthEffectProject>& project, const std::string& property_) override
        {
            if (property_ == "design_point") return designPointHandler->GetObjectId(project->designPoint);
            else return StoredObjectHandler::GetIdValue(project, property_);
        }

        void SetIntValue(const std::shared_ptr<Reliability::LengthEffectProject>& project, const std::string& property_, int value) override
        {
            if (property_ == "correlation_matrix") project->selfCorrelationMatrix = selfCorrelationMatrixHandler->GetObject(value);
            else if (property_ == "design_point_cross_section") project->designPointCrossSection = designPointHandler->GetObject(value);
            else StoredObjectHandler::SetIntValue(project, property_, value);
        }

        void SetArrayValue(const std::shared_ptr<Reliability::LengthEffectProject>& project, const std::string& property_, double* values, int size) override
        {
            if (property_ == "correlation_lengths")
            {
                project->correlationLengths.clear();
                for (int i = 0; i < size; i++)
                {
                    project->correlationLengths.push_back(values[i]);
                }
            }
            else StoredObjectHandler::SetArrayValue(project, property_, values, size);
        }

        double GetIndexedValue(const std::shared_ptr<Reliability::LengthEffectProject>& project, const std::string& property_, int index) override
        {
            if (property_ == "correlation_lengths") return project->correlationLengths[index];
            else return StoredObjectHandler::GetIndexedValue(project, property_, index);
        }

        void Execute(const std::shared_ptr<Reliability::LengthEffectProject>& project, const std::string& method_) override
        {
            if (method_ == "run") project->run();
            else StoredObjectHandler::Execute(project, method_);
        }

        DesignPointHandler* designPointHandler = nullptr;
        SelfCorrelationMatrixHandler* selfCorrelationMatrixHandler = nullptr;
    };
}

