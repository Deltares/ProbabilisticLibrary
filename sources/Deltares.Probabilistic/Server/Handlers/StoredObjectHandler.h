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

#include <type_traits>
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
     * \brief Handles generic setting and getting of objects of a specific type <T>
     * \remark This class is inherited usually for a specific type and overrides setters and getters (GetValue, SetValue, etc.). 
     */
    template<typename T>
    class StoredObjectHandler : public ObjectHandler
    {
    private:
        std::unordered_map<int, std::shared_ptr<T>> objects;
        std::unordered_map<std::shared_ptr<T>, int> object_ids;

        ObjectHandlerAdmin* admin = nullptr;

        std::vector<GetObjectCallBack<T>> inheritedObjectCallbacks;
        std::vector<GetObjectIdCallBack<T>> inheritedIdCallbacks;

    public:

        virtual ObjectType GetObjectType() = 0;

        /**
         * Registers callbacks to inherited handlers
         * \param object_callback Callback to GetObject method
         * \param id_callback Callback to GetObjectId method
         * \remarks This method should only be used by DerivedObjectHandler
         **/
        void AddInheritor(const GetObjectCallBack<T>& object_callback, const GetObjectIdCallBack<T>& id_callback)
        {
            inheritedObjectCallbacks.push_back(object_callback);
            inheritedIdCallbacks.push_back(id_callback);
        }

        /**
         * Sets the central administration for ids and project types
         * \param admin Central administration
         * \remarks This method should only be used by the ProjectHandler
         */
        void SetAdmin(ObjectHandlerAdmin* admin) override
        {
            this->admin = admin;
        }

        /**
         * Creates an object of type T and keeps it, so that it can be found with its id
         * \return Id corresponding with created object
         */
        int Create() override
        {
            if constexpr (std::is_abstract_v<T>)
            {
                throw Reliability::ProbabilisticLibraryException("Cannot create abstract class " + ProjectEntries::GetObjectTypeString(GetObjectType()));
            }
            else
            {
                std::shared_ptr<T> value = std::make_shared<T>();

                int id = admin->GetNewId();

                objects[id] = value;
                object_ids[value] = id;

                admin->RegisterType(id, GetObjectType());

                return id;
            }
        }

        /**
         * Removes an object from the administration held in this object
         * \param id Id of the object to be removed
         * \remarks A call to this method usually originates from a garbage collector
         */
        void Destroy(int id) override
        {
            auto it = objects.find(id);
            if (it != objects.end())
            {
                object_ids.erase(it->second);
                objects.erase(it);

                admin->Remove(id);
            }
        }

        /**
         * Gets the object associated with an id
         * \param id The id associated with the object
         * \return The object associated with the id
         * \remarks Inheritors of type T are also searched for the object (if the handler is a DerivedObjectHandler)
         */
        std::shared_ptr<T> GetObject(int id)
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

            // look in the inheritors if it is present there
            for (const auto& inheritor : inheritedObjectCallbacks)
            {
                object = inheritor(id);
                if (object != nullptr)
                {
                    return object;
                }
            }

            return nullptr;
        }

        /**
         * Gets objects associated with a vector of ids
         * \param ids The ids associated with the objects
         * \param size Number of ids
         * \return Vector of objects associated with the ids
         * \remarks Inheritors of type T are also searched for the object (if the handler is a DerivedObjectHandler)
         */
        std::vector<std::shared_ptr<T>> GetObjects(const int* ids, int size)
        {
            std::vector<std::shared_ptr<T>> id_objects = std::vector<std::shared_ptr<T>>(size);
            for (int i = 0; i < size; i++)
            {
                id_objects[i] = GetObject(ids[i]);
            }

            return id_objects;
        }

        /**
         * Gets the id associated with an object
         * \param object The id associated with the object
         * \return The id associated with the object, 0 if the object is a nullptr and possibly a new id is created if it was not registered before
         * \remarks Inheritors of type T are also searched for the id (if the handler is a DerivedObjectHandler)
         */
        int GetObjectId(const std::shared_ptr<T>& object)
        {
            if (object == nullptr)
            {
                return 0;
            }

            auto it = object_ids.find(object);
            int id = it != object_ids.end() ? it->second : 0;

            if (id != 0)
            {
                return id;
            }

            // look in the inheritors if it is present there (or generate an id in the inheritor)
            for (const auto& inheritor : inheritedIdCallbacks)
            {
                id = inheritor(object);
                if (id != 0)
                {
                    return id;
                }
            }

            // not found, but the object exists, then generate a new id
            if (id == 0)
            {
                id = admin->GetNewId();

                objects[id] = object;
                object_ids[object] = id;

                admin->RegisterType(id, GetObjectType());
            }

            return id;
        }

        // double

        double GetValue(int id, const std::string& property_) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return GetValue(object, property_);
        }

        virtual double GetValue(const std::shared_ptr<T>& object, const std::string& property_)
        {
            throw Reliability::ProbabilisticLibraryException("GetValue: unknown property " + property_ + " in " + ProjectEntries::GetObjectTypeString(GetObjectType()));
        }

        void SetValue(int id, const std::string& property_, double value) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return SetValue(object, property_, value);
        }

        virtual void SetValue(const std::shared_ptr<T>& object, const std::string& property_, double value)
        {
            throw Reliability::ProbabilisticLibraryException("SetValue: unknown property " + property_ + " in " + ProjectEntries::GetObjectTypeString(GetObjectType()));
        }

        // indexed double

        double GetIndexedValue(int id, const std::string& property_, int index) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return GetIndexedValue(object, property_, index);
        }

        virtual double GetIndexedValue(const std::shared_ptr<T>& object, const std::string& property_, int index)
        {
            throw Reliability::ProbabilisticLibraryException("GetIndexedValue: unknown property " + property_ + " in " + ProjectEntries::GetObjectTypeString(GetObjectType()));
        }

        void SetIndexedValue(int id, const std::string& property_, int index, double value) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return SetIndexedValue(object, property_, index, value);
        }

        virtual void SetIndexedValue(const std::shared_ptr<T>& object, const std::string& property_, int index, double value)
        {
            throw Reliability::ProbabilisticLibraryException("SetIndexedValue: unknown property " + property_ + " in " + ProjectEntries::GetObjectTypeString(GetObjectType()));
        }

        // indexed int

        int GetIndexedIntValue(int id, const std::string& property_, int index) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return GetIndexedIntValue(object, property_, index);
        }

        virtual int GetIndexedIntValue(const std::shared_ptr<T>& object, const std::string& property_, int index)
        {
            throw Reliability::ProbabilisticLibraryException("GetIndexedIntValue: unknown property " + property_ + " in " + ProjectEntries::GetObjectTypeString(GetObjectType()));
        }

        // indexed indexed double

        double GetIndexedIndexedValue(int id, const std::string& property_, int index1, int index2) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return GetIndexedIndexedValue(object, property_, index1, index2);
        }

        virtual double GetIndexedIndexedValue(const std::shared_ptr<T>& object, const std::string& property_, int index1, int index2)
        {
            throw Reliability::ProbabilisticLibraryException("GetIndexedIndexedValue: unknown property " + property_ + " in " + ProjectEntries::GetObjectTypeString(GetObjectType()));
        }

        void SetIndexedIndexedValue(int id, const std::string& property_, int index1, int index2, double value) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return SetIndexedIndexedValue(object, property_, index1, index2, value);
        }

        virtual void SetIndexedIndexedValue(const std::shared_ptr<T>& object, const std::string& property_, int index1, int index2, double value)
        {
            throw Reliability::ProbabilisticLibraryException("SetIndexedIndexedValue: unknown property " + property_ + " in " + ProjectEntries::GetObjectTypeString(GetObjectType()));
        }

        // indexed indexed int

        void SetIndexedIndexedIntValue(int id, const std::string& property_, int index1, int index2, int value) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return SetIndexedIndexedIntValue(object, property_, index1, index2, value);
        }

        virtual void SetIndexedIndexedIntValue(const std::shared_ptr<T>& object, const std::string& property_, int index1, int index2, int value)
        {
            throw Reliability::ProbabilisticLibraryException("SetIndexedIndexedIntValue: unknown property " + property_ + " in " + ProjectEntries::GetObjectTypeString(GetObjectType()));
        }

        // arg

        double GetArgValue(int id, const std::string& property_, double argument) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return GetArgValue(object, property_, argument);
        }

        virtual double GetArgValue(const std::shared_ptr<T>& object, const std::string& property_, double argument)
        {
            throw Reliability::ProbabilisticLibraryException("GetArgValue: unknown property " + property_ + " in " + ProjectEntries::GetObjectTypeString(GetObjectType()));
        }

        void SetArgValue(int id, const std::string& property_, double argument, double value) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return SetArgValue(object, property_, argument, value);
        }

        virtual void SetArgValue(const std::shared_ptr<T>& object, const std::string& property_, double argument, double value)
        {
            throw Reliability::ProbabilisticLibraryException("SetArgValue: unknown property " + property_ + " in " + ProjectEntries::GetObjectTypeString(GetObjectType()));
        }

        // int arg

        double GetIntArgValue(int id, int argument, const std::string& property_) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return GetIntArgValue(object, argument, property_);
        }

        virtual double GetIntArgValue(const std::shared_ptr<T>& object, int argument, const std::string& property_)
        {
            throw Reliability::ProbabilisticLibraryException("GetIntArgValue: unknown property " + property_ + " in " + ProjectEntries::GetObjectTypeString(GetObjectType()));
        }

        void SetIntArgValue(int id, int argument, const std::string& property_, double value) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return SetIntArgValue(object, argument, property_, value);
        }

        virtual void SetIntArgValue(const std::shared_ptr<T>& object, int argument, const std::string& property_, double value)
        {
            throw Reliability::ProbabilisticLibraryException("SetIntArgValue: unknown property " + property_ + " in " + ProjectEntries::GetObjectTypeString(GetObjectType()));
        }

        // int

        int GetIntValue(int id, const std::string& property_) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return GetIntValue(object, property_);
        }

        virtual int GetIntValue(const std::shared_ptr<T>& object, const std::string& property_)
        {
            throw Reliability::ProbabilisticLibraryException("GetIntValue: unknown property " + property_ + " in " + ProjectEntries::GetObjectTypeString(GetObjectType()));
        }

        void SetIntValue(int id, const std::string& property_, int value) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return SetIntValue(object, property_, value);
        }

        virtual void SetIntValue(const std::shared_ptr<T>& object, const std::string& property_, int value)
        {
            throw Reliability::ProbabilisticLibraryException("SetIntValue: unknown property " + property_ + " in " + ProjectEntries::GetObjectTypeString(GetObjectType()));
        }

        // id

        int GetIdValue(int id, const std::string& property_) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return GetIdValue(object, property_);
        }

        virtual int GetIdValue(const std::shared_ptr<T>& object, const std::string& property_)
        {
            throw Reliability::ProbabilisticLibraryException("GetIdValue: unknown property " + property_ + " in " + ProjectEntries::GetObjectTypeString(GetObjectType()));
        }

        int GetIndexedIdValue(int id, const std::string& property_, int index) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return GetIndexedIdValue(object, property_, index);
        }

        virtual int GetIndexedIdValue(const std::shared_ptr<T>& object, const std::string& property_, int index)
        {
            throw Reliability::ProbabilisticLibraryException("GetIndexedIdValue: unknown property " + property_ + " in " + ProjectEntries::GetObjectTypeString(GetObjectType()));
        }

        void SetIdValues(std::vector<std::shared_ptr<T>>& list, int* ids, int size)
        {
            list.clear();
            for (int i = 0; i < size; i++)
            {
                std::shared_ptr<T> value = GetObject(ids[i]);
                list.push_back(value);
            }
        }

        // array

        void SetArrayValue(int id, const std::string& property_, double* values, int size) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return SetArrayValue(object, property_, values, size);
        }

        virtual void SetArrayValue(const std::shared_ptr<T>& object, const std::string& property_, double* values, int size)
        {
            throw Reliability::ProbabilisticLibraryException("SetArrayValue: unknown property " + property_ + " in " + ProjectEntries::GetObjectTypeString(GetObjectType()));
        }

        void SetArrayIntValue(int id, const std::string& property_, int* values, int size) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return SetArrayIntValue(object, property_, values, size);
        }

        virtual void SetArrayIntValue(const std::shared_ptr<T>& object, const std::string& property_, int* values, int size)
        {
            throw Reliability::ProbabilisticLibraryException("SetArrayIntValue: unknown property " + property_ + " in " + ProjectEntries::GetObjectTypeString(GetObjectType()));
        }

        // bool

        bool GetBoolValue(int id, const std::string& property_) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return GetBoolValue(object, property_);
        }

        virtual bool GetBoolValue(const std::shared_ptr<T>& object, const std::string& property_)
        {
            throw Reliability::ProbabilisticLibraryException("GetBoolValue: unknown property " + property_ + " in " + ProjectEntries::GetObjectTypeString(GetObjectType()));
        }

        void SetBoolValue(int id, const std::string& property_, bool value) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return SetBoolValue(object, property_, value);
        }

        virtual void SetBoolValue(const std::shared_ptr<T>& object, const std::string& property_, bool value)
        {
            throw Reliability::ProbabilisticLibraryException("SetBoolValue: unknown property " + property_ + " in " + ProjectEntries::GetObjectTypeString(GetObjectType()));
        }

        // std::string

        std::string GetStringValue(int id, const std::string& property_) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return GetStringValue(object, property_);
        }

        virtual std::string GetStringValue(const std::shared_ptr<T>& object, const std::string& property_)
        {
            throw Reliability::ProbabilisticLibraryException("GetStringValue: unknown property " + property_ + " in " + ProjectEntries::GetObjectTypeString(GetObjectType()));
        }

        void SetStringValue(int id, const std::string& property_, const std::string& value) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return SetStringValue(object, property_, value);
        }

        virtual void SetStringValue(const std::shared_ptr<T>& object, const std::string& property_, const std::string& value)
        {
            throw Reliability::ProbabilisticLibraryException("SetStringValue: unknown property " + property_ + " in " + ProjectEntries::GetObjectTypeString(GetObjectType()));
        }

        // method

        void Execute(int id, const std::string& method_) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return Execute(object, method_);
        }

        virtual void Execute(const std::shared_ptr<T>& object, const std::string& method_)
        {
            throw Reliability::ProbabilisticLibraryException("Execute: unknown method " + method_ + " in " + ProjectEntries::GetObjectTypeString(GetObjectType()));
        }

        // callbacks

        void SetCallBack(int id, const std::string& property_, Models::ZValuesCallBack callBack) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return SetCallBack(object, property_, callBack);
        }

        virtual void SetCallBack(const std::shared_ptr<T>& object, const std::string& property_, Models::ZValuesCallBack callBack)
        {
            throw Reliability::ProbabilisticLibraryException("GetCallBack: unknown property " + property_ + " in " + ProjectEntries::GetObjectTypeString(GetObjectType()));
        }

        void SetProgressCallBacks(int id, Models::ProgressCallBack progress, Models::DetailedProgressCallBack detailed, Models::TextualProgressCallBack textual) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return SetProgressCallBacks(object, progress, detailed, textual);
        }

        virtual void SetProgressCallBacks(const std::shared_ptr<T>& object, Models::ProgressCallBack progress, Models::DetailedProgressCallBack detailed, Models::TextualProgressCallBack textual)
        {
            throw Reliability::ProbabilisticLibraryException("SetProgressCallBacks");
        }

        void SetMultipleCallBack(int id, const std::string& property_, Models::ZValuesMultipleCallBack callBack) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return SetMultipleCallBack(object, property_, callBack);
        }

        virtual void SetMultipleCallBack(const std::shared_ptr<T>& object, const std::string& property_, Models::ZValuesMultipleCallBack callBack)
        {
            throw Reliability::ProbabilisticLibraryException("GetMultipleCallBack: unknown property " + property_ + " in " + ProjectEntries::GetObjectTypeString(GetObjectType()));
        }

        void SetEmptyCallBack(int id, const std::string& property_, Models::EmptyCallBack callBack) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return SetEmptyCallBack(object, property_, callBack);
        }

        virtual void SetEmptyCallBack(const std::shared_ptr<T>& object, const std::string& property_, Models::EmptyCallBack callBack)
        {
            throw Reliability::ProbabilisticLibraryException("GetEmptyCallBack: unknown property " + property_ + " in " + ProjectEntries::GetObjectTypeString(GetObjectType()));
        }

        void SetModelSampleCallBack(int id, const std::string& property_, Models::ModelSampleCallback callBack) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return SetModelSampleCallBack(object, property_, callBack);
        }

        virtual void SetModelSampleCallBack(const std::shared_ptr<T>& object, const std::string& property_, Models::ModelSampleCallback callBack)
        {
            throw Reliability::ProbabilisticLibraryException("GetModelSampleCallBack: unknown property " + property_ + " in " + ProjectEntries::GetObjectTypeString(GetObjectType()));
        }

        void SetMultipleModelSampleCallBack(int id, const std::string& property_, Models::MultipleModelSampleCallback callBack) override
        {
            std::shared_ptr<T> object = GetObject(id);
            return SetMultipleModelSampleCallBack(object, property_, callBack);
        }

        virtual void SetMultipleModelSampleCallBack(const std::shared_ptr<T>& object, const std::string& property_, Models::MultipleModelSampleCallback callBack)
        {
            throw Reliability::ProbabilisticLibraryException("GetMultipleModelSampleCallBack: unknown property " + property_ + " in " + ProjectEntries::GetObjectTypeString(GetObjectType()));
        }
    };
}

