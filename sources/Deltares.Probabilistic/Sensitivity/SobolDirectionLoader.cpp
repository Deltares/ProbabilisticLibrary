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
#include "SobolDirectionLoader.h"
#include "SobolResource.h"
#include <cmath>

namespace Deltares::Sensitivity
{
    std::vector<SobolDirection> SobolDirectionLoader::getDirections(int size)
    {
        std::vector<SobolDirection> directions;

        directions.push_back(getFirstDirection());

        for (int i = 0; i < size; i++)
        {
            directions.push_back(getDirection(i));
        }

        return directions;
    }

    SobolDirection SobolDirectionLoader::getFirstDirection()
    {
        std::vector<unsigned int> values(maxLength + 1);
        values[0] = static_cast<unsigned int>(std::pow(2, scale));
        for (int i = 1; i <= maxLength; i++)
        {
            // for the first dimension every m_i = 1;
            values[i] = static_cast<unsigned int>(1 << (scale - i));
        }
        SobolDirection direction = SobolDirection(1, 0, 0, values);
        return direction;
    }

    SobolDirection SobolDirectionLoader::getDirection(int index)
    {
        std::vector<unsigned int> values = SobolResource::getSobolDirection(index);

        unsigned int s = values[1];
        unsigned int a = values[2];

        std::vector<unsigned int> m_i = std::vector<unsigned int>(values.size() - 3);
        for (size_t i = 0; i < m_i.size(); i++)
        {
            m_i[i] = values[i + 3];
        }

        std::vector<unsigned int> v(maxLength + 1);

        if (maxLength <= s)
        {
            for (unsigned int i = 1; i <= maxLength; i++)
            {
                v[i] = m_i[i] << (scale - i);
            }
        }
        else
        {
            for (unsigned int i = 1; i <= s; i++)
            {
                v[i] = m_i[i - 1] << (scale - i);
            }

            for (unsigned int i = s + 1; i <= maxLength; i++)
            {
                v[i] = v[i - s] ^ (v[i - s] >> s);
                for (unsigned int k = 1; k <= s - 1; k++)
                {
                    constexpr unsigned int maxShift = sizeof(unsigned int);
                    unsigned int shift = std::min(s - 1 - k, maxShift);
                    v[i] ^= ((a >> shift) & 1) * v[i - k];
                }
            }
        }

        SobolDirection direction = SobolDirection(index + 1, s, a, v);
        return direction;
    }
};

