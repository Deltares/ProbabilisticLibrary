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

#include <string>
#include <memory>

#include "../Deltares.Probabilistic/Server/ProjectServer.h"
#include "../Deltares.Probabilistic/Server/ExternalServerHandler.h"
#include "../Deltares.Probabilistic/Server/ExternalLibraryHandler.h"
#include "stringHelper.h"

#ifdef __GNUC__
#define DLL_PUBLIC __attribute__ ((visibility("default")))
#else
#define DLL_PUBLIC __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
#endif

Deltares::Server::ProjectServer projectServer = Deltares::Server::ProjectServer();
std::string last_exception = "";

extern "C" DLL_PUBLIC void AddLibrary(const char* library)
{
    std::string libraryStr(library);

    if (libraryStr.ends_with(".exe"))
    {
        std::shared_ptr<Deltares::Server::ExternalServerHandler> externalHandler = std::make_shared<Deltares::Server::ExternalServerHandler>(libraryStr);
        projectServer.AddHandler(externalHandler);
    }
    else if (libraryStr.ends_with(".dll"))
    {
        std::shared_ptr<Deltares::Server::ExternalLibraryHandler> externalHandler = std::make_shared<Deltares::Server::ExternalLibraryHandler>(libraryStr);
        externalHandler->Initialize();
        projectServer.AddHandler(externalHandler);
    }
}

extern "C" DLL_PUBLIC int Create(const char* type)
{
    std::string typeStr = type;
    int id = projectServer.Create(typeStr);
    return id;
}

extern "C" DLL_PUBLIC void Destroy(int id)
{
    projectServer.Destroy(id);
}

extern "C" DLL_PUBLIC void Exit()
{
    projectServer.Exit();
}

extern "C" DLL_PUBLIC double GetValue(int id, const char* property)
{
    std::string propertyStr(property);
    return projectServer.GetValue(id, propertyStr);
}

extern "C" DLL_PUBLIC void SetValue(int id, const char* property, double value)
{
    std::string propertyStr(property);
    projectServer.SetValue(id, propertyStr, value);
}

extern "C" DLL_PUBLIC int GetIntValue(int id, const char* property)
{
    std::string propertyStr(property);
    return projectServer.GetIntValue(id, propertyStr);
}

extern "C" DLL_PUBLIC void SetIntValue(int id, const char* property, int value)
{
    std::string propertyStr(property);
    projectServer.SetIntValue(id, propertyStr, value);
}

extern "C" DLL_PUBLIC int GetIdValue(int id, const char* property)
{
    std::string propertyStr(property);
    return projectServer.GetIdValue(id, propertyStr);
}

extern "C" DLL_PUBLIC double GetIntArgValue(int id1, int id2, const char* property)
{
    std::string propertyStr(property);
    return projectServer.GetIntArgValue(id1, id2, propertyStr);
}

extern "C" DLL_PUBLIC void SetIntArgValue(int id1, int id2, const char* property, double value)
{
    std::string propertyStr(property);
    projectServer.SetIntArgValue(id1, id2, propertyStr, value);
}

extern "C" DLL_PUBLIC bool GetBoolValue(int id, const char* property)
{
    std::string propertyStr(property);
    return projectServer.GetBoolValue(id, propertyStr);
}

extern "C" DLL_PUBLIC void SetBoolValue(int id, const char* property, bool value)
{
    std::string propertyStr(property);
    projectServer.SetBoolValue(id, propertyStr, value);
}

extern "C" DLL_PUBLIC size_t GetStringLength(int id, const char* property)
{
    std::string propertyStr(property);
    std::string result = projectServer.GetStringValue(id, propertyStr);
    return result.length();
}

extern "C" DLL_PUBLIC void GetStringValue(int id, const char* property, char* result_c, size_t size)
{
    std::string propertyStr(property);
    std::string result = projectServer.GetStringValue(id, propertyStr);

    copyStringToCharPointer(result, result_c, size);
}

extern "C" DLL_PUBLIC void SetStringValue(int id, const char* property, const char* value)
{
    std::string propertyStr(property);
    std::string valueStr(value);
    projectServer.SetStringValue(id, propertyStr, valueStr);
}

extern "C" DLL_PUBLIC void FillArrayValue(int id, const char* property, double* values, int size)
{
    std::string propertyStr(property);
    projectServer.GetArrayValue(id, propertyStr, values, size);
}

extern "C" DLL_PUBLIC void SetArrayValue(int id, const char* property, double* values, int size)
{
    std::string propertyStr(property);
    projectServer.SetArrayValue(id, propertyStr, values, size);
}

extern "C" DLL_PUBLIC void SetArrayIntValue(int id, const char* property, int* values, int size)
{
    std::string propertyStr(property);
    projectServer.SetArrayIntValue(id, propertyStr, values, size);
}

extern "C"  DLL_PUBLIC double GetArgValue(int id, const char* property, double argument)
{
    std::string propertyStr(property);
    return projectServer.GetArgValue(id, propertyStr, argument);
}

extern "C" DLL_PUBLIC void SetArgValue(int id, const char* property, double argument, double value)
{
    std::string propertyStr(property);
    projectServer.SetArgValue(id, propertyStr, argument, value);
}

extern "C"  DLL_PUBLIC void GetArgValues(int id, const char* property, double* values, int size, double* outputValues)
{
    std::string propertyStr(property);
    projectServer.GetArgValues(id, propertyStr, values, size, outputValues);
}

extern "C"  DLL_PUBLIC double GetIndexedValue(int id, const char* property, int index)
{
    std::string propertyStr(property);
    return projectServer.GetIndexedValue(id, propertyStr, index);
}

extern "C" DLL_PUBLIC void SetIndexedValue(int id, const char* property, int index, double value)
{
    std::string propertyStr(property);
    projectServer.SetIndexedValue(id, propertyStr, index, value);
}

extern "C" DLL_PUBLIC double GetIndexedIndexedValue(int id, const char* property, int index1, int index2)
{
    std::string propertyStr(property);
    return projectServer.GetIndexedIndexedValue(id, propertyStr, index1, index2);
}

extern "C" DLL_PUBLIC void SetIndexedIndexedValue(int id, const char* property, int index1, int index2, double value)
{
    std::string propertyStr(property);
    projectServer.SetIndexedIndexedValue(id, propertyStr, index1, index2, value);
}

extern "C" DLL_PUBLIC void SetIndexedIndexedIntValue(int id, const char* property, int index1, int index2, int value)
{
    std::string propertyStr(property);
    projectServer.SetIndexedIndexedIntValue(id, propertyStr, index1, index2, value);
}

extern "C" DLL_PUBLIC int GetIndexedIntValue(int id, const char* property, int index)
{
    std::string propertyStr(property);
    return projectServer.GetIndexedIntValue(id, propertyStr, index);
}

extern "C" DLL_PUBLIC int GetIndexedIdValue(int id, const char* property, int index)
{
    std::string propertyStr(property);
    return projectServer.GetIndexedIdValue(id, propertyStr, index);
}

extern "C" DLL_PUBLIC size_t GetIndexedStringLength(int id, const char* property, int index)
{
    std::string propertyStr(property);
    std::string result = projectServer.GetIndexedStringValue(id, propertyStr, index);
    return result.length();
}

extern "C" DLL_PUBLIC void GetIndexedStringValue(int id, const char* property, int index, char* result_c, size_t size)
{
    std::string propertyStr(property);
    std::string result = projectServer.GetIndexedStringValue(id, propertyStr, index);

    copyStringToCharPointer(result, result_c, size);
}

extern "C" DLL_PUBLIC void SetCallBack(int id, const char* property, Deltares::Models::ZValuesCallBack callBack)
{
    std::string propertyStr(property);
    projectServer.SetCallBack(id, propertyStr, callBack);
}

extern "C" DLL_PUBLIC void SetMultipleCallBack(int id, const char* property, Deltares::Models::ZValuesMultipleCallBack callBack)
{
    std::string propertyStr(property);
    projectServer.SetMultipleCallBack(id, propertyStr, callBack);
}

extern "C" DLL_PUBLIC void SetEmptyCallBack(int id, const char* property, Deltares::Models::EmptyCallBack callBack)
{
    std::string propertyStr(property);
    projectServer.SetEmptyCallBack(id, propertyStr, callBack);
}

extern "C" DLL_PUBLIC void Execute(int id, const char* method)
{
    try
    {
        last_exception = "";
        std::string methodStr(method);
        projectServer.Execute(id, methodStr);
    }
    catch (const std::exception& e)
    {
        last_exception = std::string(e.what());
    }
}

extern "C" DLL_PUBLIC size_t GetExceptionLength()
{
    return last_exception.length();
}

extern "C" DLL_PUBLIC void GetException(char* result_c, size_t size)
{
    copyStringToCharPointer(last_exception, result_c, size);
}


