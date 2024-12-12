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
#pragma once
#include "ImportanceSamplingSettingsS.h"
#include "SensitivityMethod.h"

namespace Deltares
{
    namespace Sensitivity
    {
        /**
         * \brief Calculates the sensitivity using the Importance Sampling algorithm
         * \remark This algorithm focuses on the tail of the sensitivity, but only works well when the start point (in the settings) is specified well by the user
         */
        class ImportanceSamplingS : public SensitivityMethod
        {
        public:
            /**
             * \brief Settings for this algorithm
             */
            std::shared_ptr<ImportanceSamplingSettingsS> Settings = std::make_shared<ImportanceSamplingSettingsS>();

            /**
             * \brief Gets the sensitivity
             * \param modelRunner The model for which the sensitivity is calculated
             * \return The sensitivity in the form of a stochastic variable
             */
            std::shared_ptr<Statistics::Stochast> getSensitivityStochast(std::shared_ptr<Models::ModelRunner> modelRunner) override;
        private:
            double getDimensionality(std::vector<double> factors);
            double getWeight(std::shared_ptr<Models::Sample> modifiedSample, std::shared_ptr<Models::Sample> sample, double dimensionality);
            std::vector<double> getFactors(std::shared_ptr<Reliability::StochastSettingsSet> stochastSettings);
            double getConvergence(int samples, double weightedSum);
        };
    }
}

