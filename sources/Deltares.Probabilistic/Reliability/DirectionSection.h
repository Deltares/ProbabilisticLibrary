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
#include "../Math/NumericSupport.h"
#include "../Statistics/StandardNormal.h"

namespace Deltares::Reliability
{
    class DirectionSection
    {
    public:
        DirectionSection(Numeric::DoubleType type, double uLow, double uHigh) :
            Type(type), ULow(uLow), UHigh(uHigh) {}

        DirectionSection(Numeric::DoubleType type, double uLow, double uHigh, double zLow, double zHigh) :
            Type(type), ULow(uLow), UHigh(uHigh), ZLow(zLow), ZHigh(zHigh) {}

        const Numeric::DoubleType Type;
        const double ULow;
        const double UHigh;
        const double ZLow = 0.0;
        const double ZHigh = 0.0;

        double getProbability() const
        {
            const double pHigh = Statistics::StandardNormal::getQFromU(UHigh);
            const double pLow = Statistics::StandardNormal::getQFromU(ULow);
            return pLow - pHigh;
        }
    };
}

