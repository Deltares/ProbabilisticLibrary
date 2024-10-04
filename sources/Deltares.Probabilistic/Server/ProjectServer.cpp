// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of Streams.
//
// Streams is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// All names, logos, and references to "Deltares" are registered trademarks of
// Stichting Deltares and remain full property of Stichting Deltares at all times.
// All rights reserved.
//
#include "ProjectServer.h"

#include <complex>
#include <string>
#include <memory>

namespace Deltares
{
    namespace Server
    {
        using namespace Deltares::Statistics;
        using namespace Deltares::Reliability;

        void ProjectServer::AddHandler(std::shared_ptr<BaseHandler> handler)
        {
            int handlerIndex = handlers.size();
            handlers.push_back(handler);
            handler->SetServer(std::shared_ptr<BaseServer>(this), handlerIndex, handlers[0]);
        }

        int ProjectServer::GetNewObjectId(int handlerIndex)
        {
            id_ += 1;
            handlersTable[id_] = this->handlers[handlerIndex];
            return id_;
        }

        int ProjectServer::Create(std::string object_type)
        {
            for (int i = 0; i < this->handlers.size(); i++)
            {
                if (this->handlers[i]->CanHandle(object_type))
                {
                    int counter = GetNewObjectId(i);
                    this->handlers[i]->Create(object_type, counter);

                    return counter;
                }
            }

            throw probLibException("Object type not supported");
        }

        void ProjectServer::Destroy(int id)
        {
            handlersTable[id]->Destroy(id);
        }

        double ProjectServer::GetValue(int id, std::string property_)
        {
            return handlersTable[id]->GetValue(id, property_);
        }

        void ProjectServer::SetValue(int id, std::string property_, double value)
        {
            handlersTable[id]->SetValue(id, property_, value);
        }

        int ProjectServer::GetIntValue(int id, std::string property_)
        {
            return handlersTable[id]->GetIntValue(id, property_);
        }

        void ProjectServer::SetIntValue(int id, std::string property_, int value)
        {
            handlersTable[id]->SetIntValue(id, property_, value);
        }

        double ProjectServer::GetIntArgValue(int id1, int id2, std::string property_)
        {
            return handlersTable[id1]->GetIntArgValue(id1, id2, property_);
        }

        void ProjectServer::SetIntArgValue(int id1, int id2, std::string property_, double value)
        {
            handlersTable[id1]->SetIntArgValue(id1, id2, property_, value);
        }

        bool ProjectServer::GetBoolValue(int id, std::string property_)
        {
            return handlersTable[id]->GetBoolValue(id, property_);
        }

        void ProjectServer::SetBoolValue(int id, std::string property_, bool value)
        {
            handlersTable[id]->SetBoolValue(id, property_, value);
        }

        std::string ProjectServer::GetStringValue(int id, std::string property_)
        {
            return handlersTable[id]->GetStringValue(id, property_);
        }

        void ProjectServer::SetStringValue(int id, std::string property_, std::string value)
        {
            handlersTable[id]->SetStringValue(id, property_, value);
        }

        void ProjectServer::SetArrayValue(int id, std::string property_, double* values, int size)
        {
            handlersTable[id]->SetArrayValue(id, property_, values, size);
        }

        std::vector<int> ProjectServer::GetArrayIntValue(int id, std::string property_)
        {
            return handlersTable[id]->GetArrayIntValue(id, property_);
        }

        void ProjectServer::SetArrayIntValue(int id, std::string property_, int* values, int size)
        {
            handlersTable[id]->SetArrayIntValue(id, property_, values, size);
        }

        double ProjectServer::GetArgValue(int id, std::string property_, double argument)
        {
            return handlersTable[id]->GetArgValue(id, property_, argument);
        }

        void ProjectServer::SetArgValue(int id, std::string property_, double argument, double value)
        {
            handlersTable[id]->SetArgValue(id, property_, argument, value);
        }

        double ProjectServer::GetIndexedValue(int id, std::string property_, int index)
        {
            return handlersTable[id]->GetIndexedValue(id, property_, index);
        }

        void ProjectServer::SetIndexedValue(int id, std::string property_, int index, double value)
        {
            handlersTable[id]->SetIndexedValue(id, property_, index, value);
        }

        double ProjectServer::GetIndexedIndexedValue(int id, std::string property_, int index1, int index2)
        {
            return handlersTable[id]->GetIndexedIndexedValue(id, property_, index1, index2);
        }

        void ProjectServer::SetIndexedIndexedValue(int id, std::string property_, int index1, int index2, double value)
        {
            handlersTable[id]->SetIndexedIndexedValue(id, property_, index1, index2, value);
        }

        int ProjectServer::GetIndexedIntValue(int id, std::string property_, int index)
        {
            return handlersTable[id]->GetIndexedIntValue(id, property_, index);
        }

        void ProjectServer::SetCallBack(int id, std::string property_, ZValuesCallBack callBack)
        {
            handlersTable[id]->SetCallBack(id, property_, callBack);
        }

        void ProjectServer::SetEmptyCallBack(int id, std::string property_, ZEmptyCallBack callBack)
        {
            handlersTable[id]->SetEmptyCallBack(id, property_, callBack);
        }

        void ProjectServer::Execute(int id, std::string method_)
        {
            handlersTable[id]->Execute(id, method_);
        }

        ZLambda ProjectServer::GetCallBack(int id, std::string method_)
        {
            return handlersTable[id]->GetCallBack(id, method_);
        }
    }
}

