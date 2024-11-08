// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of Streams.
//
// Streams is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// All names, logos, and references to "Deltares" are registered trademarks of
// Stichting Deltares and remain full property of Stichting Deltares at all times.
// All rights reserved.
//
#pragma once
#include <string>
#include <vector>

namespace Deltares {
    namespace Reliability {

        class probLibString
        {
        public:
            bool iStrcmp(const std::string& s1, const std::string& s2);
            bool iFind(const std::string& s1, const std::string& s2);
            static std::string double2str(const double x);
            static std::string double2strTrimmed(const double x);
            static std::string doubles2str(const std::vector<double>& x);
            static std::string doubles2strTrimmed(const std::vector<double>& x);

            // trim from both ends of string (right then left)
            std::string trim(const std::string& s, const char* t);

        private:
            std::string strToLower(const std::string& data);

            // trim from end of string (right)
            std::string rtrim(const std::string& s, const char* t);

            // trim from beginning of string (left)
            std::string ltrim(const std::string& s, const char* t);
        };
    }
}
