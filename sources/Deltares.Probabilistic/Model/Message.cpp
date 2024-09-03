#include "Message.h"
#include "../Utils/probLibException.h"

namespace Deltares
{
    namespace Models
    {
        std::string Message::getMessageTypeString(MessageType type)
        {
            switch (type)
            {
            case MessageType::Debug: return "debug";
            case MessageType::Info: return "info";
            case MessageType::Warning: return "warning";
            case MessageType::Error: return "error";
            default: throw Reliability::probLibException("message type");
            }
        }

        MessageType Message::getMessageType(std::string type)
        {
            if (type == "debug")  return MessageType::Debug;
            else if (type == "info") return MessageType::Info;
            else if (type == "warning") return MessageType::Warning;
            else if (type == "error") return MessageType::Error;
            else throw Reliability::probLibException("message type");
        }
    }
}

