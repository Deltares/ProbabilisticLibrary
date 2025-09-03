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
     * \brief Contains predefined values, which will be used to generate Sobol sequences
     * \remark Data taken from https://github.com/joe-kuo/sobol_data/
     */
    class SobolResource
    {
    public:
        /**
         * \brief Gets the predefined values by index
         * \param index Index
         * \return Predefined values
         */
        static std::vector<uint32_t> getSobolDirection(int index)
        {
            if (!initialized)
            {
                initialized = true;
                initialize();
            }

            return sobolValues[index];
        }

    private:
        static void initialize();
        static inline std::vector<std::vector<uint32_t>> sobolValues;
        static inline bool initialized = false;
    };
}

