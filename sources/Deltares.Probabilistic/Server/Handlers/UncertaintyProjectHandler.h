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
#include "../../Uncertainty/UncertaintyProject.h"

namespace Deltares::Server
{
    /**
     * \brief Handles properties and methods of class RunProject
     */
    class UncertaintyProjectHandler : public StoredObjectHandler<Uncertainty::UncertaintyProject>
    {
    public:
        ObjectType GetObjectType() override
        {
            return ObjectType::UncertaintyProject;
        }

        int GetIdValue(const std::shared_ptr<Uncertainty::UncertaintyProject>& project, const std::string& property_) override
        {
            if (property_ == "uncertainty_stochast") return stochastHandler->GetObjectId(project->uncertaintyResult->stochast);
            else if (property_ == "uncertainty_result") return uncertaintyResultHandler->GetObjectId(project->uncertaintyResult);
            else if (property_ == "output_correlation_matrix") return correlationMatrixHandler->GetObjectId(project->outputCorrelationMatrix);
            else return modelProjectHandler->GetIdValue(project, property_);
        }

        int GetIntValue(const std::shared_ptr<Uncertainty::UncertaintyProject>& project, const std::string& property_) override
        {
            if (property_ == "uncertainty_stochasts_count") return static_cast<int>(project->uncertaintyResults.size());
            else if (property_ == "uncertainty_results_count") return static_cast<int>(project->uncertaintyResults.size());
            else if (property_ == "uncertainty_parameters_count") return static_cast<int>(project->uncertaintyParameters.size());
            else return modelProjectHandler->GetIntValue(project, property_);
        }

        void SetIntValue(const std::shared_ptr<Uncertainty::UncertaintyProject>& project, const std::string& property_, int value) override
        {
            if (property_ == "settings") project->setSettings(uncertaintySettingsHandler->GetObject(value));
            else modelProjectHandler->SetIntValue(project, property_, value);
        }

        bool GetBoolValue(const std::shared_ptr<Uncertainty::UncertaintyProject>& project, const std::string& property_) override
        {
            return modelProjectHandler->GetBoolValue(project, property_);
        }

        void SetBoolValue(const std::shared_ptr<Uncertainty::UncertaintyProject>& project, const std::string& property_, bool value) override
        {
            modelProjectHandler->SetBoolValue(project, property_, value);
        }

        std::string GetStringValue(const std::shared_ptr<Uncertainty::UncertaintyProject>& project, const std::string& property_) override
        {
            if (property_ == "parameter") return project->parameter;
            else return modelProjectHandler->GetStringValue(project, property_);
        }

        void SetStringValue(const std::shared_ptr<Uncertainty::UncertaintyProject>& project, const std::string& property_, const std::string& value) override
        {
            if (property_ == "parameter") project->parameter = value;
            else modelProjectHandler->SetStringValue(project, property_, value);
        }

        int GetIndexedIdValue(const std::shared_ptr<Uncertainty::UncertaintyProject>& project, const std::string& property_, int index) override
        {
            if (property_ == "uncertainty_stochasts") return stochastHandler->GetObjectId(project->uncertaintyResults[index]->stochast);
            else if (property_ == "uncertainty_results") return uncertaintyResultHandler->GetObjectId(project->uncertaintyResults[index]);
            else if (property_ == "uncertainty_parameters") return modelParameterHandler->GetObjectId(project->uncertaintyParameters[index]);
            else return modelProjectHandler->GetIndexedIdValue(project, property_, index);
        }

        void SetArrayIntValue(const std::shared_ptr<Uncertainty::UncertaintyProject>& project, const std::string& property_, int* values, int size) override
        {
            if (property_ == "uncertainty_parameters")
            {
                project->uncertaintyParameters.clear();

                for (int i = 0; i < size; i++)
                {
                    project->uncertaintyParameters.push_back(modelParameterHandler->GetObject(values[i]));
                }
            }
            else modelProjectHandler->SetArrayIntValue(project, property_, values, size);
        }

        void Execute(const std::shared_ptr<Uncertainty::UncertaintyProject>& project, const std::string& method_) override
        {
            modelProjectHandler->Execute(project, method_);
        }

        void SetCallBack(const std::shared_ptr<Uncertainty::UncertaintyProject>& project, const std::string& property_, Models::ZValuesCallBack callBack) override
        {
            modelProjectHandler->SetCallBack(project, property_, callBack);
        }

        void SetMultipleCallBack(const std::shared_ptr<Uncertainty::UncertaintyProject>& project, const std::string& property_, Models::ZValuesMultipleCallBack callBack) override
        {
            modelProjectHandler->SetMultipleCallBack(project, property_, callBack);
        }

        void SetEmptyCallBack(const std::shared_ptr<Uncertainty::UncertaintyProject>& project, const std::string& property_, Models::EmptyCallBack callBack) override
        {
            modelProjectHandler->SetEmptyCallBack(project, property_, callBack);
        }

        void SetModelSampleCallBack(const std::shared_ptr<Uncertainty::UncertaintyProject>& project, const std::string& property_, Models::ModelSampleCallback callBack) override
        {
            modelProjectHandler->SetModelSampleCallBack(project, property_, callBack);
        }

        void SetMultipleModelSampleCallBack(const std::shared_ptr<Uncertainty::UncertaintyProject>& project, const std::string& property_, Models::MultipleModelSampleCallback callBack) override
        {
            modelProjectHandler->SetMultipleModelSampleCallBack(project, property_, callBack);
        }

        void SetProgressCallBacks(const std::shared_ptr<Uncertainty::UncertaintyProject>& project, Models::ProgressCallBack progress, Models::DetailedProgressCallBack detailed, Models::TextualProgressCallBack textual) override
        {
            modelProjectHandler->SetProgressCallBacks(project, progress, detailed, textual);
        }

        ModelProjectHandler* modelProjectHandler = nullptr;
        ModelParameterHandler* modelParameterHandler = nullptr;
        StochastHandler* stochastHandler = nullptr;
        UncertaintySettingsHandler* uncertaintySettingsHandler = nullptr;
        UncertaintyResultHandler* uncertaintyResultHandler = nullptr;
        CorrelationMatrixHandler* correlationMatrixHandler = nullptr;
    };
}

