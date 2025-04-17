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

#include "ReliabilityMethod.h"
#include "ZGetter.h"

namespace Deltares::Reliability
{
    PrecomputeDirections::PrecomputeDirections(const DirectionReliabilitySettings& settings, const double z0) :
        settings(settings), z0(z0) {}

    // precompute Z-values
    void PrecomputeDirections::precompute(Models::ModelRunner& modelRunner,
        std::vector<DirectionReliabilityDS>& directions, std::vector<bool>& mask)
    {
        const size_t nSamples = directions.size();

        // copy z-value zero sample
        auto precomputed = PrecomputeValues();
        const auto z0pv = PrecomputeValue(0.0, std::abs(z0), false, true);
        precomputed.values.emplace_back(z0pv);
        const double z0Fac = ReliabilityMethod::getZFactor(z0);

        if (modelRunner.canCalculateBeta()) return;

        // precompute Z-values multiples of Dsdu
        const int sectionsCount = settings.SectionCount();
        const auto model = ZGetter(modelRunner, settings);
        for (int k = 1; k < sectionsCount; k++)
        {
            
            std::vector<std::shared_ptr<Sample>> uSamples;
            for (size_t i = 0; i < nSamples; i++)
            {
                if (k == 1) directions[i].ProvidePrecomputeValue(z0pv);
                if (!mask[i])
                {
                    const auto uk = directions[i].GetPrecomputeUvalue();
                    auto uDirection = directions[i].directionSample.getNormalizedSample();
                    auto sample_at_uk = model.GetU(*uDirection, uk);
                    uSamples.push_back(sample_at_uk);
                }
            }
            auto zValues2 = modelRunner.getZValues(uSamples);
            Counter += zValues2.size();
            int ii = 0;
            for (size_t i = 0; i < nSamples; i++)
            {
                if (!mask[i])
                {
                    const auto uk = directions[i].GetPrecomputeUvalue();
                    directions[i].directionSample.IsRestartRequired = uSamples[ii]->IsRestartRequired;
                    directions[i].directionSample.AllowProxy = uSamples[ii]->AllowProxy;
                    directions[i].directionSample.Z = uSamples[ii]->Z;
                    auto z1pv = PrecomputeValue(uk, z0Fac * zValues2[ii],
                        uSamples[ii]->IsRestartRequired, uSamples[ii]->AllowProxy);
                    directions[i].ProvidePrecomputeValue(z1pv);
                    mask[i] = directions[i].CanPrecomputeSample();
                    ii++;
                }
            }
            if (uSamples.size() <= 1) break;
        }
    }

}
