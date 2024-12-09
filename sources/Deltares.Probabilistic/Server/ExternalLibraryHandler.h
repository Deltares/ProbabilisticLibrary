#pragma once

#if __has_include(<windows.h>)
#include <windows.h>
#endif

#include "BaseHandler.h"
#include "../Utils/probLibException.h"

namespace Deltares
{
    namespace Server
    {
        class ExternalLibraryHandler : public BaseHandler
        {
        public:
            ExternalLibraryHandler(std::string libraryName)
            {
#if __has_include(<windows.h>)
#ifdef UNICODE
                std::wstring stemp = std::wstring(libraryName.begin(), libraryName.end());
                LPCWSTR library = stemp.c_str();
#else
                LPCSTR library = libraryName.c_str();
#endif
                HINSTANCE libInstance = LoadLibraryEx(library, nullptr, LOAD_LIBRARY_SEARCH_APPLICATION_DIR);
                if (!libInstance)
                {
                    throw Reliability::probLibException("Could not load the dynamic library " + std::string(libraryName));
                }

                // resolve all function addresses
                this->initialize = (f_initialize)GetProcAddress(libInstance, "Initialize");
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
                this->getIndexedIntMethod = (f_get_indexed_int_value)GetProcAddress(libInstance, "GetIndexedIntValue");
                this->getIndexedStringMethod = (f_get_indexed_string_value)GetProcAddress(libInstance, "GetIndexedStringValue");
                this->setArrayIntMethod = (f_set_array_int_value)GetProcAddress(libInstance, "SetArrayIntValue");
#endif
            }

            void Initialize();
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
            int GetIndexedIntValue(int id, std::string property_, int index) override;
            std::string GetIndexedStringValue(int id, std::string property_, int index) override;
            void SetArrayIntValue(int id, std::string property_, int* values, int size) override;
        private:
            typedef void(*f_initialize)();
            typedef bool(*f_can_handle)(const char*);
            typedef void(*f_create)(const char*, int);
            typedef void(* f_destroy)(int);
            typedef double(* f_get_value)(int, const char*);
            typedef void(* f_set_value)(int, const char*, double);
            typedef bool(* f_get_bool_value)(int, const char*);
            typedef void(* f_set_bool_value)(int, const char*, bool);
            typedef int(* f_get_int_value)(int, const char*);
            typedef void(* f_set_int_value)(int, const char*, int);
            typedef char*(* f_get_string_value)(int, const char*);
            typedef void(* f_set_string_value)(int, const char*, const char*);
            typedef int(*f_get_indexed_int_value)(int, const char*, int);
            typedef char*(*f_get_indexed_string_value)(int, const char*, int);
            typedef void(* f_set_array_int_value)(int, const char*, int*, int);
            typedef Models::ZLambda(* f_get_callback_method)(int, const char*);

            f_initialize initialize = nullptr;
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
            f_get_indexed_int_value getIndexedIntMethod = nullptr;
            f_get_indexed_string_value getIndexedStringMethod = nullptr;
            f_set_array_int_value setArrayIntMethod = nullptr;
        };

    }
}

