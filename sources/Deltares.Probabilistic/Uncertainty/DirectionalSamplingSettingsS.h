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

#include "../Model/RunSettings.h"
#include "../Model/RandomSettings.h"
#include <memory>

namespace Deltares::Uncertainty
{
    enum class ModelType { Plane, Sphere };

    class DirectionalSamplingSettingsS
    {
    public:
        /**
         * \brief Indicates whether correlations should be calculated
         */
        bool CalculateCorrelations = false;

        /**
         * \brief Indicates whether correlations with input values should be calculated
         */
        bool CalculateInputCorrelations = false;

        /**
         * \brief The number of iterations within a direction
         */
        int MaximumIterations = 50;

        /**
         * \brief The number of directions to be calculated
         */
        int NumberDirections = 1000;

        /**
         * \brief Convergence criterion
         */
        double VariationCoefficientFailure = 0.05;

        /**
         * \brief Indicates whether the number of samples should be derived from the variation coefficient at the probability for convergence
         */
        bool DeriveSamplesFromVariationCoefficient = false;

        /**
         * \brief Gets the number of runs which is needed to achieve the variation coefficient at the probability for convergence
         */
        static int getRequiredSamples(double probability, double variationCoefficient, int nStochasts);

        /**
         * \brief Gets the number of runs which is needed to achieve the variation coefficient at the probability for convergence
         */
        int getRequiredSamples(int nStochasts) const;

        /**
         * \brief Modifies the variation coefficient so that the number of required samples matches a given value
         */
        void setRequiredSamples(int nDirections, int nStochasts);

        /**
         * \brief Initial assumed model
         */
        ModelType modelType = ModelType::Plane;

        /**
         * \brief Quantiles which should be calculated
         */
        std::vector<std::shared_ptr<Statistics::ProbabilityValue>> RequestedQuantiles;

        /**
         * \brief Settings for performing model runs
         */
        std::shared_ptr<Models::RandomSettings> randomSettings = std::make_shared<Models::RandomSettings>();

        /**
         * \brief Settings for individual stochastic variables, such as the start value
         */
        std::shared_ptr<Reliability::StochastSettingsSet> StochastSet = std::make_shared<Reliability::StochastSettingsSet>();

        /**
         * \brief Settings for performing model runs
         */
        std::shared_ptr<Models::RunSettings> RunSettings = std::make_shared<Models::RunSettings>();

        /**
         * \brief Reports whether the settings have valid values
         * \param report Report in which the validity is reported
         */
        void validate(Logging::ValidationReport& report) const
        {
            Logging::ValidationSupport::checkNotEmpty(report, this->RequestedQuantiles.size(), "requested quantiles");

            RunSettings->validate(report);
        }

    private:
        static double getVariationCoefficient(double q, int nDirections, int nStochasts);
        double getMaxProbability() const;
    };
}

