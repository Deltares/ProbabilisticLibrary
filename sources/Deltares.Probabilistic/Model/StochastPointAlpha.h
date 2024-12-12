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

#include "../Statistics/Stochast.h"
#include <memory>

namespace Deltares
{
    namespace Models
    {
        class StochastPointAlpha
        {
        public:
            std::shared_ptr<Deltares::Statistics::Stochast> Stochast = nullptr;
            int Index = 0;
            double Alpha = 0;
            double AlphaCorrelated = 0;
            double U = 0;
            double X = 0;
            double InfluenceFactor = 0;

            std::shared_ptr<StochastPointAlpha> clone();
            void invert();

            std::string getIdentifier();
        };
    }
}

