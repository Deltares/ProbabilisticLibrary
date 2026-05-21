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
#include <vector>

#include "../Model/ZModel.h"

namespace Deltares::Server
{
    class BaseHandler
    {
    public:
        virtual bool CanHandle(const std::string& object_type) { return false; }
        virtual int GetNewId() { return -1; }
        virtual int Create(const std::string& object_type) { return -1; }
        virtual void Destroy(int id) {}
        virtual void Exit() {}
        virtual bool ShouldClose() { return false; }
        virtual double GetValue(int id, const std::string& property_) { return 0; }
        virtual void SetValue(int id, const std::string& property_, double value) {}
        virtual int GetIntValue(int id, const std::string& property_) { return 0; }
        virtual void SetIntValue(int id, const std::string& property_, int value) {}
        virtual double GetIntArgValue(int id1, int id2, const std::string& property_) { return 0; }
        virtual void SetIntArgValue(int id1, int id2, const std::string& property_, double value) {}
        virtual bool GetBoolValue(int id, const std::string& property_) { return false; }
        virtual void SetBoolValue(int id, const std::string& property_, bool value) {}
        virtual std::string GetStringValue(int id, const std::string& property_) { return ""; }
        virtual std::string GetIndexedStringValue(int id, const std::string& property_, int index) { return ""; }
        virtual void SetStringValue(int id, const std::string& property_, const std::string& value) {}
        virtual void SetArrayValue(int id, const std::string& property_, double* values, int size) {}
        virtual void GetArrayValue(int id, const std::string& property_, double* values, int size) {}
        virtual void GetArgValues(int id, const std::string& property_, double* values, int size, double* outputValues) {}
        virtual std::vector<int> GetArrayIntValue(int id, const std::string& property_) { return std::vector<int>(); }
        virtual void SetArrayIntValue(int id, const std::string& property_, int* values, int size) {}
        virtual double GetArgValue(int id, const std::string& property_, double argument) { return 0; }
        virtual void SetArgValue(int id, const std::string& property_, double argument, double value) {}
        virtual double GetIndexedValue(int id, const std::string& property_, int index) { return 0; }
        virtual void SetIndexedValue(int id, const std::string& property_, int index, double value) {}
        virtual double GetIndexedIndexedValue(int id, const std::string& property_, int index1, int index2) { return 0; }
        virtual void SetIndexedIndexedValue(int id, const std::string& property_, int index1, int index2, double value) {}
        virtual void SetIndexedIndexedIntValue(int id, const std::string& property_, int index1, int index2, int value) {}
        virtual int GetIndexedIntValue(int id, const std::string& property_, int index) { return 0; }
        virtual int GetIdValue(int id, const std::string& property_) { return GetIntValue(id, property_); }
        virtual int GetIndexedIdValue(int id, const std::string& property_, int index) { return GetIndexedIntValue(id, property_, index); }
        virtual void SetCallBack(int id, const std::string& property_, Models::ZValuesCallBack callBack) {}
        virtual void SetMultipleCallBack(int id, const std::string& property_, Models::ZValuesMultipleCallBack callBack) {}
        virtual void SetEmptyCallBack(int id, const std::string& property_, Models::EmptyCallBack callBack) {}
        virtual void SetProgressCallBacks(int id, Models::ProgressCallBack progress, Models::DetailedProgressCallBack detailed, Models::TextualProgressCallBack textual) {}
        virtual void SetModelSampleCallBack(int id, const std::string& property_, Models::ModelSampleCallback callBack) {}
        virtual void SetMultipleModelSampleCallBack(int id, const std::string& property_, Models::MultipleModelSampleCallback callBack) {}
        virtual void Execute(int id, const std::string& method_) {}
    };
}

