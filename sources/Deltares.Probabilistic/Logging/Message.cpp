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

namespace Deltares::Logging
{
    using enum MessageType;

    std::string Message::getMessageTypeString(MessageType type)
    {
        switch (type)
        {
        case Debug: return "debug";
        case Info: return "info";
        case Warning: return "warning";
        case Error: return "error";
        default: throw Reliability::probLibException("message type");
        }
    }

    MessageType Message::getMessageType(const std::string& type)
    {
        if (type == "debug")  return Debug;
        else if (type == "info") return Info;
        else if (type == "warning") return Warning;
        else if (type == "error") return Error;
        else throw Reliability::probLibException("message type " + type + " not a known message type");
    }
}

