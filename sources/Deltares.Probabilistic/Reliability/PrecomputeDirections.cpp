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
        const std::vector<DirectionReliabilityDS>& directions, std::vector<bool>& shouldCompute)
    {
        const size_t nSamples = directions.size();

        // copy z-value zero sample
        const auto z0pv = PrecomputedDirectionValue(0.0, std::abs(z0), false, true);
        const double z0Fac = ReliabilityMethod::getZFactor(z0);
        for (const auto& direction : directions)
        {
            direction.ProvidePrecomputeValue(z0pv);
        }

        // precompute Z-values multiples of Dsdu
        const int sectionsCount = settings.SectionCount();
        const auto model = ZGetter(modelRunner, settings);

        // store intermediate values
        std::vector<double> uValues(nSamples);
        std::vector<std::shared_ptr<Sample>> uDirections(nSamples);
        for (size_t i = 0; i < nSamples; i++)
        {
            if (shouldCompute[i])
            {
                uDirections[i] = directions[i].getDirection().getNormalizedSample();
            }
        }

        for (int k = 1; k <= sectionsCount; k++)
        {
            //
            // collect samples
            //
            std::vector<std::shared_ptr<Sample>> uSamples;
            for (size_t i = 0; i < nSamples; i++)
            {
                if (shouldCompute[i])
                {
                    uValues[i] = directions[i].GetPrecomputeUvalue();
                    uSamples.push_back(model.GetSample(*uDirections[i], uValues[i]));
                }
            }

            //
            // get z-values for all samples in parallel
            //
            auto zValues = modelRunner.getZValues(uSamples);
            Counter += zValues.size();

            //
            // process z-values
            //
            int ii = 0;
            for (size_t i = 0; i < nSamples; i++)
            {
                if (shouldCompute[i])
                {
                    directions[i].getDirection().IsRestartRequired = uSamples[ii]->IsRestartRequired;
                    directions[i].getDirection().AllowProxy = uSamples[ii]->AllowProxy;
                    directions[i].getDirection().Z = uSamples[ii]->Z;
                    auto z1pv = PrecomputedDirectionValue(uValues[i], z0Fac * zValues[ii],
                        uSamples[ii]->IsRestartRequired, uSamples[ii]->AllowProxy);
                    directions[i].ProvidePrecomputeValue(z1pv);
                    shouldCompute[i] = directions[i].CanPrecomputeSample();
                    ii++;
                }
            }

            //
            // early return: not enough samples left to benefit from parallelization
            //
            if (uSamples.size() <= 1) break;
        }
    }

}
