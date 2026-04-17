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
#pragma once
#include "ImportanceSamplingSettingsS.h"
#include "UncertaintyMethod.h"
#include "../Reliability/DesignPointBuilder.h"

namespace Deltares::Uncertainty
{
    /**
     * \brief Calculates the sensitivity using the Importance Sampling algorithm
     * \remark This algorithm focuses on the tail of the sensitivity, but only works well when the start point (in the settings) is specified well by the user
     */
    class ImportanceSamplingS : public UncertaintyMethod
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
        UncertaintyResult getUncertaintyStochast(std::shared_ptr<Models::ModelRunner> modelRunner) override;
    private:
        static std::vector<double> getFactors(const Reliability::StochastSettingsSet& stochastSettings);
        std::shared_ptr<Models::Sample> getModifiedSample(const Models::Sample& sample, const Models::Sample& center) const;
        void updateCumulativeWeights(const std::vector<double>& zValues, const std::vector<double>& weights,
            std::vector<double>& cumulativeWeights, const Models::Sample& sample) const;
        static void adjustWeights(std::vector<double>& weights, const double weight_difference);
        void registerSample(const Models::ModelRunner& modelRunner, const std::shared_ptr<Models::Sample>& sample) const;
        void registerWeights(const std::vector<double>& weights) const;
        void registerSamples(const std::shared_ptr<Statistics::Stochast>& stochast, const std::vector<double>& zValues) const;
        bool getConverged(int sampleIndex, const Models::Sample& center, int nSamples);
        Reliability::DesignPointBuilder designPointBuilder;
        double dimensionality = 0.0;
        std::vector<double> factors;
    };
}
