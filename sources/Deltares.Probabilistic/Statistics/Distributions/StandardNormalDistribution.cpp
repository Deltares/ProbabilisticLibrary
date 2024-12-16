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
#include "StandardNormalDistribution.h"

#include "../StochastProperties.h"
#include "../StandardNormal.h"

namespace Deltares
{
    namespace Statistics
    {
        bool StandardNormalDistribution::isVarying(std::shared_ptr<StochastProperties> stochast)
        {
            return true;
        }

        double StandardNormalDistribution::getMean(std::shared_ptr<StochastProperties> stochast)
        {
            return standardNormalMean;
        }

        double StandardNormalDistribution::getDeviation(std::shared_ptr<StochastProperties> stochast)
        {
            return standardNormalDeviation;
        }

        double StandardNormalDistribution::getXFromU(std::shared_ptr<StochastProperties> stochast, double u)
        {
            return u;
        }

        double StandardNormalDistribution::getUFromX(std::shared_ptr<StochastProperties> stochast, double x)
        {
            return x;
        }

        double StandardNormalDistribution::getPDF(std::shared_ptr<StochastProperties> stochast, double x)
        {
            double distance = -x * x / 2;
            return normalFactor * exp(distance);
        }

        double StandardNormalDistribution::getCDF(std::shared_ptr<StochastProperties> stochast, double x)
        {
            return StandardNormal::getPFromU(x);
        }
    }
}




