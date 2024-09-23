#pragma once

#include "BaseHandler.h"
#include "../Utils/probLibException.h"

#define _WINSOCKAPI_

#if __has_include(<windows.h>)
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
                if (this->server_started)
                {
                    this->Send("exit", false);
                    this->server_started = false;
                }

                this->DisconnectSocket();
            }

            void SetServer(std::shared_ptr<BaseServer> server, int handlerIndex, std::shared_ptr<BaseHandler> defaultHandler) override
            {
                BaseHandler::SetServer(server, handlerIndex, defaultHandler);
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
            void SetArrayIntValue(int id, std::string property_, int* values, int size) override;
            Models::ZLambda GetCallBack(int id, std::string method) override;
        private:
            std::string serverName = "";
            bool server_started = false;

            WSADATA wsaData;
            SOCKET server_socket = INVALID_SOCKET;

            std::string Send(std::string message, bool waitForAnswer);
            SOCKET ConnectSocket();
            void DisconnectSocket();
            void StartServer();
            bool CheckConnection();

            std::string StringJoin(const std::vector<std::string>& strings, const std::string delim);
            void StartProcess(std::string processName, bool waitForExit);



        };
    }
}

