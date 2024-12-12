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
    namespace Models
    {
        namespace Wrappers
        {
            public delegate void ManagedInitializeRandomDelegate(bool repeatable, int seed);
            public delegate double ManagedNextRandomDelegate();

            ref class RandomProvider
            {
            private:
                static System::Random^ random = gcnew System::Random();

                static void initializeInitializeDelegate();
                static void initializeNextDelegate();
                static bool initialized = false;

                static void initialize(bool repeatable, int seed)
                {
                    if (repeatable)
                    {
                        random = gcnew System::Random(seed);
                    }
                    else
                    {
                        random = gcnew System::Random();
                    }
                }

                static double next()
                {
                    return random->NextDouble();
                }

            public:
                static void initialize();
            };
        }
    }
}

