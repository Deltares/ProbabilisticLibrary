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
#include "RandomValueGenerator.h"

namespace Deltares::Numeric
{
    template <typename T>
    class ShuffleItems
    {
    public:
        static void ShuffleInPlace(std::vector<T>& list, RandomValueGenerator& rng)
        {
            const int size = static_cast<int>(list.size());
            for (int i = 0; i < size - 2; ++i)
            {
                const int j = i + rng.next(size-i);
                std::swap(list[i], list[j]);
            }
        }

        static std::vector<T> GetNShuffledItems(const std::vector<T>& list, RandomValueGenerator& rng, const int count)
        {
            const int size = static_cast<int>(list.size());
            auto result = list;
            for (int i = 0; i < count; ++i)
            {
                const int j = i + rng.next(size - i);
                std::swap(result[i], result[j]);
            }
            result.resize(count);
            return result;
        }
    };
}
