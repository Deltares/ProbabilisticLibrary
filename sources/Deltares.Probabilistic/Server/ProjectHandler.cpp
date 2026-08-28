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
    using namespace Deltares::Statistics;
    using namespace Deltares::Reliability;

    ProjectHandler::ProjectHandler()
    {
    }

    bool ProjectHandler::CanHandle(const std::string& object_type)
    {
        return ProjectEntries::CanHandle(object_type);
    }

    int ProjectHandler::GetNewId()
    {
        return admin.GetNewId();
    }

    int ProjectHandler::Create(const std::string& object_type)
    {
        ObjectType objectType = ProjectEntries::GetType(object_type);

        return admin.Create(objectType);
    }

    void ProjectHandler::Destroy(int id)
    {
        admin.Destroy(id);
    }

    bool ProjectHandler::ShouldClose()
    {
        return admin.IsEmpty();
    }

    double ProjectHandler::GetValue(int id, const std::string& property_)
    {
        return admin.GetValue(id, property_);
    }

    void ProjectHandler::SetValue(int id, const std::string& property_, double value)
    {
        admin.SetValue(id, property_, value);
    }

    int ProjectHandler::GetIntValue(int id, const std::string& property_)
    {
        return admin.GetIntValue(id, property_);
    }

    int ProjectHandler::GetIdValue(int id, const std::string& property_)
    {
        return admin.GetIdValue(id, property_);
    }

    void ProjectHandler::SetIntValue(int id, const std::string& property_, int value)
    {
        return admin.SetIntValue(id, property_, value);
    }

    double ProjectHandler::GetIntArgValue(int id1, int id2, const std::string& property_)
    {
        return admin.GetIntArgValue(id1, id2, property_);
    }

    void ProjectHandler::SetIntArgValue(int id1, int id2, const std::string& property_, double value)
    {
        return admin.SetIntArgValue(id1, id2, property_, value);
    }

    bool ProjectHandler::GetBoolValue(int id, const std::string& property_)
    {
        return admin.GetBoolValue(id, property_);
    }

    void ProjectHandler::SetBoolValue(int id, const std::string& property_, bool value)
    {
        return admin.SetBoolValue(id, property_, value);
    }

    std::string ProjectHandler::GetStringValue(int id, const std::string& property_)
    {
        return admin.GetStringValue(id, property_);
    }

    void ProjectHandler::SetStringValue(int id, const std::string& property_, const std::string& value)
    {
        return admin.SetStringValue(id, property_, value);
    }

    void ProjectHandler::SetArrayValue(int id, const std::string& property_, double* values, int size)
    {
        admin.SetArrayValue(id, property_, values, size);
    }

    std::vector<int> ProjectHandler::GetArrayIntValue(int id, const std::string& property_)
    {
        return std::vector<int>(0);
    }

    void ProjectHandler::SetArrayIntValue(int id, const std::string& property_, int* values, int size)
    {
        admin.SetArrayIntValue(id, property_, values, size);
    }

    double ProjectHandler::GetArgValue(int id, const std::string& property_, double argument)
    {
        return admin.GetArgValue(id, property_, argument);
    }

    void ProjectHandler::SetArgValue(int id, const std::string& property_, double argument, double value)
    {
        admin.SetArgValue(id, property_, argument, value);
    }

    double ProjectHandler::GetIndexedValue(int id, const std::string& property_, int index)
    {
        return admin.GetIndexedValue(id, property_, index);
    }

    void ProjectHandler::SetIndexedValue(int id, const std::string& property_, int index, double value)
    {
        // not needed yet
    }

    double ProjectHandler::GetIndexedIndexedValue(int id, const std::string& property_, int index1, int index2)
    {
        return admin.GetIndexedIndexedValue(id, property_, index1, index2);
    }

    void ProjectHandler::SetIndexedIndexedValue(int id, const std::string& property_, int index1, int index2, double value)
    {
        admin.SetIndexedIndexedValue(id, property_, index1, index2, value);
    }

    void ProjectHandler::SetIndexedIndexedIntValue(int id, const std::string& property_, int index1, int index2, int value)
    {
        return admin.SetIndexedIndexedIntValue(id, property_, index1, index2, value);
    }

    int ProjectHandler::GetIndexedIntValue(int id, const std::string& property_, int index)
    {
        return 0;
    }

    int ProjectHandler::GetIndexedIdValue(int id, const std::string& property_, int index)
    {
        return admin.GetIndexedIdValue(id, property_, index);
    }

    void ProjectHandler::SetCallBack(int id, const std::string& property_, Models::ZValuesCallBack callBack)
    {
        admin.SetCallBack(id, property_, callBack);
    }

    void ProjectHandler::SetMultipleCallBack(int id, const std::string& property_, Models::ZValuesMultipleCallBack callBack)
    {
        admin.SetMultipleCallBack(id, property_, callBack);
    }

    void ProjectHandler::SetEmptyCallBack(int id, const std::string& property_, Models::EmptyCallBack callBack)
    {
        admin.SetEmptyCallBack(id, property_, callBack);
    }

    void ProjectHandler::SetProgressCallBacks(int id, Models::ProgressCallBack progress, Models::DetailedProgressCallBack detailed, Models::TextualProgressCallBack textual)
    {
        admin.SetProgressCallBacks(id, progress, detailed, textual);
    }

    void ProjectHandler::SetModelSampleCallBack(int id, const std::string& property_, Models::ModelSampleCallback callBack)
    {
        admin.SetModelSampleCallBack(id, property_, callBack);
    }

    void ProjectHandler::SetMultipleModelSampleCallBack(int id, const std::string& property_, Models::MultipleModelSampleCallback callBack)
    {
        admin.SetMultipleModelSampleCallBack(id, property_, callBack);
    }

    void ProjectHandler::Execute(int id, const std::string& method_)
    {
        admin.Execute(id, method_);
    }

    int ProjectHandler::GetStatus(const std::string& command) const
    {
        if (command == "count_entries")
        {
            return admin.GetSize();
        }
        return -1;
    }

    std::shared_ptr<DesignPointIds> ProjectHandler::GetDesignPointIds(int id)
    {
        return nullptr;
    }


}

