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
#include "Random.h"

#include <cstdlib>
#include <exception>

#include "Randomizers/MersenneTwisterRandomValueGenerator.h"
#include "Randomizers/GeorgeMarsagliaRandomValueGenerator.h"
#include "Randomizers/ModifiedKnuthSubtractiveRandomValueGenerator.h"
#include "Randomizers/RandomValueGenerator.h"
#include "../Utils/probLibException.h"


namespace Deltares
{
    namespace Numeric
    {
        void Random::initialize(RandomValueGeneratorType generatorType, bool repeatable, int seed, int seedB)
        {
            if (generatorType == RandomValueGeneratorType::ModifiedKnuthSubtractive && !ModifiedKnuthSubtractiveRandomValueGenerator::isAvailable())
            {
                generatorType = RandomValueGeneratorType::MersenneTwister;
            }

            switch (generatorType)
            {
            case MersenneTwister: Random::randomValueGenerator = new MersenneTwisterRandomValueGenerator(); break;
            case GeorgeMarsaglia: Random::randomValueGenerator = new GeorgeMarsagliaRandomValueGenerator(); break;
            case ModifiedKnuthSubtractive: Random::randomValueGenerator = new ModifiedKnuthSubtractiveRandomValueGenerator(); break;
            default: throw Deltares::Reliability::probLibException("Generator type not supported");
            }

            Random::repeatable = repeatable;
            Random::seed = seed;
            Random::seedB = seedB;

            Random::randomValueGenerator->initialize(repeatable, seed, seedB);
        }

        double Random::next()
        {
            return Random::randomValueGenerator->next();
        }

        void Random::restart()
        {
            return Random::randomValueGenerator->initialize(Random::repeatable, Random::seed, Random::seedB);
        }

        std::string Random::getRandomGeneratorTypeString(Deltares::Numeric::RandomValueGeneratorType method)
        {
            switch (method)
            {
            case RandomValueGeneratorType::GeorgeMarsaglia: return "george_marsaglia";
            case RandomValueGeneratorType::MersenneTwister: return "mersenne_twister";
            case RandomValueGeneratorType::ModifiedKnuthSubtractive: return "modified_knuth_subtractive";
            default: throw Reliability::probLibException("Random generator type");
            }
        }

        Deltares::Numeric::RandomValueGeneratorType Random::getRandomGeneratorType(std::string method)
        {
            if (method == "george_marsaglia") return RandomValueGeneratorType::GeorgeMarsaglia;
            else if (method == "mersenne_twister") return RandomValueGeneratorType::MersenneTwister;
            else if (method == "modified_knuth_subtractive") return RandomValueGeneratorType::ModifiedKnuthSubtractive;
            else throw Reliability::probLibException("Random generator type");
        }
    }
}




