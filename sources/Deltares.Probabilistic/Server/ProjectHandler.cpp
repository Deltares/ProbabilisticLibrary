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
#include "ProjectHandler.h"

namespace Deltares::Server
{
    ProjectHandler::ProjectHandler()
    {
        InitializeHandlers();
    }

    bool ProjectHandler::CanHandle(const std::string& object_type)
    {
        return ProjectEntries::CanHandle(object_type);
    }

    int ProjectHandler::GetNewId()
    {
        return admin.GetNewId();
    }

    int ProjectHandler::Create(const std::string& objectTypeString)
    {
        ObjectType objectType = ProjectEntries::GetType(objectTypeString);
        return handlers[objectType]->Create();
    }

    void ProjectHandler::Destroy(int id)
    {
        if (admin.Contains(id))
        {
            ObjectType objectType = admin.GetObjectType(id);
            handlers[objectType]->Destroy(id);
        }
    }

    bool ProjectHandler::ShouldClose()
    {
        return admin.IsEmpty();
    }

    double ProjectHandler::GetValue(int id, const std::string& property_)
    {
        ObjectType objectType = admin.GetObjectType(id);
        return handlers[objectType]->GetValue(id, property_);
    }

    void ProjectHandler::SetValue(int id, const std::string& property_, double value)
    {
        ObjectType objectType = admin.GetObjectType(id);
        handlers[objectType]->SetValue(id, property_, value);
    }

    std::string ProjectHandler::GetStringValue(int id, const std::string& property_)
    {
        ObjectType objectType = admin.GetObjectType(id);
        return handlers[objectType]->GetStringValue(id, property_);
    }

    void ProjectHandler::SetStringValue(int id, const std::string& property_, const std::string& value)
    {
        ObjectType objectType = admin.GetObjectType(id);
        handlers[objectType]->SetStringValue(id, property_, value);
    }

    int ProjectHandler::GetIntValue(int id, const std::string& property_)
    {
        ObjectType objectType = admin.GetObjectType(id);
        return handlers[objectType]->GetIntValue(id, property_);
    }

    int ProjectHandler::GetIdValue(int id, const std::string& property_)
    {
        ObjectType objectType = admin.GetObjectType(id);
        return handlers[objectType]->GetIdValue(id, property_);
    }

    void ProjectHandler::SetIntValue(int id, const std::string& property_, int value)
    {
        ObjectType objectType = admin.GetObjectType(id);
        handlers[objectType]->SetIntValue(id, property_, value);
    }

    bool ProjectHandler::GetBoolValue(int id, const std::string& property_)
    {
        ObjectType objectType = admin.GetObjectType(id);
        return handlers[objectType]->GetBoolValue(id, property_);
    }

    void ProjectHandler::SetBoolValue(int id, const std::string& property_, bool value)
    {
        ObjectType objectType = admin.GetObjectType(id);
        handlers[objectType]->SetBoolValue(id, property_, value);
    }

    double ProjectHandler::GetIntArgValue(int id1, int id2, const std::string& property_)
    {
        ObjectType objectType = admin.GetObjectType(id1);
        return handlers[objectType]->GetIntArgValue(id1, id2, property_);
    }

    void ProjectHandler::SetIntArgValue(int id1, int id2, const std::string& property_, double value)
    {
        ObjectType objectType = admin.GetObjectType(id1);
        return handlers[objectType]->SetIntArgValue(id1, id2, property_, value);
    }

    int ProjectHandler::GetIndexedIntValue(int id, const std::string& property_, int index)
    {
        return 0;
    }

    int ProjectHandler::GetIndexedIdValue(int id, const std::string& property_, int index)
    {
        ObjectType objectType = admin.GetObjectType(id);
        return handlers[objectType]->GetIndexedIdValue(id, property_, index);
    }

    void ProjectHandler::SetArrayValue(int id, const std::string& property_, double* values, int size)
    {
        ObjectType objectType = admin.GetObjectType(id);
        handlers[objectType]->SetArrayValue(id, property_, values, size);
    }

    std::vector<int> ProjectHandler::GetArrayIntValue(int id, const std::string& property_)
    {
        return std::vector<int>(0);
    }

    void ProjectHandler::SetArrayIntValue(int id, const std::string& property_, int* values, int size)
    {
        ObjectType objectType = admin.GetObjectType(id);
        handlers[objectType]->SetArrayIntValue(id, property_, values, size);
    }

    double ProjectHandler::GetArgValue(int id, const std::string& property_, double argument)
    {
        ObjectType objectType = admin.GetObjectType(id);
        return handlers[objectType]->GetArgValue(id, property_, argument);
    }

    void ProjectHandler::SetArgValue(int id, const std::string& property_, double argument, double value)
    {
        ObjectType objectType = admin.GetObjectType(id);
        handlers[objectType]->SetArgValue(id, property_, argument, value);
    }

    double ProjectHandler::GetIndexedValue(int id, const std::string& property_, int index)
    {
        ObjectType objectType = admin.GetObjectType(id);
        return handlers[objectType]->GetIndexedValue(id, property_, index);
    }

    void ProjectHandler::SetIndexedValue(int id, const std::string& property_, int index, double value)
    {
        // not needed yet
    }

    double ProjectHandler::GetIndexedIndexedValue(int id, const std::string& property_, int index1, int index2)
    {
        ObjectType objectType = admin.GetObjectType(id);
        return handlers[objectType]->GetIndexedIndexedValue(id, property_, index1, index2);
    }

    void ProjectHandler::SetIndexedIndexedValue(int id, const std::string& property_, int index1, int index2, double value)
    {
        ObjectType objectType = admin.GetObjectType(id);
        return handlers[objectType]->SetIndexedIndexedValue(id, property_, index1, index2, value);
    }

    void ProjectHandler::SetIndexedIndexedIntValue(int id, const std::string& property_, int index1, int index2, int value)
    {
        ObjectType objectType = admin.GetObjectType(id);
        return handlers[objectType]->SetIndexedIndexedIntValue(id, property_, index1, index2, value);
    }

    void ProjectHandler::Execute(int id, const std::string& method_)
    {
        ObjectType objectType = admin.GetObjectType(id);
        handlers[objectType]->Execute(id, method_);
    }

    void ProjectHandler::SetProgressCallBacks(int id, Models::ProgressCallBack progress, Models::DetailedProgressCallBack detailed, Models::TextualProgressCallBack textual)
    {
        ObjectType objectType = admin.GetObjectType(id);
        handlers[objectType]->SetProgressCallBacks(id, progress, detailed, textual);
    }

    void ProjectHandler::SetCallBack(int id, const std::string& property_, Models::ZValuesCallBack callback)
    {
        ObjectType objectType = admin.GetObjectType(id);
        handlers[objectType]->SetCallBack(id, property_, callback);
    }

    void ProjectHandler::SetMultipleCallBack(int id, const std::string& property_, Models::ZValuesMultipleCallBack callback)
    {
        ObjectType objectType = admin.GetObjectType(id);
        handlers[objectType]->SetMultipleCallBack(id, property_, callback);
    }

    void ProjectHandler::SetEmptyCallBack(int id, const std::string& property_, Models::EmptyCallBack callback)
    {
        ObjectType objectType = admin.GetObjectType(id);
        handlers[objectType]->SetEmptyCallBack(id, property_, callback);
    }

    void ProjectHandler::SetModelSampleCallBack(int id, const std::string& property_, Models::ModelSampleCallback callback)
    {
        ObjectType objectType = admin.GetObjectType(id);
        handlers[objectType]->SetModelSampleCallBack(id, property_, callback);
    }

    void ProjectHandler::SetMultipleModelSampleCallBack(int id, const std::string& property_, Models::MultipleModelSampleCallback callback)
    {
        ObjectType objectType = admin.GetObjectType(id);
        handlers[objectType]->SetMultipleModelSampleCallBack(id, property_, callback);
    }

    int ProjectHandler::GetStatus(const std::string& command) const
    {
        if (command == "count_entries")
        {
            return admin.GetSize();
        }
        return -1;
    }

    void ProjectHandler::InitializeHandlers()
    {
        modelHandlers.InitializeHandlers(handlers);
        statisticsHandlers.InitializeHandlers(handlers, &modelHandlers);
        reliabilityHandlers.InitializeHandlers(handlers, &modelHandlers, &statisticsHandlers);

        reliabilityHandlers.fragilityCurveHandler.designPointIdCallback = [this](const std::shared_ptr<Reliability::DesignPoint>& designPoint) {return this->reliabilityHandlers.designPointHandler.GetObjectId(designPoint); };

        statisticsHandlers.fragilityValueHandler.designPointCallback = [this](const int id) {return this->reliabilityHandlers.designPointHandler.GetObject(id); };
        statisticsHandlers.fragilityValueHandler.designPointIdCallback = [this](const std::shared_ptr<Reliability::DesignPoint>& designPoint) {return this->reliabilityHandlers.designPointHandler.GetObjectId(designPoint); };

        statisticsHandlers.alphaHandler.fragilityCurveHandler = &reliabilityHandlers.fragilityCurveHandler;

        reliabilityHandlers.designPointHandler.designPointIdsCallback = [this](const int id) {return this->GetDesignPointIds(id); };

        uncertaintyResultHandler.stochastHandler = &statisticsHandlers.stochastHandler;
        uncertaintyResultHandler.evaluationHandler = &modelHandlers.evaluationHandler;
        uncertaintyResultHandler.messageHandler = &modelHandlers.messageHandler;

        sensitivityResultHandler.sensitivityValueHandler = &sensitivityValueHandler;
        sensitivityResultHandler.evaluationHandler = &modelHandlers.evaluationHandler;
        sensitivityResultHandler.messageHandler = &modelHandlers.messageHandler;

        sensitivityValueHandler.stochastHandler = &statisticsHandlers.stochastHandler;

        sensitivitySettingsHandler.modelProjectSettingsHandler = &statisticsHandlers.modelProjectSettingsHandler;
        uncertaintySettingsHandler.modelProjectSettingsHandler = &statisticsHandlers.modelProjectSettingsHandler;
        uncertaintySettingsHandler.stochastSettingsHandler = &statisticsHandlers.stochastSettingsHandler;
        uncertaintySettingsHandler.probabilityValueHandler = &statisticsHandlers.probabilityValueHandler;

        statisticsHandlers.modelProjectHandler.modelProjectCallback = [this](const int id) { return this->GetProject(id); };

        sensitivityProjectHandler.modelProjectHandler = &statisticsHandlers.modelProjectHandler;
        sensitivityProjectHandler.sensitivitySettingsHandler = &sensitivitySettingsHandler;
        sensitivityProjectHandler.sensitivityResultHandler = &sensitivityResultHandler;
        sensitivityProjectHandler.modelParameterHandler = &modelHandlers.modelParameterHandler;

        uncertaintyProjectHandler.modelProjectHandler = &statisticsHandlers.modelProjectHandler;
        uncertaintyProjectHandler.modelParameterHandler = &modelHandlers.modelParameterHandler;
        uncertaintyProjectHandler.uncertaintySettingsHandler = &uncertaintySettingsHandler;
        uncertaintyProjectHandler.uncertaintyResultHandler = &uncertaintyResultHandler;
        uncertaintyProjectHandler.stochastHandler = &statisticsHandlers.stochastHandler;
        uncertaintyProjectHandler.correlationMatrixHandler = &statisticsHandlers.correlationMatrixHandler;

        handlers[ObjectType::UncertaintyResult] = &uncertaintyResultHandler;
        handlers[ObjectType::SensitivityResult] = &sensitivityResultHandler;
        handlers[ObjectType::SensitivityValue] = &sensitivityValueHandler;
        handlers[ObjectType::SensitivitySettings] = &sensitivitySettingsHandler;
        handlers[ObjectType::UncertaintySettings] = &uncertaintySettingsHandler;
        handlers[ObjectType::SensitivityProject] = &sensitivityProjectHandler;
        handlers[ObjectType::UncertaintyProject] = &uncertaintyProjectHandler;

        for (const auto& [objectType, handler] : handlers)
        {
            handler->SetAdmin(&this->admin);
        }
    }

    std::shared_ptr<Models::ModelProject> ProjectHandler::GetProject(int id)
    {
        if (statisticsHandlers.runProjectHandler.Contains(id))
        {
            return statisticsHandlers.runProjectHandler.GetObject(id);
        }
        else if (sensitivityProjectHandler.Contains(id))
        {
            return sensitivityProjectHandler.GetObject(id);
        }
        else if (uncertaintyProjectHandler.Contains(id))
        {
            return uncertaintyProjectHandler.GetObject(id);
        }
        else if (reliabilityHandlers.reliabilityProjectHandler.Contains(id))
        {
            return reliabilityHandlers.reliabilityProjectHandler.GetObject(id);
        }
        else
        {
            return nullptr;
        }
    }

    std::shared_ptr<Reliability::DesignPointIds> ProjectHandler::GetDesignPointIds(int id)
    {
        return nullptr;
    }
}

