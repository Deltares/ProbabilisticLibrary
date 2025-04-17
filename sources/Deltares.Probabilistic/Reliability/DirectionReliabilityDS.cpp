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
#include "DirectionReliabilityDS.h"
#include "DirectionSectionsCalculationDS.h"

namespace Deltares::Reliability
{
    double DirectionReliabilityDS::getBeta(Models::ModelRunner& modelRunner, double z0, const PrecomputeValues& zValues)
    {
        auto normalizedSample = directionSample.getNormalizedSample();

        auto task = BetaValueTask(normalizedSample, z0 < 0.0);

        double beta = getDirectionBeta(modelRunner, task, zValues);
        beta *= z0;

        directionSample.AllowProxy = task.UValues->AllowProxy;

        return beta;
    }

    double DirectionReliabilityDS::getDirectionBeta(Models::ModelRunner& modelRunner,
        const BetaValueTask& directionTask, const PrecomputeValues& zValues)
    {
        if (modelRunner.canCalculateBeta())
        {
            return modelRunner.getBeta(directionTask.UValues);
        }
        else
        {
            auto sections = sectionsCalc.getDirectionSections(modelRunner, directionTask, zValues);

            double beta = DirectionSectionsCalculation::getBetaFromSections(sections, Settings.FindMinimalValue);

            directionTask.UValues->AllowProxy = directionTask.UValues->AllowProxy;

            return beta;
        }
    }

    bool DirectionReliabilityDS::CanPrecomputeSample() const
    {
        return sectionsCalc.CanPrecomputeSample();
    }

    double DirectionReliabilityDS::GetPrecomputeUvalue() const
    {
        return sectionsCalc.GetPrecomputeUvalue();
    }

    void DirectionReliabilityDS::ProvidePrecomputeValue(const PrecomputeValue& zValue)
    {
        sectionsCalc.ProvidePrecomputeValue(zValue);
    }

}

