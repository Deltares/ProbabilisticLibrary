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

#include "UncertaintyMethod.h"
#include "../Model/RandomSettings.h"
#include "../Model/GradientSettings.h"
#include "../Model/ModelProjectSettings.h"

namespace Deltares::Uncertainty
{
    enum UncertaintyMethodType { UncertaintyFORM, UncertaintyFOSM, UncertaintyNumericalIntegration, UncertaintyCrudeMonteCarlo, UncertaintyImportanceSampling, UncertaintyDirectionalSampling };

    /**
     * \brief General settings applicable to all sensitivity mechanisms
     */
    class SettingsS : public Models::ModelProjectSettings
    {
    public:
        /**
         * \brief Method type how the design point (alpha values) is calculated
         */
        UncertaintyMethodType UncertaintyMethod = UncertaintyMethodType::UncertaintyCrudeMonteCarlo;

        /**
         * \brief Indicates whether correlations should be calculated
         */
        bool CalculateCorrelations = false;

        /**
         * \brief Indicates whether correlations with input variables should be calculated
         */
        bool CalculateInputCorrelations = false;

        /**
         * \brief The minimum u value
         */
        double MinimumU = -Statistics::StandardNormal::UMax;

        /**
         * \brief The maximum u value
         */
        double MaximumU = Statistics::StandardNormal::UMax;

        /**
         * \brief The minimum samples to be examined
         */
        int MinimumSamples = 1000;

        /**
         * \brief The maximum samples to be examined
         */
        int MaximumSamples = 10000;

        /**
         * \brief The minimum directions to be examined
         */
        int MinimumDirections = 100;

        /**
         * \brief The maximum directions to be examined
         */
        int MaximumDirections = 1000;

        /**
         * \brief The importance sampling algorithm stops when the calculated variation coefficient is less than this value
         */
        double VariationCoefficient = 0.05;

        /**
         * \brief The probability which is used to check whether the calculation has converged
         */
        double ProbabilityForConvergence = 0.05;

        /**
         * \brief Indicates whether the number of samples should be derived from the variation coefficient at the probability for convergence
         */
        bool DeriveSamplesFromVariationCoefficient = false;

        /**
         * \brief Maximum number of guessed design points in one FORM loop
         */
        int MaximumIterations = 50;

        /**
         * \brief Step size in form calculation
         */
        double GlobalStepSize = 0.5;

        /**
         * \brief Requested quantiles
         */
        std::vector<std::shared_ptr<Statistics::ProbabilityValue>> RequestedQuantiles;

        /**
         * \brief Settings for generating random values
         */
        std::shared_ptr<Deltares::Models::RandomSettings> RandomSettings = std::make_shared<Deltares::Models::RandomSettings>();

        /**
         * \brief Settings for calculating the gradient at a stochast point
         */
        std::shared_ptr<Models::GradientSettings> GradientSettings = std::make_shared<Models::GradientSettings>();

        /**
         * \brief Settings for individual stochastic variable, such as the start value
         */
        std::shared_ptr<Reliability::StochastSettingsSet> StochastSet = std::make_shared<Reliability::StochastSettingsSet>();

        /**
         * \brief Gets the reliability method and settings based on these settings
         */
        std::shared_ptr<Uncertainty::UncertaintyMethod> GetUncertaintyMethod();

        /**
         * \brief Reports whether these settings have valid values
         * \param report Report in which the validity is reported
         */
        void validate(Logging::ValidationReport& report);
            
        static std::string getUncertaintyMethodTypeString(UncertaintyMethodType method);
        static UncertaintyMethodType getUncertaintyMethodType(std::string method);
    private:
        const std::shared_ptr<Uncertainty::UncertaintyMethod> GetFORMMethod();
        const std::shared_ptr<Uncertainty::UncertaintyMethod> GetFOSMMethod();
        const std::shared_ptr<Uncertainty::UncertaintyMethod> GetNumericalIntegrationMethod();
        const std::shared_ptr<Uncertainty::UncertaintyMethod> GetCrudeMonteCarloMethod();
        const std::shared_ptr<Uncertainty::UncertaintyMethod> GetImportanceSamplingMethod();
        const std::shared_ptr<Uncertainty::UncertaintyMethod> GetDirectionalSamplingMethod();
    };
}

