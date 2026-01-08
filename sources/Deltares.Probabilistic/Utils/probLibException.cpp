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
#include "probLibException.h"
#include <format>

namespace Deltares::Reliability
{
    probLibException::probLibException(const std::string& message, const double number)
        : message(std::format("{} {}" , message, toString(number)))
    {
    }

    probLibException::probLibException(const std::string& message, const int number)
        : message(std::format("{} {}", message , number))
    {
    }

    probLibException::probLibException(const std::string& message, const size_t number)
        : message(std::format("{} {}", message, number))
    {
    }

    std::string probLibException::toString(const double number)
    {
        constexpr double threshold = 1e5;
        if (std::abs(number) > threshold)
        {
            return std::format("{:.4e}", number);
        }
        else
        {
            return std::format("{:.4f}", number);
        }
    }


}
