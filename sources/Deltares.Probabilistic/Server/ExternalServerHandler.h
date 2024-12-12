// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of the Probabilistic Library.
//
// The Probabilistic Library is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// All names, logos, and references to "Deltares" are registered trademarks of
// Stichting Deltares and remain full property of Stichting Deltares at all times.
// All rights reserved.
//
#pragma once

#include "BaseHandler.h"
#include "ProjectServer.h"
#include "../Utils/probLibException.h"

#define _WINSOCKAPI_

#if __has_include(<windows.h>)
#include <windows.h>
#include <winsock.h>
#include <ws2tcpip.h>
#include <filesystem>
#endif


namespace Deltares
{
    namespace Server
    {
        class ExternalServerHandler : public BaseHandler
        {
        public:
            ExternalServerHandler(std::string serverName)
            {
                this->serverName = serverName;
            }

            ~ExternalServerHandler()
            {
#if __has_include(<windows.h>)
                freeaddrinfo(address);

                if (this->server_started)
                {
                    this->Send("exit", false);
                    this->server_started = false;
                }
#endif
            }

#if __has_include(<windows.h>)
            bool CanHandle(std::string objectType) override;
            void Create(std::string objectType, int id) override;
            void Destroy(int id) override;
            void Exit() override;
            double GetValue(int id, std::string property) override;
            void SetValue(int id, std::string property, double value) override;
            double GetArgValue(int id, std::string property, double argument) override;
            bool GetBoolValue(int id, std::string property) override;
            void SetBoolValue(int id, std::string property, bool value) override;
            int GetIntValue(int id, std::string property) override;
            void SetIntValue(int id, std::string property, int value) override;
            std::string GetStringValue(int id, std::string property) override;
            void SetStringValue(int id, std::string property, std::string value) override;
            double GetIndexedValue(int id, std::string property_, int index) override;
            int GetIndexedIntValue(int id, std::string property_, int index) override;
            std::string GetIndexedStringValue(int id, std::string property, int index) override;
            void SetArrayIntValue(int id, std::string property_, int* values, int size) override;
            void GetArgValues(int id, std::string property, double* values, int size, double* outputValues) override;
            int GetIdValue(int id, std::string property_, int newId) override;
            int GetIndexedIdValue(int id, std::string property_, int index, int newId) override;
            double GetIndexedIndexedValue(int id, std::string property, int index1, int index2) override;
            void SetIndexedIndexedValue(int id, std::string property, int index1, int index2, double value) override;
            void Execute(int id, std::string method_) override;
#endif

        private:
            std::string serverName = "";
            bool server_started = false;

#if __has_include(<windows.h>)
            WSADATA wsaData;

            std::string Send(std::string message, bool waitForAnswer);
            SOCKET ConnectSocket();
            void StartServer();
            bool CheckConnection();

            std::string StringJoin(const std::vector<std::string>& strings, const std::string delim);
            std::vector<std::string> StringSplit(std::string& text, const std::string& delimiter);

            void StartProcess(std::string processName, bool waitForExit);
            void UpdateAddressInfo();

            addrinfo* address = nullptr;
            addrinfo hints;
#endif
        };
    }
}

