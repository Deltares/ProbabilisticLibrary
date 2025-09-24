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

#include "ValidationReport.h"

namespace Deltares
{
    namespace Logging
    {
        class ValidationSupport
        {
        public:
            static void checkFinite(Logging::ValidationReport& report, double value, const
                std::string& property, std::string subject = "", MessageType messageType = MessageType::Error);
            static void checkMinimum(Logging::ValidationReport& report, double minimum, double value, const
                std::string& property, std::string subject = "", MessageType messageType = MessageType::Error);
            static void checkMinimumNonInclusive(Logging::ValidationReport& report, double minimum, double value, const
                std::string& property, std::string subject = "", MessageType messageType = MessageType::Error);
            static void checkMaximum(Logging::ValidationReport& report, double maximum, double value, const
                std::string& property, std::string subject = "", MessageType messageType = MessageType::Error);
            static void checkMaximumNonInclusive(Logging::ValidationReport& report, double maximum, double value, const
                std::string& property, std::string subject = "", MessageType messageType = MessageType::Error);
            static void checkNotEmpty(Logging::ValidationReport& report, size_t size, const std::string& property,
                std::string subject = "", MessageType messageType = MessageType::Error);
            static void add(Logging::ValidationReport& report, std::string text, std::string subject = "", MessageType messageType = MessageType::Error);

        };
    }
}

