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

#include "PrecomputeValues.h"
#include "ReliabilityMethod.h"
#include <cmath>

namespace Deltares::Reliability
{
    std::pair<bool, PrecomputeValue>  PrecomputeValues::findZ(const size_t index) const
    {
        if (index < values.size())
        {
            return { true, values[index] };
        }
        else
        {
            auto result = PrecomputeValue(std::nan(""), std::nan(""), false, false);
            return { false, result};
        }

    }

};


