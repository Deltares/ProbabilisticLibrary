#pragma once
#include <string>
#include <vector>

#include "BaseServer.h"
#include "../Model/ZModel.h"

namespace Deltares
{
    namespace Server
    {
        class BaseHandler
        {
        public:
            virtual void SetServer(std::shared_ptr<BaseServer> server, int handlerIndex, std::shared_ptr<BaseHandler> defaultHandler)
            {
                this->server = server;
                this->handlerIndex = handlerIndex;
                this->defaultHandler = defaultHandler;
            }

            virtual bool CanHandle(std::string object_type) { return false; }
            virtual void Create(std::string object_type, int id) { }
            virtual void Destroy(int id) {}
            virtual double GetValue(int id, const std::string property_) { return 0; }
            virtual void SetValue(int id, const std::string property_, double value) {}
            virtual int GetIntValue(int id, std::string property_) { return 0; }
            virtual void SetIntValue(int id, std::string property_, int value) {}
            virtual double GetIntArgValue(int id1, int id2, std::string property_) { return 0; }
            virtual void SetIntArgValue(int id1, int id2, std::string property_, double value) {}
            virtual bool GetBoolValue(int id, std::string property_) { return false; }
            virtual void SetBoolValue(int id, std::string property_, bool value) {}
            virtual std::string GetStringValue(int id, std::string property_) { return ""; }
            virtual std::string GetIndexedStringValue(int id, std::string property_, int index) { return ""; }
            virtual void SetStringValue(int id, std::string property_, std::string value) {}
            virtual void SetArrayValue(int id, std::string property_, double* values, int size) {}
            virtual double GetArgValues(int id, std::string property_, double* values, int size) { return 0; }
            virtual std::vector<int> GetArrayIntValue(int id, std::string property_) { return std::vector<int>(); }
            virtual void SetArrayIntValue(int id, std::string property_, int* values, int size) {}
            virtual double GetArgValue(int id, std::string property_, double argument) { return 0; }
            virtual void SetArgValue(int id, std::string property_, double argument, double value) {}
            virtual double GetIndexedValue(int id, std::string property_, int index) { return 0; }
            virtual void SetIndexedValue(int id, std::string property_, int index, double value) {}
            virtual double GetIndexedIndexedValue(int id, std::string property_, int index1, int index2) { return 0; }
            virtual void SetIndexedIndexedValue(int id, std::string property_, int index1, int index2, double value) {}
            virtual int GetIndexedIntValue(int id, std::string property_, int index) { return 0; }
            virtual void SetCallBack(int id, std::string property_, Models::ZValuesCallBack callBack) {}
            virtual void Execute(int id, std::string method_) {}

        protected:
            std::shared_ptr<BaseServer> server = nullptr;
            int handlerIndex = -1;
            std::shared_ptr<BaseHandler> defaultHandler = nullptr;
        };
    }
}

