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
#include "SingleVariation.h"

#include <memory>

#include "SensitivityValue.h"
#include "../Model/Sample.h"
#include "../Statistics/Stochast.h"

namespace Deltares::Sensitivity
{
    using namespace Deltares::Models;
    using namespace Deltares::Statistics;

    SensitivityResult SingleVariation::getSensitivityResult(std::shared_ptr<ModelRunner> modelRunner)
    {
        int nStochasts = modelRunner->getVaryingStochastCount();

        double uLow = StandardNormal::getUFromP(Settings->LowValue);
        double uHigh = StandardNormal::getUFromP(Settings->HighValue);

        std::vector<std::shared_ptr<Sample>> samples;

        samples.push_back(std::make_shared<Sample>(nStochasts));

        for (int i = 0; i < nStochasts; i++)
        {
            std::shared_ptr<Sample> lowSample = std::make_shared<Sample>(nStochasts);
            lowSample->Values[i] = uLow;
            samples.push_back(lowSample);

            std::shared_ptr<Sample> highSample = std::make_shared<Sample>(nStochasts);
            highSample->Values[i] = uHigh;
            samples.push_back(highSample);
        }

        std::vector<double> z = modelRunner->getZValues(samples);

        SensitivityResult result = modelRunner->getSensitivityResult();

        for (int i = 0; i < nStochasts; i++)
        {
            std::shared_ptr<SensitivityValue> value = result.values[i];
            value->medium = z[0];
            value->low = z[2 * i + 1];
            value->high = z[2 * i + 2];
        }

        return result;
    }
}
