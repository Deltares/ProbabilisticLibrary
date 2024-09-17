#include "ExternalHandler.h"

#ifndef __GNUC__
namespace Deltares
{
    namespace Server
    {
        bool ExternalHandler::CanHandle(std::string objectType)
        {
            return this->canHandle(objectType.c_str());
        }

        void ExternalHandler::Create(std::string objectType, int id)
        {
            this->createMethod(objectType.c_str(), id);
        }

        void ExternalHandler::Destroy(int id)
        {
            this->destroyMethod(id);
        }

        double ExternalHandler::GetValue(int id, std::string property)
        {
            return this->getValueMethod(id, property.c_str());
        }

        void ExternalHandler::SetValue(int id, std::string property, double value)
        {
            this->setValueMethod(id, property.c_str(), value);
        }

        bool ExternalHandler::GetBoolValue(int id, std::string property)
        {
            return this->getBoolValueMethod(id, property.c_str());
        }

        void ExternalHandler::SetBoolValue(int id, std::string property, bool value)
        {
            this->setBoolValueMethod(id, property.c_str(), value);
        }

        int ExternalHandler::GetIntValue(int id, std::string property)
        {
            return this->getIntValueMethod(id, property.c_str());
        }

        void ExternalHandler::SetIntValue(int id, std::string property, int value)
        {
            this->setIntValueMethod(id, property.c_str(), value);
        }

        std::string ExternalHandler::GetStringValue(int id, std::string property)
        {
            return this->getStringValueMethod(id, property.c_str());
        }

        void ExternalHandler::SetStringValue(int id, std::string property, std::string value)
        {
            this->setStringValueMethod(id, property.c_str(), value.c_str());
        }

        int ExternalHandler::GetIndexedIntValue(int id, std::string property_, int index)
        {
            return this->getIndexedIntMethod(id, property_.c_str(), index);
        }

        void ExternalHandler::SetArrayIntValue(int id, std::string property_, int* values, int size)
        {
            this->setArrayIntMethod(id, property_.c_str(), values, size);
        }

        Models::ZLambda ExternalHandler::GetCallBack(int id, std::string method)
        {
            return this->getCallbackMethod(id, method.c_str());
        }
    }
}
#endif

