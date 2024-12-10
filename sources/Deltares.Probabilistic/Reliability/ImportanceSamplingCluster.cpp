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
#include "ImportanceSamplingCluster.h"
#include "../Statistics/StandardNormal.h"
#include "../Math/NumericSupport.h"

namespace Deltares
{
    namespace Reliability
    {
        void ImportanceSamplingCluster::initialize(int nStochasts, double z0Fac, bool z0Ignore, DesignPointMethod method, std::shared_ptr<StochastSettingsSet> stochastSet)
        {
            this->z0Fac = z0Fac;
            this->z0Ignore = z0Ignore;

            DesignPointBuilder = std::make_shared<Reliability::DesignPointBuilder>(nStochasts, method, stochastSet);

            DesignPointBuilder->initialize(z0Fac * Statistics::StandardNormal::BetaMax);
        }

        void ImportanceSamplingCluster::addSample(std::shared_ptr<Models::Sample> sample)
        {
            TotalCount++;
            TotalWeight += sample->Weight;

            if (sample->Z < 0)
            {
                FailCount++;
                FailWeight += sample->Weight;
                MaxFailWeight = std::max(MaxFailWeight, sample->Weight);
                DesignPointBuilder->addSample(sample);
            }

            if (this->NearestSample == nullptr || std::abs(sample->Z) < std::abs(this->NearestSample->Z))
            {
                this->NearestSample = sample;
            }

            // calculate the probability per cluster
            this->FailFraction = Numeric::NumericSupport::Divide(FailCount, TotalCount);
            this->ProbFailure = getProbabilityOfFailure(!z0Ignore);
        }

        double ImportanceSamplingCluster::getProbabilityOfFailure(bool useCount)
        {
            if (this->FailCount == 0)
            {
                return 0;
            }
            else if (this->FailCount == this->TotalCount)
            {
                return 1;
            }
            else if (useCount)
            {
                return getProbability(this->z0Fac, this->FailWeight, this->TotalCount);
            }
            else
            {
                return getProbability(this->z0Fac, this->FailWeight, this->TotalWeight);
            }
        }

        double ImportanceSamplingCluster::getProbability(double z0Fac, double failWeight, double totalWeight)
        {
            if (z0Fac == -1)
            {
                return 1 - failWeight / totalWeight;
            }
            else
            {
                return failWeight / totalWeight;
            }
        }
    }
}
