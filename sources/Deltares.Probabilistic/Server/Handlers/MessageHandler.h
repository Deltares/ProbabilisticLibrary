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
#include "StoredObjectHandler.h"
#include "../../Server/ProjectEntries.h"
#include "../../Logging/Message.h"

namespace Deltares::Server
{
    /**
     * \brief Handles properties and methods of class Message
     */
    class MessageHandler : public StoredObjectHandler<Logging::Message>
    {
    public:

        ObjectType GetObjectType() override
        {
            return ObjectType::Message;
        }

        std::string GetStringValue(int id, const std::string& property_) override
        {
            std::shared_ptr<Logging::Message> message = GetObject(id);

            if (property_ == "type") return Logging::Message::getMessageTypeString(message->Type);
            else if (property_ == "text") return message->Text;
            else if (property_ == "subject") return message->Subject;
            else return StoredObjectHandler::GetStringValue(id, property_);
        }

        void SetStringValue(int id, const std::string& property_, const std::string& value) override
        {
            std::shared_ptr<Logging::Message> message = GetObject(id);

            if (property_ == "type") message->Type = Logging::Message::getMessageType(value);
            else if (property_ == "text") message->Text = value;
            else if (property_ == "subject") message->Subject = value;
            else StoredObjectHandler::SetStringValue(id, property_, value);
        }
    };
}

