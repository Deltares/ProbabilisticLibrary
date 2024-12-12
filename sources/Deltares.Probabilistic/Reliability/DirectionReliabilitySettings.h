// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of the Probabilistic Library.
//
// The Probabilistic Library is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// All names, logos, and references to "Deltares" are registered trademarks of
// Stichting Deltares and remain full property of Stichting Deltares at all times.
// All rights reserved.
//
#pragma once

#include "StochastSettingsSet.h"
#include "../Model/Sample.h"

namespace Deltares
{
    namespace Reliability
    {
        enum ModelVaryingType {Monotone, Varying};

        class DirectionReliabilitySettings
        {
        public:
            ModelVaryingType modelVaryingType = ModelVaryingType::Monotone;
            double MaximumLengthU = 20;
            double Dsdu = 1;
            double EpsilonUStepSize = 0.01;
            int MaximumIterations = 50;
            double EpsilonZStepSize = 0.01;
            bool FindMinimalValue = false;
            bool UseInitialValues = false;

            std::shared_ptr<StochastSettingsSet> StochastSet = std::make_shared<StochastSettingsSet>();

            bool IsValid();
        };
    }
}

