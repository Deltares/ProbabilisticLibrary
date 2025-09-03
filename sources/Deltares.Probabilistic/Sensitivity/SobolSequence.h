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

#include "SobolDirection.h"

namespace Deltares::Sensitivity
{
    /**
     * \brief Randomizer which is capable of producing more widely spread random values
     */
    class SobolSequence
    {
    public:
        /**
         * \brief Constructor
         * \param direction Direction on which the random values will be based
         */
        SobolSequence(SobolDirection direction)
        {
            this->direction = direction;

            // skip first value
            nextUInt();
        }

        /**
         * \brief Gets the next random int value
         * \return Random value
         */
        int next();

        /**
         * \brief Gets the next random int value
         * \param maxValue Maximum value to be produced
         * \return Random value
         */
        int next(int maxValue);

        /**
         * \brief Gets the next random int value
         * \param minValue Minimum value to be produced
         * \param maxValue Maximum value to be produced
         * \return Random value
         */
        int next(int minValue, int maxValue);

        /**
         * \brief Gets the next random double value between 0 and 1
         * \return Random value
         */
        double nextDouble();
    private:
        SobolDirection direction;
        uint32_t k = 1;
        uint32_t X = 0;
        uint32_t nextUInt();
        uint32_t rightmostZeroBit(const uint32_t k);
    };
}

