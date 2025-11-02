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
#include "DeterministicDistribution.h"
#include "../StochastProperties.h"
#include "../../Math/NumericSupport.h"

namespace Deltares
{
    namespace Statistics
    {
        bool DeterministicDistribution::isVarying(StochastProperties& stochast)
        {
            return false;
        }

        void DeterministicDistribution::setMeanAndDeviation(StochastProperties& stochast, double mean, double deviation)
        {
            stochast.Location = mean;
            stochast.Scale = deviation;
        }

        void DeterministicDistribution::initialize(StochastProperties& stochast, const std::vector<double>& values)
        {
            stochast.Location = values[0];
        }

        double DeterministicDistribution::getMean(StochastProperties& stochast)
        {
            return stochast.Location;
        }

        double DeterministicDistribution::getDeviation(StochastProperties& stochast)
        {
            return 0.0;
        }

        double DeterministicDistribution::getXFromU(StochastProperties& stochast, double u)
        {
            return stochast.Location;
        }

        double DeterministicDistribution::getPDF(StochastProperties& stochast, double x)
        {
            return x == stochast.Location ? 1.0 : 0.0;
        }

        double DeterministicDistribution::getCDF(StochastProperties& stochast, double x)
        {
            if (x < stochast.Location)
            {
                return 0.0;
            }
            else if (x > stochast.Location)
            {
                return 1.0;
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

        std::vector<double> DeterministicDistribution::getDiscontinuityPoints(const StochastProperties& stochast)
        {
            return { stochast.Location };
        }

        std::vector<double> DeterministicDistribution::getSpecialPoints(StochastProperties& stochast)
        {
            constexpr double offset = 0.000001;
            constexpr double bigOffset = 10;

            return {
                stochast.Location - bigOffset,
                stochast.Location - offset,
                stochast.Location,
                stochast.Location + offset,
                stochast.Location + bigOffset
            };
        }

        void DeterministicDistribution::fit(std::shared_ptr<StochastProperties> stochast, std::vector<double>& values, const double shift)
        {
            stochast->Location = Numeric::NumericSupport::getMean(values);
            stochast->Observations = static_cast<int>(values.size());
        }
    }
}



