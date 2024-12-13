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
#include "ReliabilityResult.h"
#include "../Math/NumericSupport.h"

namespace Deltares
{
    namespace Reliability
    {
        bool ReliabilityResult::IsMeaningful(std::shared_ptr<ReliabilityResult> resultBefore, std::shared_ptr<ReliabilityResult> resultAfter)
        {
            if (resultBefore == nullptr || resultAfter == nullptr)
            {
                return true;
            }

            return !Numeric::NumericSupport::areEqual(Reliability, resultBefore->Reliability, 0.001) ||
                !Numeric::NumericSupport::areEqual(Reliability, resultAfter->Reliability, 0.001);
        }
    }
}
