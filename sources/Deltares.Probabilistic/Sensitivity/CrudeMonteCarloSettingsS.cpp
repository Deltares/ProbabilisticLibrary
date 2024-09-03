// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of the Probabilistic Library.
//
// The Probabilistic Library is free software: you can redistribute it and/or modify
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
#include "CrudeMonteCarloSettingsS.h"

namespace Deltares
{
    namespace Sensitivity
    {
        /**
         * \brief Gets the number of runs which is needed to achieve the variation coefficient at the probability for convergence
         */
        int CrudeMonteCarloSettingsS::getRequiredSamples()
        {
            double probability = this->ProbabilityForConvergence;
            double variationCoefficient = this->VariationCoefficient;

            if (probability > 0.5)
            {
                probability = 1 - probability;
            }

            double samples = (1 - probability) / (variationCoefficient * variationCoefficient * probability);

            return static_cast<int>(std::ceil(samples));
        }

        /**
         * \brief Modifies the variation coefficient so that the number of required samples matches a given value
         */
        void CrudeMonteCarloSettingsS::setRequiredSamples(int samples)
        {
            double probability = this->ProbabilityForConvergence;
            if (probability > 0 && probability < 1)
            {
                if (probability > 0.5)
                {
                    probability = 1 - probability;
                }
                double varPf = sqrt((1 - probability) / (samples * probability));
                this->VariationCoefficient = varPf;
            }
        }
    }
}


