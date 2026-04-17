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
#include "ExternalServerHandler.h"

#include <string>
#include <vector>
#include <cstring>
#include <chrono>
#include <thread>

#define _WINSOCKAPI_

#if __has_include(<windows.h>)

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#endif

#include <iostream>
#include <stdexcept>

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "11178"
#define DEFAULT_SERVER "localhost"

namespace Deltares::Server
{
#if __has_include(<windows.h>)
    // implementation based on https ://learn.microsoft.com/en-us/windows/win32/winsock/complete-client-code

    void ExternalServerHandler::UpdateAddressInfo()
    {
        // Initialize Winsock
        int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (iResult != 0)
        {
            printf("WSAStartup failed with error: %d\n", iResult);
            return;
        }

        ZeroMemory(&hints, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;

        auto hostname = std::vector<char>(256);
        gethostname(hostname.data(), static_cast<int>(hostname.size()));

        // Resolve the server address and port
        iResult = getaddrinfo(hostname.data(), DEFAULT_PORT, &hints, &address);
        if (iResult != 0)
        {
            printf("getaddrinfo failed with error: %d\n", iResult);
            WSACleanup();
        }
    }

    SOCKET ExternalServerHandler::ConnectSocket()
    {
        SOCKET ConnectSocket = INVALID_SOCKET;

        // Attempt to connect to an address until one succeeds
        for (addrinfo* ptr = address; ptr != nullptr; ptr = ptr->ai_next) {

            // Create a SOCKET for connecting to server

            ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

            if (ConnectSocket == INVALID_SOCKET)
            {
                printf("socket failed with error: %ld\n", WSAGetLastError());
                WSACleanup();
                return 1;
            }

            // Connect to server.

            int iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);

            if (iResult == SOCKET_ERROR)
            {
                printf("socket connection failed with error: %ld\n", WSAGetLastError());

                closesocket(ConnectSocket);
                ConnectSocket = INVALID_SOCKET;
                continue;
            }
            break;
        }

        if (ConnectSocket == INVALID_SOCKET) {
            printf("Unable to connect to server!\n");
            WSACleanup();
            return 1;
        }

        return ConnectSocket;
    }

    std::string ExternalServerHandler::Send(std::string message)
    {
        SOCKET server_socket = this->ConnectSocket();

        const char* sendbuf = message.c_str();

        // Send an initial buffer
        int iResult = send(server_socket, sendbuf, static_cast<int>(message.size()) + 1, 0);

        if (iResult == SOCKET_ERROR)
        {
            printf("send failed with error: %d\n", WSAGetLastError());
            closesocket(server_socket);
            WSACleanup();
            return "";
        }
        else
        {
            char receiveBuffer[DEFAULT_BUFLEN];

            int received = recv(server_socket, receiveBuffer, sizeof(receiveBuffer), 0);
            if (received < 0)
            {
                throw std::runtime_error("Receive failed");
            }

            std::string answer = std::string(receiveBuffer, received);

            while (received == DEFAULT_BUFLEN)
            {
                received = recv(server_socket, receiveBuffer, sizeof(receiveBuffer), 0);
                if (received >= 0)
                {
                    answer += std::string(receiveBuffer, received);
                }
            }

            closesocket(server_socket);

            return answer;
        }
    }

    void ExternalServerHandler::StartServer()
    {
        if (!std::filesystem::exists(this->serverName))
        {
            throw Reliability::probLibException(this->serverName + " does not exist");
        }

        UpdateAddressInfo();

        // Uncomment the next line to start the server manually, useful for debug purposes

        //server_started = CheckConnection();

        int server_count = 0;

        while (!server_started && server_count < 10)
        {
            StartProcess(this->serverName, false);

            UpdateAddressInfo();

            int connection_count = 0;
            while (!server_started && connection_count < 10)
            {
                std::this_thread::sleep_for(std::chrono::seconds(1));

                server_started = CheckConnection();
                connection_count++;
            }

            server_count++;
        }

        SetParentProcess();
    }

    void ExternalServerHandler::StartProcess(std::string processName, bool waitForExit)
    {
        STARTUPINFO si;
        PROCESS_INFORMATION pi;

#ifdef UNICODE
        std::wstring processNameW(processName.begin(), processName.end());
        LPWSTR cmdLine = processNameW.data();

        // Extract working directory
        size_t pos = processNameW.find_last_of(L"\\/");
        std::wstring workDir = (pos != std::wstring::npos)
            ? processNameW.substr(0, pos)
            : L".";
#else
        LPSTR cmdLine = const_cast<char*>(processName.c_str());

        size_t pos = processName.find_last_of("\\/");
        std::string workDirA = (pos != std::string::npos)
            ? processName.substr(0, pos)
            : ".";
        LPCSTR workDir = workDirA.c_str();
#endif

        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));

        // Start the child process. 
        bool ok = CreateProcess(
            nullptr,            // No module name (use command line)
            cmdLine,            // Command line
            nullptr,            // Process handle not inheritable
            nullptr,            // Thread handle not inheritable
            false,              // Set handle inheritance to FALSE
            CREATE_NO_WINDOW,   // use CREATE_NEW_CONSOLE to debug
            nullptr,            // Use parent's environment block
            workDir.c_str(),    // Use parent's starting directory 
            &si,                // Pointer to STARTUPINFO structure
            &pi);               // Pointer to PROCESS_INFORMATION structure

        if (!ok)
        {
            printf("CreateProcess failed (%d).\n", GetLastError());
            return;
        }

        server_started = true;

        if (waitForExit)
        {
            WaitForSingleObject(pi.hProcess, INFINITE);
        }

        // IMPORTANT: close our handles so Python can exit normally
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        ZeroMemory(&pi, sizeof(pi));
        ZeroMemory(&si, sizeof(si));
    }

    bool ExternalServerHandler::CheckConnection()
    {
        try
        {
            std::string message = "poll";
            std::string data = Send(message);
            return data == "alive" || data == "Alive";
        }
        catch (const std::exception&)
        {
            return false;
        }
    }

    void ExternalServerHandler::SetParentProcess()
    {
        DWORD processId = GetCurrentProcessId();

        std::string message = "parent:" + std::to_string(processId);
        std::string data = Send(message);
    }

    bool ExternalServerHandler::CanHandle(std::string objectType)
    {
        if (!this->server_started)
        {
            StartServer();
        }

        std::string result = this->Send("can_handle:" + objectType);
        return result == "true";
    }

    int ExternalServerHandler::GetNewId()
    {
        if (!server_started)
        {
            StartServer();
        }

        std::string result = Send("get_new_id");
        return std::stoi(result);
    }

    int ExternalServerHandler::Create(std::string objectType)
    {
        if (!server_started)
        {
            StartServer();
        }

        std::string result = Send("create:" + objectType);

        return std::stoi(result);
    }

    void ExternalServerHandler::Destroy(int id)
    {
        if (server_started)
        {
            Send("destroy:" + std::to_string(id));
        }
    }

    void ExternalServerHandler::Exit()
    {
        if (server_started)
        {
            Send("exit");
            server_started = false;
        }
    }

    double ExternalServerHandler::GetValue(int id, std::string property)
    {
        std::string result = Send("get_value:" + std::to_string(id) + ":" + property);
        return std::stod(result);
    }

    void ExternalServerHandler::SetValue(int id, std::string property, double value)
    {
        Send("set_value:" + std::to_string(id) + ":" + property + ":" + std::to_string(value));
    }

    double ExternalServerHandler::GetArgValue(int id, std::string property, double argument)
    {
        std::string result = Send("get_arg_value:" + std::to_string(id) + ":" + property + ":" + std::to_string(argument));
        return std::stod(result);
    }

    bool ExternalServerHandler::GetBoolValue(int id, std::string property)
    {
        std::string result = Send("get_bool_value:" + std::to_string(id) + ":" + property);
        return result == "true";
    }

    void ExternalServerHandler::SetBoolValue(int id, std::string property, bool value)
    {
        std::string bvalue = value ? "true" : "false";
        Send("set_bool_value:" + std::to_string(id) + ":" + property + ":" + bvalue);
    }

    int ExternalServerHandler::GetIntValue(int id, std::string property)
    {
        std::string result = Send("get_int_value:" + std::to_string(id) + ":" + property);
        return std::stoi(result);
    }

    void ExternalServerHandler::SetIntValue(int id, std::string property, int value)
    {
        Send("set_int_value:" + std::to_string(id) + ":" + property + ":" + std::to_string(value));
    }

    int ExternalServerHandler::GetIdValue(int id, std::string property)
    {
        std::string result = Send("get_id_value:" + std::to_string(id) + ":" + property);
        return std::stoi(result);
    }

    std::string ExternalServerHandler::GetStringValue(int id, std::string property)
    {
        return Send("get_string_value:" + std::to_string(id) + ":" + property);
    }

    void ExternalServerHandler::SetStringValue(int id, std::string property, std::string value)
    {
        Send("set_string_value:" + std::to_string(id) + ":" + property + ":" + value);
    }

    double ExternalServerHandler::GetIndexedValue(int id, std::string property, int index)
    {
        std::string result = Send("get_indexed_value:" + std::to_string(id) + ":" + property + ":" + std::to_string(index));
        return std::stod(result);
    }

    int ExternalServerHandler::GetIndexedIntValue(int id, std::string property, int index)
    {
        std::string result = Send("get_indexed_int_value:" + std::to_string(id) + ":" + property + ":" + std::to_string(index));
        return std::stoi(result);
    }

    int ExternalServerHandler::GetIndexedIdValue(int id, std::string property, int index)
    {
        std::string result = Send("get_indexed_id_value:" + std::to_string(id) + ":" + property + ":" + std::to_string(index));
        return std::stoi(result);
    }

    std::string ExternalServerHandler::GetIndexedStringValue(int id, std::string property, int index)
    {
        return Send("get_indexed_string_value:" + std::to_string(id) + ":" + property + ":" + std::to_string(index));
    }

    void ExternalServerHandler::GetArrayValue(int id, std::string property, double* values, int size)
    {
        std::string result = Send("get_array_value:" + std::to_string(id) + ":" + property);

        std::vector<std::string> results = StringSplit(result, ":");
        for (size_t i = 0; i < results.size(); i++)
        {
            values[i] = std::stod(results[i]);
        }
    }

    void ExternalServerHandler::SetArrayValue(int id, std::string property, double* values, int size)
    {
        std::vector<std::string> strings;
        for (int i = 0; i < size; i++)
        {
            strings.push_back(std::to_string(values[i]));
        }

        Send("set_array_value:" + std::to_string(id) + ":" + property + ":" + StringJoin(strings, ":"));
    }

    void ExternalServerHandler::SetArrayIntValue(int id, std::string property, int* values, int size)
    {
        std::vector<std::string> strings;
        for (int i = 0; i < size; i++)
        {
            strings.push_back(std::to_string(values[i]));
        }

        Send("set_array_int_value:" + std::to_string(id) + ":" + property + ":" + StringJoin(strings, ":"));
    }

    void ExternalServerHandler::GetArgValues(int id, std::string property, double* values, int size, double* outputValues)
    {
        std::vector<std::string> strings;
        for (int i = 0; i < size; i++)
        {
            strings.push_back(std::to_string(values[i]));
        }

        std::string result = Send("get_arg_values:" + std::to_string(id) + ":" + property + ":" + StringJoin(strings, ":"));

        std::vector<std::string> results = StringSplit(result, ":");
        for (size_t i = 0; i < results.size(); i++)
        {
            outputValues[i] = std::stod(results[i]);
        }
    }

    double ExternalServerHandler::GetIndexedIndexedValue(int id, std::string property, int index1, int index2)
    {
        std::string result = Send("get_indexed_indexed_value:" + std::to_string(id) + ":" + property + ":" + std::to_string(index1) + ":" + std::to_string(index2));
        return std::stod(result);
    }

    void ExternalServerHandler::SetIndexedIndexedValue(int id, std::string property, int index1, int index2, double value)
    {
        Send("set_indexed_indexed_value:" + std::to_string(id) + ":" + property + ":" + std::to_string(index1) + ":" + std::to_string(index2) + ":" + std::to_string(value));
    }

    void ExternalServerHandler::SetIndexedIndexedIntValue(int id, const std::string& property, int index1, int index2, int value)
    {
        const auto message = std::format("set_indexed_indexed_int_value: {}:{}:{}:{}:{}",
            id, property, index1, index2, value);
        Send(message);
    }

    void ExternalServerHandler::Execute(int id, std::string method)
    {
        Send("execute:" + std::to_string(id) + ":" + method);
    }

    std::string ExternalServerHandler::StringJoin(const std::vector<std::string>& strings, const std::string& delim)
    {
        if (strings.empty()) return "";
        std::vector<char> res;
        for (size_t i = 0; i < strings.size() - 1; ++i)
        {
            for (auto c : strings[i]) res.push_back(c);
            for (auto c : delim) res.push_back(c);
        }
        for (auto c : strings[strings.size() - 1]) res.push_back(c);
        return std::string{ res.begin(), res.end() };
    }

    std::vector<std::string> ExternalServerHandler::StringSplit(std::string& text, const std::string& delimiter)
    {
        std::vector<std::string> tokens;
        size_t pos = 0;
        size_t delimiterLength = delimiter.length();
        while ((pos = text.find(delimiter)) != std::string::npos)
        {
            std::string token = text.substr(0, pos);
            tokens.push_back(token);
            text.erase(0, pos + delimiterLength);
        }
        tokens.push_back(text);

        return tokens;
    }
#endif

}

