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

#include "PrecomputeDirections.h"
#include "ZGetter.h"

namespace Deltares::Reliability
{
    // precompute Z-values
    std::vector<PrecomputeValues> PrecomputeDirections::precompute(const std::shared_ptr<Models::ModelRunner>& modelRunner,
        const std::vector<std::shared_ptr<Sample>>& samples, const double z0,
        const DirectionReliabilityForDirectionalSampling& directionReliability, std::vector<bool>& mask)
    {
        const size_t nSamples = samples.size();

        // copy z-value zero sample
        auto precomputed = PrecomputeValues();
        const auto z0pv = PrecomputeValue(0.0, std::abs(z0));
        precomputed.values.emplace_back(z0pv);
        auto zValues = std::vector(nSamples, precomputed);
        const double z0Fac = ReliabilityMethod::getZFactor(z0);

        // precompute Z-values multiples of Dsdu
        const auto settings = directionReliability.Settings;
        const int sectionsCount = static_cast<int>(settings->MaximumLengthU / settings->Dsdu);
        const auto model = ZGetter(modelRunner, settings);
        for (int k = 1; k < sectionsCount; k++)
        {
            const auto u1 = k * settings->Dsdu;
            std::vector<std::shared_ptr<Sample>> uSamples;
            for (size_t i = 0; i < nSamples; i++)
            {
                if (!mask[i])
                {
                    auto uDirection = samples[i]->getNormalizedSample();
                    auto z1 = model.GetU(uDirection, u1);
                    uSamples.push_back(z1);
                }
            }
            auto zValues2 = modelRunner->getZValues(uSamples);
            int ii = 0;
            for (size_t i = 0; i < nSamples; i++)
            {
                if (!mask[i])
                {
                    auto z1pv = PrecomputeValue(u1, z0Fac * zValues2[ii]);
                    zValues[i].values.push_back(z1pv);
                    if (z0 * zValues2[ii] < 0.0)
                    {
                        mask[i] = true;
                    }
                    else if (settings->modelVaryingType == ModelVaryingType::Monotone)
                    {
                        if (std::abs(zValues2[ii]) > std::abs(z0))
                        {
                            mask[i] = true;
                        }
                    }
                    ii++;
                }
            }
            if (uSamples.size() <= 1) break;
        }
        return zValues;
    }

}
