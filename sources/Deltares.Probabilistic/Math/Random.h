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
#include "Randomizers/RandomValueGenerator.h"
#include <string>
#include <memory>

namespace Deltares::Numeric
{
    /**
     * MersenneTwister : default for all languages
     */
    enum class RandomValueGeneratorType { MersenneTwister };

    class Random
    {
    private:
        bool repeatable_ = true;
        int seed_ = 0;
        time_t timeStamp = 0;
        std::unique_ptr<RandomValueGenerator> randomValueGenerator = nullptr;

    public:
        /**
         * \brief Initializes the random value generator
         * \param generatorType The algorithm which produces random values
         * \param repeatable Indicates whether each time this method is called, subsequent calls to the random generator produces equal results
         * \param seed Seed value needed for random generation
         * \param fixedTimeStamp The time stamp to be used if not repeatable, 0 will generate a new time stamp 
         */
        void initialize(RandomValueGeneratorType generatorType, bool repeatable = true, int seed = 0, time_t fixedTimeStamp = 0);

        /**
         * \brief Retrieves the next random value
         * \returns Random value
         */
        double next() const;

        /**
         * \brief Restarts the random value generator, so that same random values are generated (even if repeatable is false)
         */
        void restart() const;

        /**
         * \brief Restarts the random value generator, so that same random values are generated (even if repeatable is false)
         */
        void fixateInitialize() {}


        static std::string getRandomGeneratorTypeString(RandomValueGeneratorType method);
        static RandomValueGeneratorType getRandomGeneratorType(const std::string& method);
    };
}

