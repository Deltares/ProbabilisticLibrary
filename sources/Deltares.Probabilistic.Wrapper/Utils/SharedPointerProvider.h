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

#include <memory>
#include <vector>

namespace Deltares
{
    namespace Utils
    {
        namespace Wrappers
        {
            template <class T>

            /// <summary>
            /// Holds a smart pointer, which is not possible in a ref class
            /// </summary>
            class SharedPointerProvider
            {
            public:

                SharedPointerProvider() {}

                SharedPointerProvider(T* object)
                {
                    this->object = std::make_shared<T>(*object);
                }

                SharedPointerProvider(std::shared_ptr<T> object)
                {
                    this->object = object;
                }

                /**
                 * \brief Smart pointer, intended as read only object
                 */
                std::shared_ptr<T> object = nullptr;
            };
        }
    }
}

