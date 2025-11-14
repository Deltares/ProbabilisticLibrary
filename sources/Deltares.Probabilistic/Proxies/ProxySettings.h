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

#include "../Model/GradientSettings.h"
#include "../Model/RunSettings.h"
#include "../Model/Validatable.h"
#include "../Model/ModelSample.h"

namespace Deltares::Proxies
{
    enum class ProxyInitializationType
    {
        Single,
        SingleAndFullFactorial,
        Double,
        ExperimentalDesign,
        FullFactorial,
        Random
    };

    enum class ProxyMethodType
    {
        FirstOrder,
        SecondOrder,
        LinearGrid,
        GPR
    };

    /**
     * \brief Settings for FORM algorithm
     */
    class ProxySettings : public Models::Validatable
    {
    public:
        /**
         * \brief Relaxation factor, which is applied when generating the guessed design point for a new iteration
         */
        double  VarianceFactor = 1.0;

        /**
         * \brief Maximum number of guessed design points in one FORM loop
         */
        bool Disturbed = true;

        /**
         * \brief Maximum number of FORM loops when no convergence is reached
         */
        int NumberOfRuns = 10;

        /**
         * \brief Increase factor of Maximum iterations when a new FORM loop is applied
         */
        int Seed = 0;

        /**
         * \brief The FORM loop stops when the next guessed design point is less than this value away from the current guessed design point
         */
        ProxyInitializationType InitializationType = ProxyInitializationType::Double;

        /**
         * \brief If true, makes a guess for the design point if the final FORM loop does not converge
         */
        ProxyMethodType MethodType = ProxyMethodType::FirstOrder;

        /**
         * \brief Settings for performing model runs
         */
        std::vector<std::shared_ptr<Models::ModelSample>> TrainingSamples;

        void validate(Logging::ValidationReport& report) const override
        {
            Logging::ValidationSupport::checkMinimumInt(report, 1, NumberOfRuns, "number of runs");
            Logging::ValidationSupport::checkMinimum(report, 0.01, VarianceFactor, "variance factor");
        }
    };
}

