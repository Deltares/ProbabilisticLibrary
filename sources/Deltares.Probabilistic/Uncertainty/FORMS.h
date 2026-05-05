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
#include "FORMSettingsS.h"
#include "UncertaintyMethod.h"

namespace Deltares::Uncertainty
{
    /**
     * \brief Calculates the sensitivity using the FORM algorithm
     */
    class FORMS : public UncertaintyMethod
    {
    public:
        /**
         * \brief Settings for this algorithm
         */
        FORMSettingsS Settings = FORMSettingsS();

        /**
         * \brief Gets the sensitivity
         * \param modelRunner The model for which the sensitivity is calculated
         * \return The sensitivity in the form of a stochastic variable
         */
        UncertaintyResult getUncertaintyStochast(std::shared_ptr<Models::ModelRunner> modelRunner) override;
    private:
        static bool isZValid(Models::ModelRunner& modelRunner, double z);
        static bool isMonotone(Models::ModelRunner& modelRunner, double z, double zPrevious, bool ascending);
        static bool isGradientValid(Models::ModelRunner& modelRunner, std::vector<double>& gradient);
        static bool isBetaValid(Models::ModelRunner& modelRunner, double beta, double betaPrevious, double requiredBetaIncrement);
        static void repairResults(std::vector<double>& values);
        static bool areResultsValid(const std::vector<double>& values);
        void checkQuantiles(const Models::ModelRunner& modelRunner, const std::shared_ptr<Models::Sample>& startPoint, const std::shared_ptr<Models::Sample>& previousPoint, double factor);
        std::unordered_map<std::shared_ptr<Statistics::ProbabilityValue>, std::shared_ptr<Models::Evaluation>> evaluations;
    };
}

