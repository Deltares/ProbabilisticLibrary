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
#include "Randomizers/MersenneTwisterRandomValueGenerator.h"
#include "Randomizers/GeorgeMarsagliaRandomValueGenerator.h"
#include "Randomizers/ModifiedKnuthSubtractiveRandomValueGenerator.h"
#include "Randomizers/RandomValueGenerator.h"
#include "../Utils/probLibException.h"

namespace Deltares::Numeric
{
    using enum RandomValueGeneratorType;

    void Random::initialize(RandomValueGeneratorType generatorType, bool repeatable, int seed, int seedB)
    {
        if (generatorType == ModifiedKnuthSubtractive && !ModifiedKnuthSubtractiveRandomValueGenerator::isAvailable())
        {
            generatorType = MersenneTwister;
        }

        switch (generatorType)
        {
        case MersenneTwister:
        {
            randomValueGenerator = std::make_unique<MersenneTwisterRandomValueGenerator>();
            break;
        }
        case GeorgeMarsaglia:
        {
            randomValueGenerator = std::make_unique <GeorgeMarsagliaRandomValueGenerator>();
            break;
        }
        case ModifiedKnuthSubtractive:
        {
            randomValueGenerator = std::make_unique <ModifiedKnuthSubtractiveRandomValueGenerator>();
            break;
        }
        default: throw Reliability::probLibException("Generator type not supported");
        }

        this->repeatable = repeatable;
        this->seed = seed;
        this->seedB = seedB;

        randomValueGenerator->initialize(repeatable, seed, seedB);
    }

    double Random::next() const
    {
        return randomValueGenerator->next();
    }

    void Random::restart() const
    {
        return randomValueGenerator->initialize(repeatable, seed, seedB);
    }

    std::string Random::getRandomGeneratorTypeString(RandomValueGeneratorType method)
    {
        switch (method)
        {
        case GeorgeMarsaglia: return "george_marsaglia";
        case MersenneTwister: return "mersenne_twister";
        case ModifiedKnuthSubtractive: return "modified_knuth_subtractive";
        default: throw Reliability::probLibException("Random generator type");
        }
    }

    RandomValueGeneratorType Random::getRandomGeneratorType(const std::string& method)
    {
        if (method == "george_marsaglia") return GeorgeMarsaglia;
        else if (method == "mersenne_twister") return MersenneTwister;
        else if (method == "modified_knuth_subtractive") return ModifiedKnuthSubtractive;
        else throw Reliability::probLibException("Random generator type");
    }
}

