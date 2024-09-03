#pragma once
#include <string>

namespace Deltares
{
    namespace Models
    {
        enum MessageType { Debug, Info, Warning, Error };

        class Message
        {
        public:
            Message() { }

            Message(MessageType type, std::string text)
            {
                this->Type = type;
                this->Text = text;
            }

            MessageType Type = MessageType::Error;
            std::string Text = "";

            static std::string getMessageTypeString(MessageType type);
            static MessageType getMessageType(std::string type);
        };
    }
}

