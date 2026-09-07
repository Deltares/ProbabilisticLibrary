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
#include "../../Utils/ProbabilisticLibraryException.h"

namespace Deltares::Server
{
    template<typename T>
    using GetObjectCallBack = std::function<std::shared_ptr<T>(int)>;

    template<typename T>
    using GetObjectIdCallBack = std::function<int(std::shared_ptr<T>)>;

    /**
     * \brief Base class for object handlers which contains a registration of all objects
     */
    template<typename T>
    class StoredObjectHandler : public ObjectHandler
    {
    private:
        std::unordered_map<int, std::shared_ptr<T>> objects;
        std::unordered_map<std::shared_ptr<T>, int> objectIds;

        ObjectHandlerAdmin* admin = nullptr;

        StoredObjectHandler<T>* baseHandler = nullptr;
        std::vector<GetObjectCallBack<T>> inheritors;

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

        template<typename U>
        requires std::derived_from<T, U>
        void SetBaseHandler(StoredObjectHandler<U>* baseHandler)
        {
            this->baseHandler = baseHandler;
            baseHandler->AddInheritor(this);
        }

        //template<typename T>
        void AddInheritor(StoredObjectHandler<T>* inheritor)
        {
            inheritors.emplace_back([inheritor](const int id) { return inheritor->GetObject(id); });
        }

        std::shared_ptr<T> GetObject(int id) const
        {
            if (id == 0)
            {
                return nullptr;
            }

            auto it = objects.find(id);
            std::shared_ptr<T> object = it != objects.end() ? it->second : nullptr;

            if (object != nullptr)
            {
                return object;
            }

            for (const auto& inheritor : inheritors)
            {
                object = inheritor(id);
                if (object != nullptr)
                {
                    return object;
                }
            }

            return nullptr;
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

        virtual double GetValue(const std::shared_ptr<T>& object, const std::string& property_)
        {
            if (baseHandler != nullptr) return baseHandler->GetValue(object, property_);
            else throw Reliability::ProbabilisticLibraryException("GetValue: unknown property " + property_ + " in " + ProjectEntries::GetObjectTypeString(GetObjectType()));
        }

        void SetValue(int id, const std::string& property_, double value) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return SetValue(object, property_, value);
        }

        virtual void SetValue(const std::shared_ptr<T>& object, const std::string& property_, double value)
        {
            if (baseHandler != nullptr) baseHandler->SetValue(object, property_, value);
            else throw Reliability::ProbabilisticLibraryException("SetValue: unknown property " + property_ + " in " + ProjectEntries::GetObjectTypeString(GetObjectType()));
        }

        // indexed double

        double GetIndexedValue(int id, const std::string& property_, int index) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return GetIndexedValue(object, property_, index);
        }

        virtual double GetIndexedValue(const std::shared_ptr<T>& object, const std::string& property_, int index)
        {
            if (baseHandler != nullptr) return baseHandler->GetIndexedValue(object, property_, index);
            else throw Reliability::ProbabilisticLibraryException("GetIndexedValue: unknown property " + property_ + " in " + ProjectEntries::GetObjectTypeString(GetObjectType()));
        }

        void SetIndexedValue(int id, const std::string& property_, int index, double value) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return SetIndexedValue(object, property_, index, value);
        }

        virtual void SetIndexedValue(const std::shared_ptr<T>& object, const std::string& property_, int index, double value)
        {
            if (baseHandler != nullptr) baseHandler->SetIndexedValue(object, property_, index, value);
            else throw Reliability::ProbabilisticLibraryException("SetIndexedValue: unknown property " + property_ + " in " + ProjectEntries::GetObjectTypeString(GetObjectType()));
        }

        // indexed int

        int GetIndexedIntValue(int id, const std::string& property_, int index) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return GetIndexedIntValue(object, property_, index);
        }

        virtual int GetIndexedIntValue(const std::shared_ptr<T>& object, const std::string& property_, int index)
        {
            if (baseHandler != nullptr) return baseHandler->GetIndexedIntValue(object, property_, index);
            else throw Reliability::ProbabilisticLibraryException("GetIndexedIntValue: unknown property " + property_ + " in " + ProjectEntries::GetObjectTypeString(GetObjectType()));
        }

        // indexed indexed double

        double GetIndexedIndexedValue(int id, const std::string& property_, int index1, int index2) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return GetIndexedIndexedValue(object, property_, index1, index2);
        }

        virtual double GetIndexedIndexedValue(const std::shared_ptr<T>& object, const std::string& property_, int index1, int index2)
        {
            if (baseHandler != nullptr) return baseHandler->GetIndexedIndexedValue(object, property_, index1, index2);
            else throw Reliability::ProbabilisticLibraryException("GetIndexedIndexedValue: unknown property " + property_ + " in " + ProjectEntries::GetObjectTypeString(GetObjectType()));
        }

        void SetIndexedIndexedValue(int id, const std::string& property_, int index1, int index2, double value) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return SetIndexedIndexedValue(object, property_, index1, index2, value);
        }

        virtual void SetIndexedIndexedValue(const std::shared_ptr<T>& object, const std::string& property_, int index1, int index2, double value)
        {
            if (baseHandler != nullptr) baseHandler->SetIndexedIndexedValue(object, property_, index1, index2, value);
            else throw Reliability::ProbabilisticLibraryException("SetIndexedIndexedValue: unknown property " + property_ + " in " + ProjectEntries::GetObjectTypeString(GetObjectType()));
        }

        // indexed indexed int

        void SetIndexedIndexedIntValue(int id, const std::string& property_, int index1, int index2, int value) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return SetIndexedIndexedIntValue(object, property_, index1, index2, value);
        }

        virtual void SetIndexedIndexedIntValue(const std::shared_ptr<T>& object, const std::string& property_, int index1, int index2, int value)
        {
            if (baseHandler != nullptr) baseHandler->SetIndexedIndexedIntValue(object, property_, index1, index2, value);
            else throw Reliability::ProbabilisticLibraryException("SetIndexedIndexedIntValue: unknown property " + property_ + " in " + ProjectEntries::GetObjectTypeString(GetObjectType()));
        }

        // arg

        double GetArgValue(int id, const std::string& property_, double argument) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return GetArgValue(object, property_, argument);
        }

        virtual double GetArgValue(const std::shared_ptr<T>& object, const std::string& property_, double argument)
        {
            if (baseHandler != nullptr) return baseHandler->GetArgValue(object, property_, argument);
            else throw Reliability::ProbabilisticLibraryException("GetArgValue: unknown property " + property_ + " in " + ProjectEntries::GetObjectTypeString(GetObjectType()));
        }

        void SetArgValue(int id, const std::string& property_, double argument, double value) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return SetArgValue(object, property_, argument, value);
        }

        virtual void SetArgValue(const std::shared_ptr<T>& object, const std::string& property_, double argument, double value)
        {
            if (baseHandler != nullptr) baseHandler->SetArgValue(object, property_, argument, value);
            else throw Reliability::ProbabilisticLibraryException("SetArgValue: unknown property " + property_ + " in " + ProjectEntries::GetObjectTypeString(GetObjectType()));
        }

        // int arg

        double GetIntArgValue(int id, int argument, const std::string& property_) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return GetIntArgValue(object, argument, property_);
        }

        virtual double GetIntArgValue(const std::shared_ptr<T>& object, int argument, const std::string& property_)
        {
            if (baseHandler != nullptr) return baseHandler->GetIntArgValue(object, argument, property_);
            else throw Reliability::ProbabilisticLibraryException("GetIntArgValue: unknown property " + property_ + " in " + ProjectEntries::GetObjectTypeString(GetObjectType()));
        }

        void SetIntArgValue(int id, int argument, const std::string& property_, double value) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return SetIntArgValue(object, argument, property_, value);
        }

        virtual void SetIntArgValue(const std::shared_ptr<T>& object, int argument, const std::string& property_, double value)
        {
            if (baseHandler != nullptr) baseHandler->SetIntArgValue(object, argument, property_, value);
            else throw Reliability::ProbabilisticLibraryException("SetIntArgValue: unknown property " + property_ + " in " + ProjectEntries::GetObjectTypeString(GetObjectType()));
        }

        // int

        int GetIntValue(int id, const std::string& property_) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return GetIntValue(object, property_);
        }

        virtual int GetIntValue(const std::shared_ptr<T>& object, const std::string& property_)
        {
            if (baseHandler != nullptr) return baseHandler->GetIntValue(object, property_);
            else throw Reliability::ProbabilisticLibraryException("GetIntValue: unknown property " + property_ + " in " + ProjectEntries::GetObjectTypeString(GetObjectType()));
        }

        void SetIntValue(int id, const std::string& property_, int value) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return SetIntValue(object, property_, value);
        }

        virtual void SetIntValue(const std::shared_ptr<T>& object, const std::string& property_, int value)
        {
            if (baseHandler != nullptr) baseHandler->SetIntValue(object, property_, value);
            else throw Reliability::ProbabilisticLibraryException("SetIntValue: unknown property " + property_ + " in " + ProjectEntries::GetObjectTypeString(GetObjectType()));
        }

        // id

        int GetIdValue(int id, const std::string& property_) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return GetIdValue(object, property_);
        }

        virtual int GetIdValue(const std::shared_ptr<T>& object, const std::string& property_)
        {
            if (baseHandler != nullptr) return baseHandler->GetIdValue(object, property_);
            else throw Reliability::ProbabilisticLibraryException("GetIdValue: unknown property " + property_ + " in " + ProjectEntries::GetObjectTypeString(GetObjectType()));
        }

        int GetIndexedIdValue(int id, const std::string& property_, int index) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return GetIndexedIdValue(object, property_, index);
        }

        virtual int GetIndexedIdValue(const std::shared_ptr<T>& object, const std::string& property_, int index)
        {
            if (baseHandler != nullptr) return baseHandler->GetIndexedIdValue(object, property_, index);
            else throw Reliability::ProbabilisticLibraryException("GetIndexedIdValue: unknown property " + property_ + " in " + ProjectEntries::GetObjectTypeString(GetObjectType()));
        }

        // array

        void SetArrayValue(int id, const std::string& property_, double* values, int size) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return SetArrayValue(object, property_, values, size);
        }

        virtual void SetArrayValue(const std::shared_ptr<T>& object, const std::string& property_, double* values, int size)
        {
            if (baseHandler != nullptr) baseHandler->SetArrayValue(object, property_, values, size);
            else throw Reliability::ProbabilisticLibraryException("SetArrayValue: unknown property " + property_ + " in " + ProjectEntries::GetObjectTypeString(GetObjectType()));
        }

        void SetArrayIntValue(int id, const std::string& property_, int* values, int size) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return SetArrayIntValue(object, property_, values, size);
        }

        virtual void SetArrayIntValue(const std::shared_ptr<T>& object, const std::string& property_, int* values, int size)
        {
            if (baseHandler != nullptr) baseHandler->SetArrayIntValue(object, property_, values, size);
            else throw Reliability::ProbabilisticLibraryException("SetArrayIntValue: unknown property " + property_ + " in " + ProjectEntries::GetObjectTypeString(GetObjectType()));
        }

        // bool

        bool GetBoolValue(int id, const std::string& property_) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return GetBoolValue(object, property_);
        }

        virtual bool GetBoolValue(const std::shared_ptr<T>& object, const std::string& property_)
        {
            if (baseHandler != nullptr) return baseHandler->GetBoolValue(object, property_);
            else throw Reliability::ProbabilisticLibraryException("GetBoolValue: unknown property " + property_ + " in " + ProjectEntries::GetObjectTypeString(GetObjectType()));
        }

        void SetBoolValue(int id, const std::string& property_, bool value) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return SetBoolValue(object, property_, value);
        }

        virtual void SetBoolValue(const std::shared_ptr<T>& object, const std::string& property_, bool value)
        {
            if (baseHandler != nullptr) baseHandler->SetBoolValue(object, property_, value);
            else throw Reliability::ProbabilisticLibraryException("SetBoolValue: unknown property " + property_ + " in " + ProjectEntries::GetObjectTypeString(GetObjectType()));
        }

        // std::string

        std::string GetStringValue(int id, const std::string& property_) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return GetStringValue(object, property_);
        }

        virtual std::string GetStringValue(const std::shared_ptr<T>& object, const std::string& property_)
        {
            if (baseHandler != nullptr) return baseHandler->GetStringValue(object, property_);
            else throw Reliability::ProbabilisticLibraryException("GetStringValue: unknown property " + property_ + " in " + ProjectEntries::GetObjectTypeString(GetObjectType()));
        }

        void SetStringValue(int id, const std::string& property_, const std::string& value) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return SetStringValue(object, property_, value);
        }

        virtual void SetStringValue(const std::shared_ptr<T>& object, const std::string& property_, const std::string& value)
        {
            if (baseHandler != nullptr) baseHandler->SetStringValue(object, property_, value);
            else throw Reliability::ProbabilisticLibraryException("SetStringValue: unknown property " + property_ + " in " + ProjectEntries::GetObjectTypeString(GetObjectType()));
        }

        // method

        void Execute(int id, const std::string& method_) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return Execute(object, method_);
        }

        virtual void Execute(const std::shared_ptr<T>& object, const std::string& method_)
        {
            if (baseHandler != nullptr) baseHandler->Execute(object, method_);
            else throw Reliability::ProbabilisticLibraryException("Execute: unknown method " + method_ + " in " + ProjectEntries::GetObjectTypeString(GetObjectType()));
        }

        // callbacks

        void SetCallBack(int id, const std::string& property_, Models::ZValuesCallBack callBack) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return SetCallBack(object, property_, callBack);
        }

        virtual void SetCallBack(const std::shared_ptr<T>& object, const std::string& property_, Models::ZValuesCallBack callBack)
        {
            if (baseHandler != nullptr) baseHandler->SetCallBack(object, property_, callBack);
            else throw Reliability::ProbabilisticLibraryException("GetCallBack: unknown property " + property_ + " in " + ProjectEntries::GetObjectTypeString(GetObjectType()));
        }

        void SetProgressCallBacks(int id, Models::ProgressCallBack progress, Models::DetailedProgressCallBack detailed, Models::TextualProgressCallBack textual) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return SetProgressCallBacks(object, progress, detailed, textual);
        }

        virtual void SetProgressCallBacks(const std::shared_ptr<T>& object, Models::ProgressCallBack progress, Models::DetailedProgressCallBack detailed, Models::TextualProgressCallBack textual)
        {
            if (baseHandler != nullptr) baseHandler->SetProgressCallBacks(object, progress, detailed, textual);
            else throw Reliability::ProbabilisticLibraryException("SetProgressCallBacks");
        }

        void SetMultipleCallBack(int id, const std::string& property_, Models::ZValuesMultipleCallBack callBack) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return SetMultipleCallBack(object, property_, callBack);
        }

        virtual void SetMultipleCallBack(const std::shared_ptr<T>& object, const std::string& property_, Models::ZValuesMultipleCallBack callBack)
        {
            if (baseHandler != nullptr) baseHandler->SetMultipleCallBack(object, property_, callBack);
            else throw Reliability::ProbabilisticLibraryException("GetMultipleCallBack: unknown property " + property_ + " in " + ProjectEntries::GetObjectTypeString(GetObjectType()));
        }

        void SetEmptyCallBack(int id, const std::string& property_, Models::EmptyCallBack callBack) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return SetEmptyCallBack(object, property_, callBack);
        }

        virtual void SetEmptyCallBack(const std::shared_ptr<T>& object, const std::string& property_, Models::EmptyCallBack callBack)
        {
            if (baseHandler != nullptr) baseHandler->SetEmptyCallBack(object, property_, callBack);
            else throw Reliability::ProbabilisticLibraryException("GetEmptyCallBack: unknown property " + property_ + " in " + ProjectEntries::GetObjectTypeString(GetObjectType()));
        }

        void SetModelSampleCallBack(int id, const std::string& property_, Models::ModelSampleCallback callBack) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return SetModelSampleCallBack(object, property_, callBack);
        }

        virtual void SetModelSampleCallBack(const std::shared_ptr<T>& object, const std::string& property_, Models::ModelSampleCallback callBack)
        {
            if (baseHandler != nullptr) baseHandler->SetModelSampleCallBack(object, property_, callBack);
            else throw Reliability::ProbabilisticLibraryException("GetModelSampleCallBack: unknown property " + property_ + " in " + ProjectEntries::GetObjectTypeString(GetObjectType()));
        }

        void SetMultipleModelSampleCallBack(int id, const std::string& property_, Models::MultipleModelSampleCallback callBack) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return SetMultipleModelSampleCallBack(object, property_, callBack);
        }

        virtual void SetMultipleModelSampleCallBack(const std::shared_ptr<T>& object, const std::string& property_, Models::MultipleModelSampleCallback callBack)
        {
            if (baseHandler != nullptr) baseHandler->SetMultipleModelSampleCallBack(object, property_, callBack);
            else throw Reliability::ProbabilisticLibraryException("GetMultipleModelSampleCallBack: unknown property " + property_ + " in " + ProjectEntries::GetObjectTypeString(GetObjectType()));
        }
    };
}

