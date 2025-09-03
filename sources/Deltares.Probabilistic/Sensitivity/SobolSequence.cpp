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
#include "../Math/BinarySupport.h"

#include <stdexcept>

namespace Deltares::Sensitivity
{
    unsigned int SobolSequence::nextUInt()
    {
        unsigned int yn = X;
        unsigned int m = direction.values[0];

        // EQUIVALENT OF K FOR THE DIRECTION NUMBER
        unsigned int r = Mathematics::BinarySupport::RightmostZeroBit(k++);
        unsigned int v = direction.values[r];

        // generate next value
        X = yn ^ m ^ v;

        return yn;
    }

    int SobolSequence::next()
    {
        return static_cast<int>(nextDouble() * std::numeric_limits<int>::max());
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
        constexpr double maxValue = std::numeric_limits<unsigned int>::max();

        double random = nextUInt();
        double value = random / maxValue;
        return value;
    }
};

