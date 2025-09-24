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
#pragma once
#include <string>

namespace Deltares
{
    namespace Logging
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
            std::string Subject = "";

            static std::string getMessageTypeString(MessageType type);
            static MessageType getMessageType(std::string type);
        };
    }
}

