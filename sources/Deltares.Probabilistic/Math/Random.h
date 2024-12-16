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
#include "Randomizers/RandomValueGenerator.h"
#include <string>

namespace Deltares
{
    namespace Numeric
    {
        /**
         * MersenneTwister : C++ and Python default
         * GeorgeMarsaglia: Fortran default
         * ModifiedKnuthSubtractive: .Net default
         */
        enum RandomValueGeneratorType { MersenneTwister, GeorgeMarsaglia, ModifiedKnuthSubtractive };
        

        class Random
        {
        private:
            inline static bool repeatable = true;
            inline static int seed = 0;
            inline static int seedB = 0;
            inline static RandomValueGeneratorType generatorType = RandomValueGeneratorType::MersenneTwister;
            inline static RandomValueGenerator* randomValueGenerator = nullptr;

        public:
            static void initialize(RandomValueGeneratorType generatorType, bool repeatable = true, int seed = 0, int seedB = 0);
            static double next();
            static void restart();

            static std::string getRandomGeneratorTypeString(Deltares::Numeric::RandomValueGeneratorType method);
            static Deltares::Numeric::RandomValueGeneratorType getRandomGeneratorType(std::string method);
        };
    }
}

