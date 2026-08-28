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
#include "../../Server/ProjectEntries.h"
#include "../../Model/RunProject.h"

namespace Deltares::Server
{
    /**
     * \brief Handles properties and methods of class RunProject
     */
    class RunProjectHandler : public StoredObjectHandler<Models::RunProject>
    {
    public:
        ObjectType GetObjectType() override
        {
            return ObjectType::RunProject;
        }

        int GetIdValue(const std::shared_ptr<Models::RunProject>& project, const std::string& property_) override
        {
            if (property_ == "realization") return evaluationHandler->GetObjectId(project->evaluation);
            else return modelProjectHandler->GetIdValue(project, property_);
        }

        int GetIntValue(const std::shared_ptr<Models::RunProject>& project, const std::string& property_) override
        {
            return modelProjectHandler->GetIntValue(project, property_);
        }

        void SetIntValue(const std::shared_ptr<Models::RunProject>& project, const std::string& property_, int value) override
        {
            if (property_ == "settings") project->setSettings(runProjectSettingsHandler->GetObject(value));
            else modelProjectHandler->SetIntValue(project, property_, value);
        }

        bool GetBoolValue(const std::shared_ptr<Models::RunProject>& project, const std::string& property_) override
        {
            return modelProjectHandler->GetBoolValue(project, property_);
        }

        void SetBoolValue(const std::shared_ptr<Models::RunProject>& project, const std::string& property_, bool value) override
        {
            modelProjectHandler->SetBoolValue(project, property_, value);
        }

        std::string GetStringValue(const std::shared_ptr<Models::RunProject>& project, const std::string& property_) override
        {
            return modelProjectHandler->GetStringValue(project, property_);
        }

        void SetStringValue(const std::shared_ptr<Models::RunProject>& project, const std::string& property_, const std::string& value) override
        {
            modelProjectHandler->SetStringValue(project, property_, value);
        }

        int GetIndexedIdValue(const std::shared_ptr<Models::RunProject>& project, const std::string& property_, int index) override
        {
            return modelProjectHandler->GetIndexedIdValue(project, property_, index);
        }

        void SetArrayIntValue(const std::shared_ptr<Models::RunProject>& project, const std::string& property_, int* values, int size) override
        {
            modelProjectHandler->SetArrayIntValue(project, property_, values, size);
        }

        void Execute(const std::shared_ptr<Models::RunProject>& project, const std::string& method_) override
        {
            modelProjectHandler->Execute(project, method_);
        }

        void SetCallBack(const std::shared_ptr<Models::RunProject>& project, const std::string& property_, Models::ZValuesCallBack callBack) override
        {
            modelProjectHandler->SetCallBack(project, property_, callBack);
        }

        void SetMultipleCallBack(const std::shared_ptr<Models::RunProject>& project, const std::string& property_, Models::ZValuesMultipleCallBack callBack) override
        {
            modelProjectHandler->SetMultipleCallBack(project, property_, callBack);
        }

        void SetEmptyCallBack(const std::shared_ptr<Models::RunProject>& project, const std::string& property_, Models::EmptyCallBack callBack) override
        {
            modelProjectHandler->SetEmptyCallBack(project, property_, callBack);
        }

        void SetModelSampleCallBack(const std::shared_ptr<Models::RunProject>& project, const std::string& property_, Models::ModelSampleCallback callBack) override
        {
            modelProjectHandler->SetModelSampleCallBack(project, property_, callBack);
        }

        void SetMultipleModelSampleCallBack(const std::shared_ptr<Models::RunProject>& project, const std::string& property_, Models::MultipleModelSampleCallback callBack) override
        {
            modelProjectHandler->SetMultipleModelSampleCallBack(project, property_, callBack);
        }

        void SetProgressCallBacks(const std::shared_ptr<Models::RunProject>& project, Models::ProgressCallBack progress, Models::DetailedProgressCallBack detailed, Models::TextualProgressCallBack textual) override
        {
            modelProjectHandler->SetProgressCallBacks(project, progress, detailed, textual);
        }

        ModelProjectHandler* modelProjectHandler = nullptr;
        RunProjectSettingsHandler* runProjectSettingsHandler = nullptr;
        EvaluationHandler* evaluationHandler = nullptr;
    };
}

