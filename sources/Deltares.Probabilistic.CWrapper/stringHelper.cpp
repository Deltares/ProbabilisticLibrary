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
#include "stringHelper.h"

tError fillErrorStruct(const std::string& error_message)
{
    tError error;
    size_t length = std::min(error_message.length(), error_message_length);
    for (size_t i = 0; i < length; i++)
    {
        error.errorMessage[i] = error_message[i];
    }
    size_t last = std::min(error_message.length(), error_message_length-1);
    error.errorMessage[last] = '\0';
    error.errorCode = -1;
    return error;
}

void copyStringToCharPointer(const std::string& str, char* p, const size_t size)
{
    const char* cStr = str.c_str();

#ifdef __GNUC__
    snprintf(p, size, "%s", cStr);
#else
    _snprintf_s(p, size, _TRUNCATE, cStr);
#endif
}
