#include "../Deltares.Probabilistic/Model/ProjectServer.h"

#include <string>

extern "C" __declspec(dllexport) const void* Create(char* type)
{
    std::string typeStr = type;
    return Deltares::Models::ProjectServer::Create(typeStr);
}

extern "C" __declspec(dllexport) double GetValue(void* id, char* property)
{
    std::string propertyStr(property);
    return Deltares::Models::ProjectServer::GetValue(id, propertyStr);
}

extern "C" __declspec(dllexport) void SetValue(void* id, char* property, double value)
{
    std::string propertyStr(property);
    Deltares::Models::ProjectServer::SetValue(id, propertyStr, value);
}

extern "C" __declspec(dllexport) const char* GetStringValue(void* id, char* property)
{
    std::string propertyStr(property);
    return Deltares::Models::ProjectServer::GetStringValue(id, propertyStr).c_str();
}

extern "C"  __declspec(dllexport) void SetStringValue(void* id, char* property, char* value)
{
    std::string propertyStr(property);
    std::string valueStr(value);
    Deltares::Models::ProjectServer::SetStringValue(id, propertyStr, valueStr);
}

extern "C"  __declspec(dllexport) double GetArgValue(void* id, char* property, double argument)
{
    std::string propertyStr(property);
    return Deltares::Models::ProjectServer::GetArgValue(id, propertyStr, argument);
}

extern "C" __declspec(dllexport) void SetArgValue(void* id, char* property, double argument, double value)
{
    std::string propertyStr(property);
    Deltares::Models::ProjectServer::SetArgValue(id, propertyStr, argument, value);
}






