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

#include "StoredObjectHandler.h"

namespace Deltares::Server
{
    /**
     * \brief Base class for object handlers which contains a registration of all objects
     */
    template<typename T, typename U>
    requires std::derived_from<T, U>
    class DerivedObjectHandler : public StoredObjectHandler<T>
    {
    private:
        StoredObjectHandler<U>* baseHandler = nullptr;

    public:

        void SetBaseHandler(StoredObjectHandler<U>* handler)
        {
            baseHandler = handler;

            GetObjectCallBack<U> objectCallback = [this](const int id) { return this->GetObject(id); };
            baseHandler->AddInheritor(objectCallback);
        }

        std::shared_ptr<U> GetBaseObject(int id)
        {
            return baseHandler->GetObject(id);
        }

        std::vector<std::shared_ptr<U>> GetBaseObjects(int* ids, int size)
        {
            return baseHandler->GetObjects(ids, size);
        }

        int GetBaseObjectId(const std::shared_ptr<U>& object)
        {
            return baseHandler->GetObjectId(object);
        }

        // double

        double GetValue(const std::shared_ptr<T>& object, const std::string& property_) override
        {
            return baseHandler->GetValue(object, property_);
        }

        void SetValue(const std::shared_ptr<T>& object, const std::string& property_, double value) override
        {
            baseHandler->SetValue(object, property_, value);
        }

        // indexed double

        double GetIndexedValue(const std::shared_ptr<T>& object, const std::string& property_, int index) override
        {
            return baseHandler->GetIndexedValue(object, property_, index);
        }

        void SetIndexedValue(const std::shared_ptr<T>& object, const std::string& property_, int index, double value) override
        {
            baseHandler->SetIndexedValue(object, property_, index, value);
        }

        // indexed int

        int GetIndexedIntValue(const std::shared_ptr<T>& object, const std::string& property_, int index) override
        {
            return baseHandler->GetIndexedIntValue(object, property_, index);
        }

        // indexed indexed double

        double GetIndexedIndexedValue(const std::shared_ptr<T>& object, const std::string& property_, int index1, int index2) override
        {
            return baseHandler->GetIndexedIndexedValue(object, property_, index1, index2);
        }

        void SetIndexedIndexedValue(const std::shared_ptr<T>& object, const std::string& property_, int index1, int index2, double value) override
        {
            baseHandler->SetIndexedIndexedValue(object, property_, index1, index2, value);
        }

        // indexed indexed int

        void SetIndexedIndexedIntValue(const std::shared_ptr<T>& object, const std::string& property_, int index1, int index2, int value) override
        {
            baseHandler->SetIndexedIndexedIntValue(object, property_, index1, index2, value);
        }

        // arg

        double GetArgValue(const std::shared_ptr<T>& object, const std::string& property_, double argument) override
        {
            return baseHandler->GetArgValue(object, property_, argument);
        }

        void SetArgValue(const std::shared_ptr<T>& object, const std::string& property_, double argument, double value) override
        {
            baseHandler->SetArgValue(object, property_, argument, value);
        }

        // int arg

        double GetIntArgValue(const std::shared_ptr<T>& object, int argument, const std::string& property_) override
        {
            return baseHandler->GetIntArgValue(object, argument, property_);
        }

        void SetIntArgValue(const std::shared_ptr<T>& object, int argument, const std::string& property_, double value) override
        {
            baseHandler->SetIntArgValue(object, argument, property_, value);
        }

        // int

        int GetIntValue(const std::shared_ptr<T>& object, const std::string& property_) override
        {
            return baseHandler->GetIntValue(object, property_);
        }

        void SetIntValue(const std::shared_ptr<T>& object, const std::string& property_, int value) override
        {
            baseHandler->SetIntValue(object, property_, value);
        }

        // id

        int GetIdValue(const std::shared_ptr<T>& object, const std::string& property_) override
        {
            return baseHandler->GetIdValue(object, property_);
        }

        int GetIndexedIdValue(const std::shared_ptr<T>& object, const std::string& property_, int index) override
        {
            return baseHandler->GetIndexedIdValue(object, property_, index);
        }

        void SetBaseIdValues(std::vector<std::shared_ptr<U>>& list, int* ids, int size)
        {
            baseHandler->SetIdValues(list, ids, size);
        }

        // array

        void SetArrayValue(const std::shared_ptr<T>& object, const std::string& property_, double* values, int size) override
        {
            baseHandler->SetArrayValue(object, property_, values, size);
        }

        void SetArrayIntValue(const std::shared_ptr<T>& object, const std::string& property_, int* values, int size) override
        {
            baseHandler->SetArrayIntValue(object, property_, values, size);
        }

        // bool

        bool GetBoolValue(const std::shared_ptr<T>& object, const std::string& property_) override
        {
            return baseHandler->GetBoolValue(object, property_);
        }
         
        void SetBoolValue(const std::shared_ptr<T>& object, const std::string& property_, bool value) override
        {
            baseHandler->SetBoolValue(object, property_, value);
        }

        // std::string

        std::string GetStringValue(const std::shared_ptr<T>& object, const std::string& property_) override
        {
            return baseHandler->GetStringValue(object, property_);
        }

        void SetStringValue(const std::shared_ptr<T>& object, const std::string& property_, const std::string& value) override
        {
            baseHandler->SetStringValue(object, property_, value);
        }

        // method

        void Execute(const std::shared_ptr<T>& object, const std::string& method_) override
        {
            baseHandler->Execute(object, method_);
        }

        // callbacks

        void SetCallBack(const std::shared_ptr<T>& object, const std::string& property_, Models::ZValuesCallBack callBack) override
        {
            baseHandler->SetCallBack(object, property_, callBack);
        }

        void SetProgressCallBacks(const std::shared_ptr<T>& object, Models::ProgressCallBack progress, Models::DetailedProgressCallBack detailed, Models::TextualProgressCallBack textual) override
        {
            baseHandler->SetProgressCallBacks(object, progress, detailed, textual);
        }

        void SetMultipleCallBack(const std::shared_ptr<T>& object, const std::string& property_, Models::ZValuesMultipleCallBack callBack) override
        {
            baseHandler->SetMultipleCallBack(object, property_, callBack);
        }

        void SetEmptyCallBack(const std::shared_ptr<T>& object, const std::string& property_, Models::EmptyCallBack callBack) override
        {
            baseHandler->SetEmptyCallBack(object, property_, callBack);
        }

        void SetModelSampleCallBack(const std::shared_ptr<T>& object, const std::string& property_, Models::ModelSampleCallback callBack) override
        {
            baseHandler->SetModelSampleCallBack(object, property_, callBack);
        }

        void SetMultipleModelSampleCallBack(const std::shared_ptr<T>& object, const std::string& property_, Models::MultipleModelSampleCallback callBack)
        {
            baseHandler->SetMultipleModelSampleCallBack(object, property_, callBack);
        }
    };
}

