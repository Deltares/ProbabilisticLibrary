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
#include <vector>

namespace Deltares::Sensitivity
{
    /**
     * \brief Contains a sobol direction
     */
    class SobolDirection
    {
    public:
        /**
         * \brief Index of the sobol direction
         */
        int index = 0;

        /**
         * \brief Size of the values
         */
        uint32_t size = 0;

        /**
         * \brief a-parameter
         */
        uint32_t a = 0;

        /**
         * \brief Values of the direction
         */
        std::vector<uint32_t> values;

        SobolDirection() = default;

        SobolDirection(int d, uint32_t s, uint32_t a, const std::vector<uint32_t>& v)
            : index(d), size(s), a(a), values(v) {
        }
    };
}

