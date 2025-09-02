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
#include <memory>
#include <string>
#include <vector>

namespace Deltares::Sensitivity
{
    /**
     * \brief Contains a sobol direction
     */
    class SobolDirection {
    public:
        int d = 0;
        uint32_t s = 0;
        uint32_t a = 0;
        std::vector<uint32_t> v;

        SobolDirection() = default;

        SobolDirection(int d, uint32_t s, uint32_t a, const std::vector<uint32_t>& v)
            : d(d), s(s), a(a), v(v) {
        }

        int D() const
        {
            return this->d;
        }

        uint32_t S() const
        {
            return this->s;
        }

        uint32_t A() const
        {
            return this->a;
        }

        const std::vector<uint32_t>& V() const
        {
            return this->v;
        }
    };

    /// <summary>
    /// Loads source data for a <see cref="Direction"/> from internal storage
    /// </summary>
    class SobolDirectionLoader
    {
        /// <summary>
        /// Gets a specified number of <see cref="Direction"/>s
        /// </summary>
        /// <param name="size"></param>
        /// <returns></returns>
    public:
        static std::vector<SobolDirection> getDirections(int size);

    private:
        static SobolDirection getFirstDirection();
        static SobolDirection getDirection(int index);
    };
}

