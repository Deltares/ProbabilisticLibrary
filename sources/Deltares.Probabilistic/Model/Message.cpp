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

