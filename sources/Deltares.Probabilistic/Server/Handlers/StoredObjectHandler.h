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

#include <unordered_map>
#include "ObjectHandler.h"
#include "../ProjectEntries.h"

namespace Deltares::Server
{
    template<typename T>

    /**
     * \brief Base class for object handlers which contains a registration of all objects
     */
    class StoredObjectHandler : public ObjectHandler
    {
    private:
        std::unordered_map<int, std::shared_ptr<T>> objects;
        std::unordered_map<std::shared_ptr<T>, int> objectIds;

        ObjectHandlerAdmin* admin = nullptr;

    public:

        void SetAdmin(ObjectHandlerAdmin* admin) override
        {
            this->admin = admin;
        }

        int Create() override
        {
            std::shared_ptr<T> value = std::make_shared<T>();

            int id = admin->GetNewId();

            objects[id] = value;
            objectIds[value] = id;

            admin->RegisterType(id, GetObjectType());

            return id;
        }

        void Destroy(int id) override
        {
            auto it = objects.find(id);
            if (it != objects.end())
            {
                objectIds.erase(it->second);
                objects.erase(it);

                admin->Remove(id);
            }
        }

        std::shared_ptr<T> GetObject(int id) const
        {
            if (id == 0)
            {
                return nullptr;
            }

            auto it = objects.find(id);
            return it != objects.end() ? it->second : nullptr;
        }

        int GetObjectId(const std::shared_ptr<T>& object)
        {
            if (object == nullptr)
            {
                return 0;
            }

            if (!objectIds.contains(object))
            {
                int newId = admin->GetNewId();

                objects[newId] = object;
                objectIds[object] = newId;

                admin->RegisterType(newId, GetObjectType());
            }

            return objectIds.at(object);
        }

        bool Contains(int id)
        {
            return objects.contains(id);
        }

        bool ContainsObject(const std::shared_ptr<T>& object)
        {
            return objectIds.contains(object);
        }

        virtual ObjectType GetObjectType() = 0;
    };
}

