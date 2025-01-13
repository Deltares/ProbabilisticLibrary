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
#include "../Utils/probLibException.h"

namespace Deltares::Numeric
{
    using enum RandomValueGeneratorType;

    void Random::initialize(RandomValueGeneratorType generatorType, bool repeatable, int seed)
    {
        if (generatorType == MersenneTwister)
        {
            randomValueGenerator = std::make_unique<MersenneTwisterRandomValueGenerator>();
        }
        else
        {
            throw Reliability::probLibException("Generator type not supported");
        }

        repeatable_ = repeatable;
        seed_ = seed;

        randomValueGenerator->initialize(repeatable, seed);
    }

    double Random::next() const
    {
        return randomValueGenerator->next();
    }

    void Random::restart() const
    {
        return randomValueGenerator->initialize(repeatable_, seed_);
    }

    std::string Random::getRandomGeneratorTypeString([[maybe_unused]] RandomValueGeneratorType method)
    {
        return "mersenne_twister";
    }

    RandomValueGeneratorType Random::getRandomGeneratorType([[maybe_unused]] const std::string& method)
    {
        return MersenneTwister;
    }
}

