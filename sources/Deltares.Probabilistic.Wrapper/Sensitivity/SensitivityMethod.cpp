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
#include "SensitivityMethod.h"

#include "../Model/ModelRunner.h"
#include "../Statistics/Stochast.h"

namespace Deltares
{
    namespace Sensitivity
    {
        namespace Wrappers
        {
            Statistics::Wrappers::Stochast^ SensitivityMethod::GetStochast(Models::Wrappers::ModelRunner^ modelRunner)
            {
                const std::shared_ptr<Sensitivity::SensitivityMethod> sensitivityMethod = this->GetNativeSensitivityMethod();

                const std::shared_ptr<Models::ModelRunner> nativeModelRunner = modelRunner->GetModelRunner();

                nativeModelRunner->initializeForRun();

                const std::shared_ptr<Statistics::Stochast> nativeStochast = sensitivityMethod->getStochast(nativeModelRunner);

                Statistics::Wrappers::Stochast^ stochast = gcnew Statistics::Wrappers::Stochast(nativeStochast);

                return stochast;
            };
        }
    }
}


