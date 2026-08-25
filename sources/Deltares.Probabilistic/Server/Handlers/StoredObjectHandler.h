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

        virtual ObjectType GetObjectType() = 0;

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

        // double

        double GetValue(int id, const std::string& property_) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return GetValue(object, property_);
        }

        virtual double GetValue(const std::shared_ptr<T>& object, const std::string& property_) { return 0; }

        void SetValue(int id, const std::string& property_, double value) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return SetValue(object, property_);
        }

        virtual void SetValue(const std::shared_ptr<T>& object, const std::string& property_, double value) {}

        // indexed double

        double GetIndexedValue(int id, const std::string& property_, int index) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return GetIndexedValue(object, property_);
        }

        virtual double GetIndexedValue(const std::shared_ptr<T>& object, const std::string& property_, int index) { return 0; }

        void SetIndexedValue(int id, const std::string& property_, int index, double value) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return SetIndexedValue(object, property_);
        }

        virtual void SetIndexedValue(const std::shared_ptr<T>& object, const std::string& property_, int index, double value) {}

        // arg

        double GetArgValue(int id, const std::string& property_, double argument) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return GetArgValue(object, property_, argument);
        }

        virtual double GetArgValue(const std::shared_ptr<T>& object, const std::string& property_, double argument) { return 0; }

        void SetArgValue(int id, const std::string& property_, double argument, double value) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return SetArgValue(object, property_, argument, value);
        }

        virtual void SetArgValue(const std::shared_ptr<T>& object, const std::string& property_, double argument, double value) {}

        // int

        int GetIntValue(int id, const std::string& property_) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return GetIntValue(object, property_);
        }

        virtual int GetIntValue(const std::shared_ptr<T>& object, const std::string& property_) { return 0; }

        void SetIntValue(int id, const std::string& property_, int value) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return SetIntValue(object, property_, value);
        }

        virtual void SetIntValue(const std::shared_ptr<T>& object, const std::string& property_, int value) {}

        // id

        int GetIdValue(int id, const std::string& property_) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return GetIdValue(object, property_);
        }

        virtual int GetIdValue(const std::shared_ptr<T>& object, const std::string& property_) { return 0; }

        int GetIndexedIdValue(int id, const std::string& property_, int index) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return GetIndexedIdValue(object, property_, index);
        }

        virtual int GetIndexedIdValue(const std::shared_ptr<T>& object, const std::string& property_, int index) { return 0; }

        // array

        void SetArrayValue(int id, const std::string& property_, double* value, int size) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return SetArrayValue(object, property_, value, size);
        }

        virtual void SetArrayValue(const std::shared_ptr<T>& object, const std::string& property_, double* value, int size) {}

        void SetArrayIntValue(int id, const std::string& property_, int* value, int size) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return SetArrayValue(object, property_, value, size);
        }

        virtual void SetArrayIntValue(const std::shared_ptr<T>& object, const std::string& property_, int* value, int size) {}

        // bool

        bool GetBoolValue(int id, const std::string& property_) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return GetBoolValue(object, property_);
        }

        virtual bool GetBoolValue(const std::shared_ptr<T>& object, const std::string& property_) { return false; }

        void SetBoolValue(int id, const std::string& property_, bool value) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return SetBoolValue(object, property_, value);
        }

        virtual void SetBoolValue(const std::shared_ptr<T>& object, const std::string& property_, bool value) { }

        // std::string

        std::string GetStringValue(int id, const std::string& property_) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return GetStringValue(object, property_);
        }

        virtual std::string GetStringValue(const std::shared_ptr<T>& object, const std::string& property_) { return ""; }

        void SetStringValue(int id, const std::string& property_, const std::string& value) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return SetStringValue(object, property_, value);
        }

        virtual void SetStringValue(const std::shared_ptr<T>& object, const std::string& property_, const std::string& value) {}

        // method

        void Execute(int id, const std::string& method_) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return Execute(object, method_);
        }

        virtual void Execute(const std::shared_ptr<T>& object, const std::string& method_) {}
    };
}

