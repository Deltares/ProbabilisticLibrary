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

#include "CopulaCorrelationHandler.h"
#include "CorrelationMatrixHandler.h"
#include "ModelParameterHandler.h"
#include "StoredObjectHandler.h"
#include "ValidationReportHandler.h"
#include "../../Server/ProjectEntries.h"
#include "../../Model/ModelProject.h"

namespace Deltares::Server
{
    /**
     * \brief Handles properties and methods of class ModelProject
     */
    class ModelProjectHandler : public StoredObjectHandler<Models::ModelProject>
    {
    public:
        ObjectType GetObjectType() override
        {
            return ObjectType::Settings;
        }

        int GetIdValue(const std::shared_ptr<Models::ModelProject>& project, const std::string& property_) override
        {
            if (property_ == "validate") return validationReportHandler->GetObjectId(std::make_shared<Logging::ValidationReport>(project->getValidationReport()));
            else return StoredObjectHandler::GetIdValue(project, property_);
        }

        int GetIntValue(const std::shared_ptr<Models::ModelProject>& project, const std::string& property_) override
        {
            if (property_ == "index") return project->model->Index;
            else if (property_ == "stochasts_count") return static_cast<int>(project->stochasts.size());
            else if (property_ == "total_model_runs") return project->modelRuns;
            else return StoredObjectHandler::GetIntValue(project, property_);
        }

        void SetIntValue(const std::shared_ptr<Models::ModelProject>& project, const std::string& property_, int value) override
        {
            if (property_ == "correlation_matrix") project->correlation = correlationMatrixHandler->GetObject(value);
            else if (property_ == "copula_correlation") project->correlation = copulaCorrelationHandler->GetObject(value);
            else if (property_ == "share_project") project->shareStochasts(modelProjectCallback(value));
            else if (property_ == "total_model_runs") project->modelRuns = value;
            else StoredObjectHandler::SetIntValue(project, property_, value);
        }

        bool GetBoolValue(const std::shared_ptr<Models::ModelProject>& project, const std::string& property_) override
        {
            if (property_ == "is_valid") return project->isValid();
            else return StoredObjectHandler::GetBoolValue(project, property_);
        }

        void SetBoolValue(const std::shared_ptr<Models::ModelProject>& project, const std::string& property_, bool value) override
        {
            if (property_ == "callback_assigned") { if (project->model != nullptr) project->model->callbackAssigned = value; }
            else StoredObjectHandler::SetBoolValue(project, property_, value);
        }

        std::string GetStringValue(const std::shared_ptr<Models::ModelProject>& project, const std::string& property_) override
        {
            if (property_ == "model_name") return project->model->name;
            else return StoredObjectHandler::GetStringValue(project, property_);
        }

        void SetStringValue(const std::shared_ptr<Models::ModelProject>& project, const std::string& property_, const std::string& value) override
        {
            if (property_ == "model_name") project->model->name = value;
            else StoredObjectHandler::SetStringValue(project, property_, value);
        }

        int GetIndexedIdValue(const std::shared_ptr<Models::ModelProject>& project, const std::string& property_, int index) override
        {
            if (property_ == "stochasts") return stochastHandler->GetObjectId(project->stochasts[index]);
            else return StoredObjectHandler::GetIndexedIdValue(project, property_, index);
        }

        void SetArrayIntValue(const std::shared_ptr<Models::ModelProject>& project, const std::string& property_, int* values, int size) override
        {
            if (property_ == "variables")
            {
                project->stochasts.clear();
                for (int i = 0; i < size; i++)
                {
                    project->stochasts.push_back(stochastHandler->GetObject(values[i]));
                }
            }
            else if (property_ == "input_parameters")
            {
                project->model->inputParameters.clear();
                for (int i = 0; i < size; i++)
                {
                    project->model->inputParameters.push_back(modelParameterHandler->GetObject(values[i]));
                }
                project->updateStochasts();
            }
            else if (property_ == "output_parameters")
            {
                project->model->outputParameters.clear();
                for (int i = 0; i < size; i++)
                {
                    project->model->outputParameters.push_back(modelParameterHandler->GetObject(values[i]));
                }
            }
            else StoredObjectHandler::SetArrayIntValue(project, property_, values, size);
        }

        void Execute(const std::shared_ptr<Models::ModelProject>& project, const std::string& method_) override
        {
            if (method_ == "run") project->run();
            else if (method_ == "stop") project->stop();
            else StoredObjectHandler::Execute(project, method_);
        }

        void SetCallBack(const std::shared_ptr<Models::ModelProject>& project, const std::string& property_, Models::ZValuesCallBack callBack) override
        {
            if (property_ == "model") project->model = std::make_shared<Models::ZModel>(callBack);
        }

        void SetMultipleCallBack(const std::shared_ptr<Models::ModelProject>& project, const std::string& property_, Models::ZValuesMultipleCallBack callBack) override
        {
            if (property_ == "model")
            {
                if (project->model == nullptr)
                {
                    project->model = std::make_shared<Models::ZModel>();
                }

                project->model->setMultipleCallback(callBack);
            }
        }

        void SetEmptyCallBack(const std::shared_ptr<Models::ModelProject>& project, const std::string& property_, Models::EmptyCallBack callBack) override
        {
            if (property_ == "run_samples")
            {
                if (project->model == nullptr)
                {
                    project->model = std::make_shared<Models::ZModel>();
                }

                project->model->setRunMethod(callBack);
            }
            else if (property_ == "next")
            {
                if (project->model == nullptr)
                {
                    project->model = std::make_shared<Models::ZModel>();
                }

                project->model->setNextCalculation(callBack);
            }
        }

        void SetModelSampleCallBack(const std::shared_ptr<Models::ModelProject>& project, const std::string& property_, Models::ModelSampleCallback callBack) override
        {
            if (property_ == "model")
            {
                if (project->model == nullptr)
                {
                    project->model = std::make_shared<Models::ZModel>();
                }

                project->model->setModelSampleCallback(callBack);
            }
        }

        void SetMultipleModelSampleCallBack(const std::shared_ptr<Models::ModelProject>& project, const std::string& property_, Models::MultipleModelSampleCallback callBack) override
        {
            if (property_ == "model")
            {
                if (project->model == nullptr)
                {
                    project->model = std::make_shared<Models::ZModel>();
                }

                project->model->setMultipleModelSampleCallback(callBack);
            }
        }

        void SetProgressCallBacks(const std::shared_ptr<Models::ModelProject>& project, Models::ProgressCallBack progress, Models::DetailedProgressCallBack detailed, Models::TextualProgressCallBack textual) override
        {
            project->progressIndicator = std::make_shared<Models::ProgressIndicator>(progress, detailed, textual);
        }

        ValidationReportHandler* validationReportHandler = nullptr;
        ModelParameterHandler* modelParameterHandler = nullptr;
        StochastHandler* stochastHandler = nullptr;
        CorrelationMatrixHandler* correlationMatrixHandler = nullptr;
        CopulaCorrelationHandler* copulaCorrelationHandler = nullptr;

        GetObjectCallBack<Models::ModelProject> modelProjectCallback = nullptr;
    };
}

