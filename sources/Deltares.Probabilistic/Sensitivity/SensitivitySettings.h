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

#include "SensitivityMethod.h"
#include "SingleVariation.h"
#include "Sobol.h"
#include "../Model/RunSettings.h"

namespace Deltares::Sensitivity
{
    enum SensitivityMethodType { SensitivitySingleVariation, SensitivitySobol };

    /**
     * \brief General settings applicable to all mechanisms
     */
    class SensitivitySettings
    {
    public:
        /**
         * \brief Method type how the design point (alpha values) is calculated
         */
        SensitivityMethodType SensitivityMethod = SensitivityMethodType::SensitivitySobol;

        /**
         * \brief Low value fraction
         */
        double LowValue = 0.05;

        /**
         * \brief High value fraction
         */
        double HighValue = 0.95;

        /**
         * \brief The number of iterations
         */
        int Iterations = 1000;

        /**
         * \brief Settings for performing model runs
         */
        std::shared_ptr<Models::RunSettings> RunSettings = std::make_shared<Models::RunSettings>();

        /**
         * \brief Gets the reliability method and settings based on these settings
         */
        std::shared_ptr<Sensitivity::SensitivityMethod> GetSensitivityMethod();

        /**
         * \brief Indicates whether these settings have valid values
         * \return Indication
         */
        bool isValid() const;

        static std::string getSensitivityMethodTypeString(SensitivityMethodType method);
        static SensitivityMethodType getSensitivityMethodType(std::string method);
    private:
        std::shared_ptr<Sobol> GetSobolMethod() const;
        std::shared_ptr<SingleVariation> GetSingleVariationMethod() const;
    };
}

