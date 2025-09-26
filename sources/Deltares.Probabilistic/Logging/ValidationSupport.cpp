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
#include "ValidationSupport.h"

#include <utility>
#include <cmath>


namespace Deltares
{
    namespace Logging
    {
        void ValidationSupport::checkNotNull(Logging::ValidationReport& report, const bool value, const
            std::string& property, std::string subject, MessageType messageType)
        {
            if (value)
            {
                std::shared_ptr<Message> message = std::make_shared<Message>();
                message->Text = property + " is not assigned";
                message->Type = messageType;
                message->Subject = std::move(subject);

                report.messages.push_back(message);
            }
        }

        void ValidationSupport::checkFinite(Logging::ValidationReport& report, double value, const
            std::string& property, std::string subject, MessageType messageType)
        {
            if (std::isinf(value))
            {
                std::shared_ptr<Message> message = std::make_shared<Message>();
                message->Text = property + " is infinite";
                message->Type = messageType;
                message->Subject = std::move(subject);

                report.messages.push_back(message);
            }
        }

        void ValidationSupport::checkMinimum(Logging::ValidationReport& report, double minimum, double value, const
            std::string& property, std::string subject, MessageType messageType)
        {
            if (value < minimum)
            {
                std::shared_ptr<Message> message = std::make_shared<Message>();
                message->Text = property + " (" + std::to_string(value) + ") is less than " + std::to_string(minimum);
                message->Type = messageType;
                message->Subject = std::move(subject);

                report.messages.push_back(message);
            }
        }

        void ValidationSupport::checkMinimumNonInclusive(Logging::ValidationReport& report, double minimum, double value, const
            std::string& property, std::string subject, MessageType messageType)
        {
            if (value <= minimum)
            {
                std::shared_ptr<Message> message = std::make_shared<Message>();
                message->Text = property + " (" + std::to_string(value) + ") is not greater than " + std::to_string(minimum);
                message->Type = messageType;
                message->Subject = std::move(subject);

                report.messages.push_back(message);
            }
        }

        void ValidationSupport::checkMaximum(Logging::ValidationReport& report, double maximum, double value, const
            std::string& property, std::string subject, MessageType messageType)
        {
            if (value > maximum)
            {
                std::shared_ptr<Message> message = std::make_shared<Message>();
                message->Text = property + " (" + std::to_string(value) + ") is greater than " + std::to_string(maximum);
                message->Type = messageType;
                message->Subject = std::move(subject);

                report.messages.push_back(message);
            }
        }

        void ValidationSupport::checkMaximumNonInclusive(Logging::ValidationReport& report, double maximum, double value, const
            std::string& property, std::string subject, MessageType messageType)
        {
            if (value >= maximum)
            {
                std::shared_ptr<Message> message = std::make_shared<Message>();
                message->Text = property + " (" + std::to_string(value) + ") is not less than " + std::to_string(maximum);
                message->Type = messageType;
                message->Subject = std::move(subject);

                report.messages.push_back(message);
            }
        }

        void ValidationSupport::checkNotEmpty(Logging::ValidationReport& report, size_t size, const std::string& property,
            std::string subject, MessageType messageType)
        {
            if (size == 0)
            {
                std::shared_ptr<Message> message = std::make_shared<Message>();
                message->Text = property + " is empty";
                message->Type = messageType;
                message->Subject = std::move(subject);

                report.messages.push_back(message);
            }
        }

        void ValidationSupport::add(Logging::ValidationReport& report, std::string text, std::string subject, MessageType messageType)
        {
            std::shared_ptr<Message> message = std::make_shared<Message>();
            message->Text = std::move(text);
            message->Type = messageType;
            message->Subject = std::move(subject);

            report.messages.push_back(message);
        }

    }
}

