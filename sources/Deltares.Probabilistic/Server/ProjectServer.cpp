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
#include "ProjectServer.h"

#include <iostream>
#include <string>
#include <memory>

namespace Deltares::Server
{
    using namespace Deltares::Statistics;
    using namespace Deltares::Reliability;

    void ProjectServer::SetHandler(std::shared_ptr<BaseHandler> handler)
    {
        this->handler = handler;
    }

    int ProjectServer::GetNewId()
    {
        return handler->GetNewId();
    }

    int ProjectServer::Create(std::string object_type)
    {
        if (handler->CanHandle(object_type))
        {
            return handler->Create(object_type);
        }

        throw probLibException("Object type \"" + object_type + "\" not supported");
    }

    void ProjectServer::Destroy(int id)
    {
        handler->Destroy(id);
    }

    void ProjectServer::Exit()
    {
        handler->Exit();
    }

    double ProjectServer::GetValue(int id, std::string property_)
    {
        return handler->GetValue(id, property_);
    }

    void ProjectServer::SetValue(int id, std::string property_, double value)
    {
        handler->SetValue(id, property_, value);
    }

    int ProjectServer::GetIntValue(int id, std::string property_)
    {
        return handler->GetIntValue(id, property_);
    }

    void ProjectServer::SetIntValue(int id, std::string property_, int value)
    {
        handler->SetIntValue(id, property_, value);
    }

    double ProjectServer::GetIntArgValue(int id1, int id2, std::string property_)
    {
        return handler->GetIntArgValue(id1, id2, property_);
    }

    void ProjectServer::SetIntArgValue(int id1, int id2, std::string property_, double value)
    {
        handler->SetIntArgValue(id1, id2, property_, value);
    }

    bool ProjectServer::GetBoolValue(int id, std::string property_)
    {
        return handler->GetBoolValue(id, property_);
    }

    void ProjectServer::SetBoolValue(int id, std::string property_, bool value)
    {
        handler->SetBoolValue(id, property_, value);
    }

    std::string ProjectServer::GetStringValue(int id, std::string property_)
    {
        return handler->GetStringValue(id, property_);
    }

    void ProjectServer::SetStringValue(int id, std::string property_, std::string value)
    {
        handler->SetStringValue(id, property_, value);
    }

    std::string ProjectServer::GetIndexedStringValue(int id, const std::string property_, int index)
    {
        return handler->GetIndexedStringValue(id, property_, index);
    }

    void ProjectServer::GetArrayValue(int id, std::string property_, double* values, int size)
    {
        return handler->GetArrayValue(id, property_, values, size);
    }

    void ProjectServer::SetArrayValue(int id, std::string property_, double* values, int size)
    {
        handler->SetArrayValue(id, property_, values, size);
    }

    std::vector<int> ProjectServer::GetArrayIntValue(int id, std::string property_)
    {
        return handler->GetArrayIntValue(id, property_);
    }

    void ProjectServer::SetArrayIntValue(int id, std::string property_, int* values, int size)
    {
        handler->SetArrayIntValue(id, property_, values, size);
    }

    double ProjectServer::GetArgValue(int id, std::string property_, double argument)
    {
        return handler->GetArgValue(id, property_, argument);
    }

    void ProjectServer::SetArgValue(int id, std::string property_, double argument, double value)
    {
        handler->SetArgValue(id, property_, argument, value);
    }

    void ProjectServer::GetArgValues(int id, std::string property_, double* values, int size, double* outputValues)
    {
        return handler->GetArgValues(id, property_, values, size, outputValues);
    }

    double ProjectServer::GetIndexedValue(int id, std::string property_, int index)
    {
        return handler->GetIndexedValue(id, property_, index);
    }

    void ProjectServer::SetIndexedValue(int id, std::string property_, int index, double value)
    {
        handler->SetIndexedValue(id, property_, index, value);
    }

    double ProjectServer::GetIndexedIndexedValue(int id, std::string property_, int index1, int index2)
    {
        return handler->GetIndexedIndexedValue(id, property_, index1, index2);
    }

    void ProjectServer::SetIndexedIndexedValue(int id, std::string property_, int index1, int index2, double value)
    {
        handler->SetIndexedIndexedValue(id, property_, index1, index2, value);
    }

    void ProjectServer::SetIndexedIndexedIntValue(int id, std::string property_, int index1, int index2, int value)
    {
        handler->SetIndexedIndexedIntValue(id, property_, index1, index2, value);
    }

    int ProjectServer::GetIndexedIntValue(int id, std::string property_, int index)
    {
        return handler->GetIndexedIntValue(id, property_, index);
    }

    int ProjectServer::GetIdValue(int id, std::string property_)
    {
        int objectId = handler->GetIdValue(id, property_);

        return objectId;
    }

    int ProjectServer::GetIndexedIdValue(int id, std::string property_, int index)
    {
        int objectId = handler->GetIndexedIdValue(id, property_, index);

        return objectId;
    }

    void ProjectServer::SetCallBack(int id, std::string property_, Models::ZValuesCallBack callBack)
    {
        handler->SetCallBack(id, property_, callBack);
    }

    void ProjectServer::SetMultipleCallBack(int id, std::string property_, Models::ZValuesMultipleCallBack callBack)
    {
        handler->SetMultipleCallBack(id, property_, callBack);
    }

    void ProjectServer::SetEmptyCallBack(int id, std::string property_, Models::EmptyCallBack callBack)
    {
        handler->SetEmptyCallBack(id, property_, callBack);
    }

    void ProjectServer::SetModelSampleCallBack(int id, std::string property_, Models::ModelSampleCallback callBack)
    {
        handler->SetModelSampleCallBack(id, property_, callBack);
    }

    void ProjectServer::SetMultipleModelSampleCallBack(int id, std::string property_, Models::MultipleModelSampleCallback callBack)
    {
        handler->SetMultipleModelSampleCallBack(id, property_, callBack);
    }

    void ProjectServer::SetProgressCallBacks(int id, Models::ProgressCallBack progress, Models::DetailedProgressCallBack detailed, Models::TextualProgressCallBack textual)
    {
        handler->SetProgressCallBacks(id, progress, detailed, textual);
    }

    void ProjectServer::Execute(int id, std::string method_)
    {
        handler->Execute(id, method_);
    }
}

