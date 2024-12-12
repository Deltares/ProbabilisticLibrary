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

#include "../../Deltares.Probabilistic/Statistics/StandardNormal.h"

namespace Deltares
{
    namespace Statistics
    {
        namespace Wrappers
        {
            public ref class StandardNormal
            {
            public:
                static double GetUFromP(double p) { return Statistics::StandardNormal::getUFromP(p); }
                static double GetUFromQ(double q) { return Statistics::StandardNormal::getUFromQ(q); }
                static double GetPFromU(double u) { return Statistics::StandardNormal::getPFromU(u); }
                static double GetQFromU(double u) { return Statistics::StandardNormal::getQFromU(u); }
                static double GetRFromP(double p) { return Statistics::StandardNormal::getTFromP(p); }
                static double GetPFromR(double r) { return Statistics::StandardNormal::getPFromT(r); }

                static property double UMax
                {
                    double get() { return Statistics::StandardNormal::UMax; }
                }

                static property double BetaMax
                {
                    double get() { return Statistics::StandardNormal::BetaMax; }
                }

                static bool IsBetaMin(double beta)
                {
                    return beta <= -BetaMax;
                }

                static bool IsBetaMax(double beta)
                {
                    return beta >= BetaMax;
                }
            };
        }
    }
}

