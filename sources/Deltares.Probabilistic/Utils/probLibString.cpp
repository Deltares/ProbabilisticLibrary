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
#include "probLibString.h"
#include <string.h> //For strcasecmp(). Also could be found in <mem.h>
#include <cmath>
#include <algorithm>
#include <cctype>

#include "probLibException.h"

namespace Deltares {
    namespace Reliability {

#ifdef _MSC_VER
        //not #if defined(_WIN32) || defined(_WIN64) because we have strncasecmp in mingw
#define strncasecmp _strnicmp
#define strcasecmp _stricmp
#endif

        bool probLibString::iStrcmp(const std::string& s1, const std::string& s2)
        {
            if (s1.length() != s2.length())
                return false;  // optimization since std::string holds length in variable.
            return strcasecmp(s1.c_str(), s2.c_str()) == 0;
        }

        bool probLibString::iFind(const std::string& s1, const std::string& s2)
        {
            return (strToLower(s1).find(strToLower(s2)) < s1.length());
        }

        std::string probLibString::strToLower(const std::string& data)
        {
            std::string s = data;
            std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::tolower(c); });
            return s;
        }

        std::string probLibString::double2str(const double x)
        {
            constexpr size_t buffer_size = 32;
            char buffer[buffer_size];
            if (x == 0.0 || (fabs(x) > 1e-5 && fabs(x) < 1e10))
            {
                snprintf(buffer, buffer_size, "%15.6f", x);
            }
            else
            {
                snprintf(buffer, buffer_size, "%15.6e", x);
            }
            std::string return_value = buffer;
            return return_value;
        }

        std::string probLibString::double2strTrimmed(const double x)
        {
            auto str = double2str(x);
            str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
            return str;
        }

        std::string probLibString::doubles2str(const std::vector<double>& x)
        {
            std::string s;
            for (const auto& value : x)
            {
                if (!s.empty()) s += "_";
                s += double2str(value);
            }
            return s;
        }

        std::string probLibString::doubles2strTrimmed(const std::vector<double>& x)
        {
            std::string s;
            for (const auto& value : x)
            {
                if (!s.empty()) s += "_";
                s += double2strTrimmed(value);
            }
            return s;
        }

        // trim from end of string (right)
        std::string probLibString::rtrim(const std::string& s, const char* t)
        {
            std::string r = s;
            r.erase(r.find_last_not_of(t) + 1);
            return r;
        }

        // trim from beginning of string (left)
        std::string probLibString::ltrim(const std::string& s, const char* t)
        {
            std::string r = s;
            r.erase(0, r.find_first_not_of(t));
            return r;
        }

        // trim from both ends of string (right then left)
        std::string probLibString::trim(const std::string& s, const char* t)
        {
            auto trimmed = rtrim(s, t);
            return ltrim(trimmed, t);
        }
    }
}
