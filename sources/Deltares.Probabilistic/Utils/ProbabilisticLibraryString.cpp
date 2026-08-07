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
#include "ProbabilisticLibraryString.h"
#include <algorithm>
#include <cctype>
#include <format>

namespace Deltares::Reliability
{
    std::string ProbabilisticLibraryString::strToLower(const std::string& data)
    {
        std::string s = data;
        std::ranges::transform(s, s.begin(), [](unsigned char c) { return std::tolower(c); });
        return s;
    }

    std::string ProbabilisticLibraryString::double2str(const double x)
    {
        return std::format("{:15.6g}", x);
    }

    std::string ProbabilisticLibraryString::double2strTrimmed(const double x)
    {
        auto str = double2str(x);
        std::erase(str, ' ');
        return str;
    }

    std::string ProbabilisticLibraryString::doubles2strTrimmed(const std::vector<double>& x)
    {
        std::string s;
        for (const auto& value : x)
        {
            if (!s.empty()) s += "_";
            s += double2strTrimmed(value);
        }
        return s;
    }

    std::string ProbabilisticLibraryString::double2strForText(const double x)
    {
        return removeTrailingZeros(double2strTrimmed(x));
    }

    std::string ProbabilisticLibraryString::removeTrailingZeros(const std::string& s)
    {
        int i = static_cast<int>(s.length()) - 1;
        int p = static_cast<int>(s.find_last_of('.')); // index of decimal point

        if (p < 0)
        {
            return s;
        }
        else
        {
            while (i >= 0 && i > p && s[i] == '0') --i;
            if (s[i] == '.') --i; // remove the decimal point too if this is the last character
            std::string res = s.substr(0, i + 1);

            return res;
        }
    }

    // trim from end of string (right)
    std::string ProbabilisticLibraryString::rtrim(const std::string& s, const char* t)
    {
        std::string r = s;
        r.erase(r.find_last_not_of(t) + 1);
        return r;
    }

    // trim from beginning of string (left)
    std::string ProbabilisticLibraryString::ltrim(const std::string& s, const char* t)
    {
        std::string r = s;
        r.erase(0, r.find_first_not_of(t));
        return r;
    }

    // trim from both ends of string (right then left)
    std::string ProbabilisticLibraryString::trim(const std::string& s, const char* t)
    {
        auto trimmed = rtrim(s, t);
        return ltrim(trimmed, t);
    }
}
