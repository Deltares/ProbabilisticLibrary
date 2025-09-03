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
        unsigned int index = 0;

        /**
         * \brief Size of the values
         */
        unsigned int size = 0;

        /**
         * \brief a-parameter
         */
        unsigned int a = 0;

        /**
         * \brief Values of the direction
         */
        std::vector<unsigned int> values;

        SobolDirection() = default;

        SobolDirection(const unsigned int d, const unsigned int s, const unsigned int a, const std::vector<unsigned int>& v)
            : index(d), size(s), a(a), values(v) {
        }
    };
}

