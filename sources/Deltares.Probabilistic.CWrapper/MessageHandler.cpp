#include "../Deltares.Probabilistic/Model/ProjectServer.h"

#include <string>

std::shared_ptr<Deltares::Models::ProjectServer> projectServer = std::make_shared< Deltares::Models::ProjectServer>();

extern "C" __declspec(dllexport) int Create(char* type)
{
    std::string typeStr = type;
    int id = projectServer->Create(typeStr);
    return id;
}

extern "C" __declspec(dllexport) double GetValue(int id, char* property)
{
    std::string propertyStr(property);
    return projectServer->GetValue(id, propertyStr);
}

extern "C" __declspec(dllexport) void SetValue(int id, char* property, double value)
{
    std::string propertyStr(property);
    projectServer->SetValue(id, propertyStr, value);
}

extern "C" __declspec(dllexport) int GetIntValue(int id, char* property)
{
    std::string propertyStr(property);
    return projectServer->GetIntValue(id, propertyStr);
}

extern "C" __declspec(dllexport) void SetIntValue(int id, char* property, int value)
{
    std::string propertyStr(property);
    projectServer->SetIntValue(id, propertyStr, value);
}

extern "C" __declspec(dllexport) bool GetBoolValue(int id, char* property)
{
    std::string propertyStr(property);
    return projectServer->GetBoolValue(id, propertyStr);
}

extern "C" __declspec(dllexport) void SetBoolValue(int id, char* property, bool value)
{
    std::string propertyStr(property);
    projectServer->SetBoolValue(id, propertyStr, value);
}

extern "C" __declspec(dllexport) const char* GetStringValue(int id, char* property)
{
    std::string propertyStr(property);
    std::string result = projectServer->GetStringValue(id, propertyStr);
    const char* resultC = result.c_str();
    return resultC;
}

extern "C"  __declspec(dllexport) void SetStringValue(int id, char* property, char* value)
{
    std::string propertyStr(property);
    std::string valueStr(value);
    projectServer->SetStringValue(id, propertyStr, valueStr);
}

extern "C" __declspec(dllexport) void SetArrayValue(int id, char* property, int* values, int size)
{
    std::string propertyStr(property);
    projectServer->SetArrayValue(id, propertyStr, values, size);
}

extern "C"  __declspec(dllexport) double GetArgValue(int id, char* property, double argument)
{
    std::string propertyStr(property);
    return projectServer->GetArgValue(id, propertyStr, argument);
}

extern "C" __declspec(dllexport) void SetArgValue(int id, char* property, double argument, double value)
{
    std::string propertyStr(property);
    projectServer->SetArgValue(id, propertyStr, argument, value);
}

extern "C" __declspec(dllexport) void Execute(int id, char* method)
{
    std::string methodStr(method);
    projectServer->Execute(id, methodStr);
}