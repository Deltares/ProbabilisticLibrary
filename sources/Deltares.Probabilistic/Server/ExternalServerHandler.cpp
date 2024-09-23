#include "ExternalServerHandler.h"

#include <string>
#include <cstring>

#define _WINSOCKAPI_

#if __has_include(<windows.h>)

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#endif

#include <stdexcept>

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "11178"
#define DEFAULT_SERVER "localhost"

#if __has_include(<windows.h>)
namespace Deltares
{
    namespace Server
    {
        // implementation based on https ://learn.microsoft.com/en-us/windows/win32/winsock/complete-client-code

        SOCKET ExternalServerHandler::ConnectSocket()
        {
            SOCKET ConnectSocket = INVALID_SOCKET;
            struct addrinfo* address = NULL, * ptr = NULL, hints;

            // Initialize Winsock
            int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
            if (iResult != 0)
            {
                printf("WSAStartup failed with error: %d\n", iResult);
                return 0;
            }

            ZeroMemory(&hints, sizeof(hints));
            hints.ai_family = AF_INET;
            hints.ai_socktype = SOCK_STREAM;
            hints.ai_protocol = IPPROTO_TCP;

            char* hostname = new char[256];
            gethostname(hostname, 256);

            // Resolve the server address and port
            iResult = getaddrinfo(hostname, DEFAULT_PORT, &hints, &address);
            if (iResult != 0)
            {
                printf("getaddrinfo failed with error: %d\n", iResult);
                WSACleanup();
                return 1;
            }

            // Attempt to connect to an address until one succeeds
            for (ptr = address; ptr != NULL; ptr = ptr->ai_next) {

                // Create a SOCKET for connecting to server
                ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
                if (ConnectSocket == INVALID_SOCKET)
                {
                    printf("socket failed with error: %ld\n", WSAGetLastError());
                    WSACleanup();
                    return 1;
                }

                // Connect to server.
                iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
                if (iResult == SOCKET_ERROR)
                {
                    printf("socket connection failed with error: %ld\n", WSAGetLastError());

                    closesocket(ConnectSocket);
                    ConnectSocket = INVALID_SOCKET;
                    continue;
                }
                break;
            }

            freeaddrinfo(address);

            if (ConnectSocket == INVALID_SOCKET) {
                printf("Unable to connect to server!\n");
                WSACleanup();
                return 1;
            }

            return ConnectSocket;
        }

        void ExternalServerHandler::DisconnectSocket()
        {
            // shutdown the connection since no more data will be sent
            int iResult = shutdown(this->server_socket, SD_SEND);

            if (iResult == SOCKET_ERROR)
            {
                printf("shutdown failed with error: %d\n", WSAGetLastError());
                closesocket(this->server_socket);
                WSACleanup();
            }

            char recvbuf[DEFAULT_BUFLEN];
            int recvbuflen = DEFAULT_BUFLEN;

            // Receive until the peer closes the connection
            do {
                iResult = recv(this->server_socket, recvbuf, recvbuflen, 0);
                if (iResult > 0)
                    printf("Bytes received: %d\n", iResult);
                else if (iResult == 0)
                    printf("Connection closed\n");
                else
                    printf("recv failed with error: %d\n", WSAGetLastError());

            } while (iResult > 0);

            // cleanup
            closesocket(this->server_socket);
            WSACleanup();

            this->server_socket = INVALID_SOCKET;
        }

        std::string ExternalServerHandler::Send(std::string message, bool waitForAnswer)
        {
            const char* sendbuf = message.c_str();

            // Send an initial buffer
            int iResult = send(this->server_socket, sendbuf, message.size() + 1, 0);
            if (iResult == SOCKET_ERROR)
            {
                printf("send failed with error: %d\n", WSAGetLastError());
                closesocket(this->server_socket);
                WSACleanup();
                return "";
            }

            if (waitForAnswer)
            {
                char recvbuf[DEFAULT_BUFLEN];
                int recvbuflen = DEFAULT_BUFLEN;

                int received = recv(this->server_socket, recvbuf, sizeof(recvbuf), 0);
                if (received < 0)
                {
                    throw std::runtime_error("Receive failed");
                }

                return std::string(recvbuf, received);
            }

            return "";
        }

        void ExternalServerHandler::StartServer()
        {
            bool connected = false;
            int count = 0;

            while (!connected && count < 10)
            {
                StartProcess(this->serverName, false);

                this->server_started = true;

                std::this_thread::sleep_for(std::chrono::seconds(1));

                this->server_socket = this->ConnectSocket();

                connected = CheckConnection();
                if (!connected)
                {
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    count++;
                }
            }
        }

        void ExternalServerHandler::StartProcess(std::string processName, bool waitForExit)
        {
            STARTUPINFO si;
            PROCESS_INFORMATION pi;

            std::wstring processNameW(processName.begin(), processName.end());

            ZeroMemory(&si, sizeof(si));
            si.cb = sizeof(si);
            ZeroMemory(&pi, sizeof(pi));

            // Start the child process. 
            if (!CreateProcess(NULL,   // No module name (use command line)
                processNameW.data(),        // Command line
                NULL,           // Process handle not inheritable
                NULL,           // Thread handle not inheritable
                FALSE,          // Set handle inheritance to FALSE
                0,              // No creation flags
                NULL,           // Use parent's environment block
                NULL,           // Use parent's starting directory 
                &si,            // Pointer to STARTUPINFO structure
                &pi)           // Pointer to PROCESS_INFORMATION structure
                )
            {
                printf("CreateProcess failed (%d).\n", GetLastError());
                return;
            }

            if (waitForExit)
            {
                // Wait until child process exits.
                WaitForSingleObject(pi.hProcess, INFINITE);

                // Close process and thread handles. 
                CloseHandle(pi.hProcess);
                CloseHandle(pi.hThread);
            }
        }

        bool ExternalServerHandler::CheckConnection()
        {
            try
            {
                std::string message = "poll";
                std::string data = Send(message, true);
                return true;
            }
            catch (const std::exception& e)
            {
                return false;
            }
        }

        void ExternalServerHandler::Initialize()
        {
            this->StartServer();
        }

        bool ExternalServerHandler::CanHandle(std::string objectType)
        {
            std::string result = this->Send("can_handle:" + objectType, true);
            return result == "true";
        }

        void ExternalServerHandler::Create(std::string objectType, int id)
        {
            this->Send("create:" + objectType + ":" + std::to_string(id), false);
        }

        void ExternalServerHandler::Destroy(int id)
        {
            this->Send("destroy:" + std::to_string(id), false);
        }

        double ExternalServerHandler::GetValue(int id, std::string property)
        {
            std::string result = this->Send("get_value:" + property, true);
            return std::stod(result);
        }

        void ExternalServerHandler::SetValue(int id, std::string property, double value)
        {
            this->Send("set_value:" + property + ":" + std::to_string(value), false);
        }

        bool ExternalServerHandler::GetBoolValue(int id, std::string property)
        {
            std::string result = this->Send("get_bool_value:" + property, true);
            return result == "true";
        }

        void ExternalServerHandler::SetBoolValue(int id, std::string property, bool value)
        {
            std::string bvalue = value ? "true" : "false";
            this->Send("set_bool_value:" + property + ":" + bvalue, false);
        }

        int ExternalServerHandler::GetIntValue(int id, std::string property)
        {
            std::string result = this->Send("get_int_value:" + property, true);
            return std::stoi(result);
        }

        void ExternalServerHandler::SetIntValue(int id, std::string property, int value)
        {
            this->Send("set_int_value:" + property + ":" + std::to_string(value), false);
        }

        std::string ExternalServerHandler::GetStringValue(int id, std::string property)
        {
            return this->Send("get_string_value:" + property, true);
        }

        void ExternalServerHandler::SetStringValue(int id, std::string property, std::string value)
        {
            this->Send("set_string_value:" + property + ":" + value, false);
        }

        int ExternalServerHandler::GetIndexedIntValue(int id, std::string property, int index)
        {
            std::string result = this->Send("get_indexed_int_value:" + property + ":" + std::to_string(index), true);
            return std::stoi(result);
        }

        void ExternalServerHandler::SetArrayIntValue(int id, std::string property, int* values, int size)
        {
            std::vector<std::string> strings;
            for (int i = 0; i < size; i++)
            {
                strings.push_back(std::to_string(values[i]));
            }

            this->Send("set_array_int_value:" + property + ":" + StringJoin(strings, ":"), false);
        }

        Models::ZLambda ExternalServerHandler::GetCallBack(int id, std::string method)
        {
            return nullptr;
        }

        std::string ExternalServerHandler::StringJoin(const std::vector<std::string>& strings, const std::string delim)
        {
            if (strings.size() == 0) return "";
            std::vector<char> res;
            for (int i = 0; i < strings.size() - 1; ++i)
            {
                for (auto c : strings[i]) res.push_back(c);
                for (auto c : delim) res.push_back(c);
            }
            for (auto c : strings[strings.size() - 1]) res.push_back(c);
            return std::string{ res.begin(), res.end() };
        }
    }
}
#endif

