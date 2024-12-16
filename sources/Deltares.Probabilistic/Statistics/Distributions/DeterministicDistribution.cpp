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
#include "DeterministicDistribution.h"
#include "../StochastProperties.h"
#include "../../Math/NumericSupport.h"

namespace Deltares
{
    namespace Statistics
    {
        bool DeterministicDistribution::isVarying(std::shared_ptr<StochastProperties> stochast)
        {
            return false;
        }

        void DeterministicDistribution::setMeanAndDeviation(std::shared_ptr<StochastProperties> stochast, double mean, double deviation)
        {
            stochast->Location = mean;
            stochast->Scale = deviation;
        }

        void DeterministicDistribution::initialize(std::shared_ptr<StochastProperties> stochast, std::vector<double> values)
        {
            stochast->Location = values[0];
        }

        double DeterministicDistribution::getMean(std::shared_ptr<StochastProperties> stochast)
        {
            return stochast->Location;
        }

        double DeterministicDistribution::getDeviation(std::shared_ptr<StochastProperties> stochast)
        {
            return 0;
        }

        double DeterministicDistribution::getXFromU(std::shared_ptr<StochastProperties> stochast, double u)
        {
            return stochast->Location;
        }

        double DeterministicDistribution::getPDF(std::shared_ptr<StochastProperties> stochast, double x)
        {
            return x == stochast->Location ? 1 : 0;
        }

        double DeterministicDistribution::getCDF(std::shared_ptr<StochastProperties> stochast, double x)
        {
            if (x < stochast->Location)
            {
                return 0;
            }
            else if (x > stochast->Location)
            {
                return 1;
            }
            else
            {
                return 0.5;
            }
        }

        void DeterministicDistribution::setXAtU(std::shared_ptr<StochastProperties> stochast, double x, double u, ConstantParameterType constantType)
        {
            stochast->Location = x;
        }

        void DeterministicDistribution::fit(std::shared_ptr<StochastProperties> stochast, std::vector<double>& values)
        {
            stochast->Location = Numeric::NumericSupport::getMean(values);
        }
    }
}



