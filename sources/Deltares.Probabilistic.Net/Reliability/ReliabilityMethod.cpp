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
#include "ReliabilityMethod.h"

#include "DesignPoint.h"

namespace Deltares
{
    namespace Reliability
    {
        namespace Wrappers
        {
            DesignPoint^ ReliabilityMethod::GetDesignPoint(Models::Wrappers::ModelRunner^ modelRunner)
            {
                const std::shared_ptr<Reliability::ReliabilityMethod> reliabilityMethod = this->GetReliabilityMethod();

                const std::shared_ptr<Models::ModelRunner> nativeModelRunner = modelRunner->GetModelRunner();

                nativeModelRunner->initializeForRun();

                const std::shared_ptr<Reliability::DesignPoint> nativeDesignPoint = reliabilityMethod->getDesignPoint(nativeModelRunner);

                Wrappers::DesignPoint^ designPoint = gcnew Wrappers::DesignPoint(nativeDesignPoint, modelRunner->Stochasts);

                return designPoint;
            };
        }
    }
}


