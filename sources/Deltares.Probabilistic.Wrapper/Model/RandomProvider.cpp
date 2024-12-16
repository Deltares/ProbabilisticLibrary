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
#include "RandomProvider.h"

#include "../../Deltares.Probabilistic/Math/Randomizers/ModifiedKnuthSubtractiveRandomValueGenerator.h"

namespace Deltares
{
    namespace Models
    {
        namespace Wrappers
        {
            void RandomProvider::initializeInitializeDelegate()
            {
                ManagedInitializeRandomDelegate^ fp = gcnew ManagedInitializeRandomDelegate(RandomProvider::initialize);
                System::Runtime::InteropServices::GCHandle gch = System::Runtime::InteropServices::GCHandle::Alloc(fp);

                System::IntPtr callbackPtr = System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(fp);
                InitializeRandomDelegate functionPointer = static_cast<InitializeRandomDelegate>(callbackPtr.ToPointer());

                Numeric::ModifiedKnuthSubtractiveRandomValueGenerator::setInitializeDelegate(functionPointer);
            }

            void RandomProvider::initializeNextDelegate()
            {
                ManagedNextRandomDelegate^ fp = gcnew ManagedNextRandomDelegate(RandomProvider::next);
                System::Runtime::InteropServices::GCHandle gch = System::Runtime::InteropServices::GCHandle::Alloc(fp);

                System::IntPtr callbackPtr = System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(fp);
                NextRandomDelegate functionPointer = static_cast<NextRandomDelegate>(callbackPtr.ToPointer());

                Numeric::ModifiedKnuthSubtractiveRandomValueGenerator::setNextDelegate(functionPointer);
            }

            void RandomProvider::initialize()
            {
                if (!initialized)
                {
                    initialized = true;
                    initializeInitializeDelegate();
                    initializeNextDelegate();
                }
            }
        }
    }
}

