#include "../Deltares.Probabilistic/Model/ProjectServer.h"

#include <string>
#ifdef __GNUC__
#define DLL_PUBLIC __attribute__ ((visibility("default")))
#else
#define DLL_PUBLIC __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
#endif

std::shared_ptr<Deltares::Models::ProjectServer> projectServer = std::make_shared< Deltares::Models::ProjectServer>();

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

extern "C" DLL_PUBLIC const char* GetStringValue(int id, char* property)
{
    std::string propertyStr(property);
    std::string result = projectServer->GetStringValue(id, propertyStr);
    const char* resultC = result.c_str();
    return resultC;
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

// this method does not work
extern "C" DLL_PUBLIC int* GetArrayIntValue(int id, char* property)
{
    std::string propertyStr(property);
    std::vector<int> values = projectServer->GetArrayIntValue(id, propertyStr);
    values.push_back(0);

    int* data = values.data();

    return data;
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

extern "C" DLL_PUBLIC void SetCallBack(int id, char* property, Deltares::Models::ZValuesCallBack callBack)
{
    std::string propertyStr(property);
    projectServer->SetCallBack(id, propertyStr, callBack);
}

extern "C" DLL_PUBLIC void Execute(int id, char* method)
{
    std::string methodStr(method);
    projectServer->Execute(id, methodStr);
}