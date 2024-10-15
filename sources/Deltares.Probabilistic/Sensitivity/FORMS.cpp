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
#include "FORMS.h"
#include <vector>
#include <cmath>
#include <memory>

#include "../Math/NumericSupport.h"
#include "../Model/Sample.h"
#include "../Model/GradientCalculator.h"
#include "../Statistics/DistributionType.h"

using namespace Deltares::Models;

namespace Deltares
{
    namespace Sensitivity
    {
        std::shared_ptr<Statistics::Stochast> FORMS::getStochast(std::shared_ptr<Models::ModelRunner> modelRunner)
        {
            int nStochasts = modelRunner->getVaryingStochastCount();

            std::shared_ptr<Models::GradientCalculator> gradientCalculator = std::make_shared<GradientCalculator>();
            gradientCalculator->Settings = std::make_shared<GradientSettings>();
            gradientCalculator->Settings->StepSize = Settings->StepSize;
            gradientCalculator->Settings->gradientType = GradientType::OneDirection;

            std::shared_ptr<Sample> zeroSample = std::make_shared<Sample>(nStochasts);

            double z0 = modelRunner->getZValue(zeroSample);

            return nullptr;
        }
    }
}



