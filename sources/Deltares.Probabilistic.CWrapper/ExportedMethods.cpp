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

#include <string>
#include <memory>

#include "../Deltares.Probabilistic/Server/ProjectServer.h"
#include "../Deltares.Probabilistic/Server/ExternalServerHandler.h"
#include "../Deltares.Probabilistic/Server/ExternalLibraryHandler.h"

#ifdef __GNUC__
#define DLL_PUBLIC __attribute__ ((visibility("default")))
#else
#define DLL_PUBLIC __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
#endif

Deltares::Server::ProjectServer* projectServer = new Deltares::Server::ProjectServer();

extern "C" DLL_PUBLIC void AddLibrary(char* library)
{
    std::string libraryStr(library);

    if (libraryStr.ends_with(".exe"))
    {
        std::shared_ptr<Deltares::Server::ExternalServerHandler> externalHandler = std::make_shared<Deltares::Server::ExternalServerHandler>(libraryStr);
        externalHandler->Initialize();
        projectServer->AddHandler(externalHandler);
    }
    else if (libraryStr.ends_with(".dll"))
    {
        std::shared_ptr<Deltares::Server::ExternalLibraryHandler> externalHandler = std::make_shared<Deltares::Server::ExternalLibraryHandler>(libraryStr);
        externalHandler->Initialize();
        projectServer->AddHandler(externalHandler);
    }
}

extern "C" DLL_PUBLIC int Create(char* type)
{
    std::string typeStr = type;
    int id = projectServer->Create(typeStr);
    return id;
}

extern "C" DLL_PUBLIC void Destroy(int id)
{
    projectServer->Destroy(id);
}

extern "C" DLL_PUBLIC double GetValue(int id, char* property)
{
    std::string propertyStr(property);
    return projectServer->GetValue(id, propertyStr);
}

extern "C" DLL_PUBLIC void SetValue(int id, char* property, double value)
{
    std::string propertyStr(property);
    projectServer->SetValue(id, propertyStr, value);
}

extern "C" DLL_PUBLIC int GetIntValue(int id, char* property)
{
    std::string propertyStr(property);
    return projectServer->GetIntValue(id, propertyStr);
}

extern "C" DLL_PUBLIC void SetIntValue(int id, char* property, int value)
{
    std::string propertyStr(property);
    projectServer->SetIntValue(id, propertyStr, value);
}

extern "C" DLL_PUBLIC double GetIntArgValue(int id1, int id2, char* property)
{
    std::string propertyStr(property);
    return projectServer->GetIntArgValue(id1, id2, propertyStr);
}

extern "C" DLL_PUBLIC void SetIntArgValue(int id1, int id2, char* property, double value)
{
    std::string propertyStr(property);
    projectServer->SetIntArgValue(id1, id2, propertyStr, value);
}

extern "C" DLL_PUBLIC bool GetBoolValue(int id, char* property)
{
    std::string propertyStr(property);
    return projectServer->GetBoolValue(id, propertyStr);
}

extern "C" DLL_PUBLIC void SetBoolValue(int id, char* property, bool value)
{
    std::string propertyStr(property);
    projectServer->SetBoolValue(id, propertyStr, value);
}

extern "C" DLL_PUBLIC size_t GetStringLength(int id, char* property)
{
    std::string propertyStr(property);
    std::string result = projectServer->GetStringValue(id, propertyStr);
    return result.length();
}

extern "C" DLL_PUBLIC void GetStringValue(int id, char* property, char* result_c, size_t size)
{
    std::string propertyStr(property);
    std::string result = projectServer->GetStringValue(id, propertyStr);
    const char* result_b = result.c_str();

#ifdef __GNUC__
    sprintf(result_c, "%s", result_b);
#else
    _snprintf_s(result_c, size, _TRUNCATE, result_b);
#endif
}

extern "C" DLL_PUBLIC void SetStringValue(int id, char* property, char* value)
{
    std::string propertyStr(property);
    std::string valueStr(value);
    projectServer->SetStringValue(id, propertyStr, valueStr);
}

extern "C" DLL_PUBLIC void SetArrayValue(int id, char* property, double* values, int size)
{
    std::string propertyStr(property);
    projectServer->SetArrayValue(id, propertyStr, values, size);
}

extern "C" DLL_PUBLIC void SetArrayIntValue(int id, char* property, int* values, int size)
{
    std::string propertyStr(property);
    projectServer->SetArrayIntValue(id, propertyStr, values, size);
}

extern "C"  DLL_PUBLIC double GetArgValue(int id, char* property, double argument)
{
    std::string propertyStr(property);
    return projectServer->GetArgValue(id, propertyStr, argument);
}

extern "C" DLL_PUBLIC void SetArgValue(int id, char* property, double argument, double value)
{
    std::string propertyStr(property);
    projectServer->SetArgValue(id, propertyStr, argument, value);
}

extern "C"  DLL_PUBLIC double GetArgValues(int id, char* property, double* values, int size)
{
    std::string propertyStr(property);
    return projectServer->GetArgValues(id, propertyStr, values, size);
}

extern "C"  DLL_PUBLIC double GetIndexedValue(int id, char* property, int index)
{
    std::string propertyStr(property);
    return projectServer->GetIndexedValue(id, propertyStr, index);
}

extern "C" DLL_PUBLIC void SetIndexedValue(int id, char* property, int index, double value)
{
    std::string propertyStr(property);
    projectServer->SetIndexedValue(id, propertyStr, index, value);
}

extern "C" DLL_PUBLIC double GetIndexedIndexedValue(int id, char* property, int index1, int index2)
{
    std::string propertyStr(property);
    return projectServer->GetIndexedIndexedValue(id, propertyStr, index1, index2);
}

extern "C" DLL_PUBLIC void SetIndexedIndexedValue(int id, char* property, int index1, int index2, double value)
{
    std::string propertyStr(property);
    projectServer->SetIndexedIndexedValue(id, propertyStr, index1, index2, value);
}

extern "C" DLL_PUBLIC int GetIndexedIntValue(int id, char* property, int index)
{
    std::string propertyStr(property);
    return projectServer->GetIndexedIntValue(id, propertyStr, index);
}

extern "C" DLL_PUBLIC size_t GetIndexedStringLength(int id, char* property, int index)
{
    std::string propertyStr(property);
    std::string result = projectServer->GetIndexedStringValue(id, propertyStr, index);
    return result.length();
}

extern "C" DLL_PUBLIC void GetIndexedStringValue(int id, char* property, int index, char* result_c, size_t size)
{
    std::string propertyStr(property);
    std::string result = projectServer->GetIndexedStringValue(id, propertyStr, index);

    const char* result_b = result.c_str();

#ifdef __GNUC__
    sprintf(result_c, "%s", result_b);
#else
    _snprintf_s(result_c, size, _TRUNCATE, result_b);
#endif
}

extern "C" DLL_PUBLIC void SetCallBack(int id, char* property, Deltares::Models::ZValuesCallBack callBack)
{
    std::string propertyStr(property);
    projectServer->SetCallBack(id, propertyStr, callBack);
}

extern "C" DLL_PUBLIC void SetInitializeCallBack(int id, char* property, Deltares::Models::ZEmptyCallBack callBack)
{
    std::string propertyStr(property);
    projectServer->SetEmptyCallBack(id, propertyStr, callBack);
}

extern "C" DLL_PUBLIC void Execute(int id, char* method)
{
    std::string methodStr(method);
    projectServer->Execute(id, methodStr);
}
