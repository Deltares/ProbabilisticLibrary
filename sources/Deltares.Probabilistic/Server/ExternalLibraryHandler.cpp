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
#include "ExternalLibraryHandler.h"

namespace Deltares
{
    namespace Server
    {
#if __has_include(<windows.h>)
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
#else
void ExternalLibraryHandler::Initialize() {}
bool ExternalLibraryHandler::CanHandle(std::string objectType) { return false; }
void ExternalLibraryHandler::Create(std::string objectType, int id) {}
void ExternalLibraryHandler::Destroy(int id) {}
double ExternalLibraryHandler::GetValue(int id, std::string property) { return std::nan(""); }
void ExternalLibraryHandler::SetValue(int id, std::string property, double value) {}
bool ExternalLibraryHandler::GetBoolValue(int id, std::string property) { return false; }
void ExternalLibraryHandler::SetBoolValue(int id, std::string property, bool value) {}
int ExternalLibraryHandler::GetIntValue(int id, std::string property) { return -1; }
void ExternalLibraryHandler::SetIntValue(int id, std::string property, int value) {}
std::string ExternalLibraryHandler::GetStringValue(int id, std::string property) { return ""; }
void ExternalLibraryHandler::SetStringValue(int id, std::string property, std::string value) {}
int ExternalLibraryHandler::GetIndexedIntValue(int id, std::string property_, int index) { return -1; }
std::string ExternalLibraryHandler::GetIndexedStringValue(int id, std::string property_, int index) { return ""; }
void ExternalLibraryHandler::SetArrayIntValue(int id, std::string property_, int* values, int size) {}
#endif
    }
}

