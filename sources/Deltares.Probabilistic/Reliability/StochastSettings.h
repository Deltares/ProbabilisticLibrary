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

#include "../Statistics/StandardNormal.h"
#include "../Statistics/Stochast.h"

namespace Deltares
{
    namespace Reliability
    {
        class StochastSettings
        {
        public:
            double MinValue = -Statistics::StandardNormal::UMax;
            double MaxValue = Statistics::StandardNormal::UMax;
            double StartValue = 0;
            double UncorrelatedStartValue = 0;

            double VarianceFactor = 1.5;
            bool IsVarianceAllowed = true;

            int Intervals = 200;
            bool IsInitializationAllowed = true;

            int StochastIndex = 0;

            bool isMinMaxDefault();
            void initializeForRun();

            // Converted min and max values, is updated by initializeForRun
            double XMinValue = 0;
            double XMaxValue = 0;

            bool IsQualitative = false;

            std::shared_ptr<Statistics::Stochast> stochast = nullptr;

            double getRepresentativeU(double u);

            std::shared_ptr<StochastSettings> clone();
        };
    }
}

