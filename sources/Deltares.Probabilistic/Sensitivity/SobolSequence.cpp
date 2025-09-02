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
#include "SobolSequence.h"

#include <stdexcept>

namespace Deltares::Sensitivity
{
    uint32_t SobolSequence::nextUInt() {
        uint32_t yn = X;
        uint32_t m = direction.v[0];

        // EQUIVALENT OF K FOR THE DIRECTION NUMBER
        uint32_t r = rightmostZeroBit(k++);
        uint32_t v = direction.v[r];

        // generate next value
        X = yn ^ m ^ v;

        return yn;
    }

    int SobolSequence::next()
    {
        return static_cast<int>(nextDouble() * std::numeric_limits<int32_t>::max());
    }

    int SobolSequence::next(int maxValue)
    {
        if (maxValue < 0)
        {
            throw std::out_of_range("maxValue is negative");
        }
        return (next() * (maxValue + 1)) << 0;
    }

    int SobolSequence::next(int minValue, int maxValue)
    {
        if (minValue > maxValue)
        {
            throw std::out_of_range("minValue is smaller than maxValue");
        }
        return (next() * (maxValue - minValue + 1)) << 0;
    }

    double SobolSequence::nextDouble()
    {
        const double maxValue = 4294967296.0;

        double random = nextUInt();
        double val = random / maxValue;
        return val;
    }

    uint32_t SobolSequence::rightmostZeroBit(const uint32_t k)
    {
        uint32_t n = k | 0;
        uint32_t i = 1;

        while ((n & 1) == 1)
        {
            n >>= 1;
            ++i;
        }

        return i;
    }
};

