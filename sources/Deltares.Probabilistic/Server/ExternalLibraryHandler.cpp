#include "ExternalLibraryHandler.h"

#if __has_include(<windows.h>)
namespace Deltares
{
    namespace Server
    {
        void ExternalLibraryHandler::Initialize()
        {
            this->initialize();
        }

        bool ExternalLibraryHandler::CanHandle(std::string objectType)
        {
            return this->canHandle(objectType.c_str());
        }

        void ExternalLibraryHandler::Create(std::string objectType, int id)
        {
            this->createMethod(objectType.c_str(), id);
        }

        void ExternalLibraryHandler::Destroy(int id)
        {
            this->destroyMethod(id);
        }

        double ExternalLibraryHandler::GetValue(int id, std::string property)
        {
            return this->getValueMethod(id, property.c_str());
        }

        void ExternalLibraryHandler::SetValue(int id, std::string property, double value)
        {
            this->setValueMethod(id, property.c_str(), value);
        }

        bool ExternalLibraryHandler::GetBoolValue(int id, std::string property)
        {
            return this->getBoolValueMethod(id, property.c_str());
        }

        void ExternalLibraryHandler::SetBoolValue(int id, std::string property, bool value)
        {
            this->setBoolValueMethod(id, property.c_str(), value);
        }

        int ExternalLibraryHandler::GetIntValue(int id, std::string property)
        {
            return this->getIntValueMethod(id, property.c_str());
        }

        void ExternalLibraryHandler::SetIntValue(int id, std::string property, int value)
        {
            this->setIntValueMethod(id, property.c_str(), value);
        }

        std::string ExternalLibraryHandler::GetStringValue(int id, std::string property)
        {
            return this->getStringValueMethod(id, property.c_str());
        }

        void ExternalLibraryHandler::SetStringValue(int id, std::string property, std::string value)
        {
            this->setStringValueMethod(id, property.c_str(), value.c_str());
        }

        int ExternalLibraryHandler::GetIndexedIntValue(int id, std::string property_, int index)
        {
            return this->getIndexedIntMethod(id, property_.c_str(), index);
        }

        std::string ExternalLibraryHandler::GetIndexedStringValue(int id, std::string property_, int index)
        {
            return this->getIndexedStringMethod(id, property_.c_str(), index);
        }

        void ExternalLibraryHandler::SetArrayIntValue(int id, std::string property_, int* values, int size)
        {
            this->setArrayIntMethod(id, property_.c_str(), values, size);
        }

        Models::ZLambda ExternalLibraryHandler::GetCallBack(int id, std::string method)
        {
            return this->getCallbackMethod(id, method.c_str());
        }
    }
}
#endif

