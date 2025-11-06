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
#include "RandomValueGenerator.h"
#include "../Utils/probLibException.h"
#include <ctime>

namespace Deltares::Numeric
{
    void RandomValueGenerator::initialize(bool repeatable, int seed, time_t fixedTimeStamp)
    {
        repeatable_ = repeatable;
        seed_ = seed;

        if (!repeatable)
        {
            if (fixedTimeStamp == 0)
            {
                timeStamp = time(nullptr);
                if (timeStamp <= lastGeneratedTimeStamp)
                {
                    timeStamp = lastGeneratedTimeStamp + 1;
                }

                lastGeneratedTimeStamp = this->timeStamp;
            }
            else
            {
                timeStamp = fixedTimeStamp;
            }
        }

        initializeGenerator(repeatable, seed, timeStamp);
    }

    double RandomValueGenerator::next()
    {
        std::uniform_real_distribution<double> distribution(0.0, 1.0);
        double ri = distribution(generator);
        return ri;
    }

    void RandomValueGenerator::restart()
    {
        initializeGenerator(repeatable_, seed_, timeStamp);
    }

    void RandomValueGenerator::initializeGenerator(bool repeatable, int seed, time_t timeStamp)
    {
        if (repeatable)
        {
            generator.seed(seed);
        }
        else
        {
            generator.seed(timeStamp);
        }
    }
}

