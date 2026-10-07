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

#include "OptimizationMethod.h"
#include "GridSearch.h"
#include "CobylaOptimization.h"
#include "AdaptiveParticleSwarmOptimization.h"
#include "../Model/ModelProjectSettings.h"


namespace Deltares::Optimization
{
    enum class OptimizationMethodType { GridSearch, Cobyla, APSO };

    /**
     * \brief General settings applicable to all optimization mechanisms
     */
    class OptimizationSettings : public Models::ModelProjectSettings
    {
    public:
        OptimizationSettings() = default;

        virtual ~OptimizationSettings() = default;

        /**
         * \brief Method type how the design point (alpha values) is calculated
         */
        OptimizationMethodType OptimizationMethod = OptimizationMethodType::GridSearch;

        /**
         * \brief Maximum number of grid moves to be performed
         */
        int MaxGridMoves = 50;

        /**
         * \brief The number of iterations
         */
        int Iterations = 1000;


        double EpsilonBeta = 0.001;

        /**
         * \brief Settings for individual parameters, such as the start value
         */
        std::shared_ptr<SearchParameterSettingsSet> SearchArea = std::make_shared<SearchParameterSettingsSet>();

        /**
         * \brief Gets the optimization method and settings based on these settings
         */
        std::shared_ptr<Optimization::OptimizationMethod> GetOptimizationMethod();

        /**
         * \brief Reports whether the settings have valid values
         * \param report Report in which the validity is reported
         */
        void validate(Logging::ValidationReport& report) const override;

        static std::string getOptimizationMethodTypeString(OptimizationMethodType method);
        static OptimizationMethodType getOptimizationMethodType(std::string method);
    private:
        std::shared_ptr<GridSearch> GetGridSearchMethod() const;
        std::shared_ptr<CobylaOptimization> GetCobylaMethod() const;
        std::shared_ptr<AdaptiveParticleSwarmOptimization> GetApsoMethod() const;
    };
}

