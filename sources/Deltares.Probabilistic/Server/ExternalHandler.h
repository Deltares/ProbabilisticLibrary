#pragma once

#include <windows.h>

#include "BaseHandler.h"
#include "../Utils/probLibException.h"

namespace Deltares
{
    namespace Server
    {
        class ExternalHandler : public BaseHandler
        {
        public:
            ExternalHandler(std::string libraryName)
            {
                std::wstring stemp = std::wstring(libraryName.begin(), libraryName.end());
                LPCWSTR library = stemp.c_str();

                HINSTANCE libInstance = LoadLibrary(library);

                if (!libInstance)
                {
                    throw Reliability::probLibException("Could not load the dynamic library " + std::string(libraryName));
                }

                // resolve all function addresses
                this->canHandle = (f_can_handle)GetProcAddress(libInstance, "CanHandle");
                this->createMethod = (f_create)GetProcAddress(libInstance, "Create");
                this->destroyMethod = (f_destroy)GetProcAddress(libInstance, "Destroy");
                this->getValueMethod = (f_get_value)GetProcAddress(libInstance, "GetValue");
                this->setValueMethod = (f_set_value)GetProcAddress(libInstance, "SetValue");
                this->getBoolValueMethod = (f_get_bool_value)GetProcAddress(libInstance, "GetBoolValue");
                this->setBoolValueMethod = (f_set_bool_value)GetProcAddress(libInstance, "SetBoolValue");
                this->getIntValueMethod = (f_get_int_value)GetProcAddress(libInstance, "GetIntValue");
                this->setIntValueMethod = (f_set_int_value)GetProcAddress(libInstance, "SetIntValue");
                this->getStringValueMethod = (f_get_string_value)GetProcAddress(libInstance, "GetStringValue");
                this->setStringValueMethod = (f_set_string_value)GetProcAddress(libInstance, "SetStringValue");
                this->getCallbackMethod = (f_get_callback_method)GetProcAddress(libInstance, "GetCallBack");
            }

            bool CanHandle(std::string objectType) override;
            void Create(std::string objectType, int id) override;
            void Destroy(int id) override;
            double GetValue(int id, std::string property) override;
            void SetValue(int id, std::string property, double value) override;
            bool GetBoolValue(int id, std::string property) override;
            void SetBoolValue(int id, std::string property, bool value) override;
            int GetIntValue(int id, std::string property) override;
            void SetIntValue(int id, std::string property, int value) override;
            std::string GetStringValue(int id, std::string property) override;
            void SetStringValue(int id, std::string property, std::string value) override;
            Models::ZLambda GetCallBack(int id, std::string method) override;
        private:
            typedef bool(__stdcall* f_can_handle)(const char*);
            typedef void(__stdcall* f_create)(const char*, int);
            typedef void(__stdcall* f_destroy)(int);
            typedef double(__stdcall* f_get_value)(int, const char*);
            typedef void(__stdcall* f_set_value)(int, const char*, double);
            typedef bool(__stdcall* f_get_bool_value)(int, const char*);
            typedef void(__stdcall* f_set_bool_value)(int, const char*, bool);
            typedef int(__stdcall* f_get_int_value)(int, const char*);
            typedef void(__stdcall* f_set_int_value)(int, const char*, int);
            typedef char*(__stdcall* f_get_string_value)(int, const char*);
            typedef void(__stdcall* f_set_string_value)(int, const char*, const char*);
            typedef Models::ZLambda(__stdcall* f_get_callback_method)(int, const char*);

            f_can_handle canHandle = nullptr;
            f_create createMethod = nullptr;
            f_destroy destroyMethod = nullptr;
            f_get_value getValueMethod = nullptr;
            f_set_value setValueMethod = nullptr;
            f_get_bool_value getBoolValueMethod = nullptr;
            f_set_bool_value setBoolValueMethod = nullptr;
            f_get_int_value getIntValueMethod = nullptr;
            f_set_int_value setIntValueMethod = nullptr;
            f_get_string_value getStringValueMethod = nullptr;
            f_set_string_value setStringValueMethod = nullptr;
            f_get_callback_method getCallbackMethod = nullptr;
        };
    }
}

