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
#include "RandomValueGenerator.h"

#ifdef _WIN32
typedef void(__stdcall* InitializeRandomDelegate) (bool repeatable, int seed);
typedef double(__stdcall* NextRandomDelegate) ();
#else
typedef void(* InitializeRandomDelegate) (bool repeatable, int seed);
typedef double(* NextRandomDelegate) ();
#endif

namespace Deltares
{
    namespace Numeric
    {
        class ModifiedKnuthSubtractiveRandomValueGenerator : public RandomValueGenerator
        {
        private:
            inline static InitializeRandomDelegate initializeDelegate = nullptr;
            inline static NextRandomDelegate nextDelegate = nullptr;
        public:
            void initialize(bool repeatable, int seed, int seedB) override;
            double next() override;

            static bool isAvailable();

            static void setInitializeDelegate(InitializeRandomDelegate initializeDelegate)
            {
                ModifiedKnuthSubtractiveRandomValueGenerator::initializeDelegate = initializeDelegate;
            }

            static void setNextDelegate(NextRandomDelegate nextDelegate)
            {
                ModifiedKnuthSubtractiveRandomValueGenerator::nextDelegate = nextDelegate;
            }
        };
    }
}

