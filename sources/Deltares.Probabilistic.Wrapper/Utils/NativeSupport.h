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

namespace Deltares
{
    namespace Utils
    {
        namespace Wrappers
        {
            public ref class NativeSupport
            {
            public:
                static std::string toNative(System::String^ text);
                static System::String^ toManaged(std::string text);

                static std::vector<double> toNative(array<double>^ values);
                static array<double>^ toManaged(std::vector<double>& values, int count);
                static array<double>^ toManaged(const std::vector<double> & values);

                static std::vector<int> toNative(array<int>^ values);
            private:
                static System::String^ escape(System::String^ text);
                static System::String^ unescape(System::String^ text);
            };
        }
    }
}

