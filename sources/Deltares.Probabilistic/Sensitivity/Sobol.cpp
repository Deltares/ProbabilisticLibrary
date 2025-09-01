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
#include "Sobol.h"

#include <algorithm>
#include <memory>

#include "../Model/Sample.h"
#include "../Statistics/Stochast.h"

namespace Deltares::Sensitivity
{
    using namespace Deltares::Models;
    using namespace Deltares::Statistics;

    SensitivityResult Sobol::getSensitivityStochast(std::shared_ptr<ModelRunner> modelRunner)
    {
        int nStochasts = modelRunner->getVaryingStochastCount();

        auto result = Sensitivity::SensitivityResult();


        return result;
    }
}
