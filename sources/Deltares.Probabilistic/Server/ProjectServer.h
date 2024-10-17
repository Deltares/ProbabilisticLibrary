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
#pragma once

#include "BaseHandler.h"
#include "BaseServer.h"
#include "ProjectHandler.h"

namespace Deltares
{
    namespace Server
    {
        class ProjectServer : public BaseServer
        {
        public:
            ProjectServer()
            {
                std::shared_ptr<BaseHandler> defaultHandler = std::make_shared<ProjectHandler>();
                this->AddHandler(defaultHandler);
            }

            ProjectServer(std::shared_ptr<BaseHandler> handler) : ProjectServer()
            {
                this->AddHandler(handler);
            }

            int GetNewObjectId(int handlerIndex) override;
            int Create(std::string object_type);
            void Destroy(int id);
            double GetValue(int id, const std::string property_);
            void SetValue(int id, const std::string property_, double value);
            int GetIntValue(int id, std::string property_);
            void SetIntValue(int id, std::string property_, int value);
            double GetIntArgValue(int id1, int id2, std::string property_);
            void SetIntArgValue(int id1, int id2, std::string property_, double value);
            bool GetBoolValue(int id, std::string property_);
            void SetBoolValue(int id, std::string property_, bool value);
            std::string GetStringValue(int id, std::string property_);
            std::string GetIndexedStringValue(int id, const std::string property_, int index);
            void SetStringValue(int id, std::string property_, std::string value);
            void SetArrayValue(int id, std::string property_, double* values, int size);
            std::vector<int> GetArrayIntValue(int id, std::string property_);
            void SetArrayIntValue(int id, std::string property_, int* values, int size);
            double GetArgValue(int id, std::string property_, double argument);
            void GetArgValues(int id, std::string property_, double* values, int size, double* outputValues);
            void SetArgValue(int id, std::string property_, double argument, double value);
            double GetIndexedValue(int id, std::string property_, int index);
            void SetIndexedValue(int id, std::string property_, int index, double value);
            double GetIndexedIndexedValue(int id, std::string property_, int index1, int index2);
            void SetIndexedIndexedValue(int id, std::string property_, int index1, int index2, double value);
            int GetIndexedIntValue(int id, std::string property_, int index);
            void SetCallBack(int id, std::string property_, Models::ZValuesCallBack callBack);
            void Execute(int id, std::string method_);
            void AddHandler(std::shared_ptr<BaseHandler> handler);

        private:
            int id_ = 0;
            std::vector<std::shared_ptr<BaseHandler>> handlers;
            std::unordered_map<int, std::shared_ptr<BaseHandler>> handlersTable;
        };
    }
}
