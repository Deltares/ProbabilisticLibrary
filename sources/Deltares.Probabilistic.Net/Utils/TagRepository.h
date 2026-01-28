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
namespace Deltares
{
    namespace Utils
    {
        namespace Wrappers
        {
            /// <summary>
            /// Repository of objects attached to a model sample or evaluation
            /// </summary>
            public ref class TagRepository
            {
            public:
                /// <summary>
                /// Gets a numeric identifier of an object
                /// </summary>
                int RegisterTag(System::Object^ object)
                {
                    objects[++counter] = object;
                    return counter;
                }

                /// <summary>
                /// Retrieves an object by an identifier
                /// </summary>
                System::Object^ RetrieveTag(int index)
                {
                    if (objects->ContainsKey(index))
                    {
                        return objects[index];
                    }
                    else
                    {
                        return nullptr;
                    }
                }
            private:
                int counter = 0;
                System::Collections::Generic::Dictionary<int, Object^>^ objects = gcnew System::Collections::Generic::Dictionary<int, Object^>();
            };
        }
    }
}

