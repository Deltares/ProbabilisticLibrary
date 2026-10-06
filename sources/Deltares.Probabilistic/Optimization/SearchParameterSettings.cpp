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
#include "SearchParameterSettings.h"
#include "../Utils/ProbabilisticLibraryException.h"

namespace Deltares::Optimization
{
    std::vector<double> SearchParameterSettings::getValues() const
    {
        std::vector<double> values(UseValues == UseValuesType::AllValues ? NumberOfValues : 1);

        if (UseValues == UseValuesType::AllValues)
        {
            const double interval = getInterval();

            for (size_t i = 0; i < values.size(); i++)
            {
                values[i] = MinValue + static_cast<double>(i) * interval;
            }
        }
        else if (UseValues == UseValuesType::MinValue)
        {
            values[0] = MinValue;
        }
        else if (UseValues == UseValuesType::MaxValue)
        {
            values[0] = MaxValue;
        }
        else
        {
            throw Reliability::ProbabilisticLibraryException("Use values type not supported");
        }

        return values;
    }


    double SearchParameterSettings::GetAbsoluteValue(double ratio) const
    {
        const double diff = MaxValue - MinValue;
        return MinValue + ratio * diff;
    }

    double SearchParameterSettings::GetRelativeValue(double value) const
    {
        return (value - MinValue) / (MaxValue - MinValue);
    }

}

