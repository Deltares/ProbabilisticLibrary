// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of Streams.
//
// Streams is free software: you can redistribute it and/or modify
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
#include "DirectionReliabilitySettings.h"

namespace Deltares
{
    namespace Reliability
    {
        bool DirectionReliabilitySettings::IsValid()
        {
            return this->Dsdu >= 0.01 && EpsilonUStepSize >= 0.00001 && EpsilonZStepSize >= 0.00001 && this->MaximumIterations >= 1 && this->MaximumLengthU >= 1;
        }
    }
}

