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

#include "ExcludingCombineSettings.h"
#include "../Reliability/DesignPoint.h"

namespace Deltares::Reliability
{
    /**
     * \brief Project for combining excluding design points
     */
    class ExcludingCombineProject
    {
    public:
        /**
         * \brief Collection of all design points to be combined
         */
        std::vector<std::shared_ptr<Deltares::Reliability::DesignPoint>> designPoints;

        /**
         * \brief Collection of all scenarios. The number of scenarios and design points should be equal.
         * \remarks The first scenario applies to the first design point, the second scenario to the second design point, etc.
         */
        std::vector<std::shared_ptr<Deltares::Statistics::Scenario>> scenarios;

        /**
         * \brief Reports whether these settings have valid values
         * \param report Report in which the validity is reported
         */
        void validate(Logging::ValidationReport& report) const;

        /**
         * \brief Validates the project and puts the result in a validation report
         * \returns Validation report
         */
        Logging::ValidationReport getValidationReport();

        /**
         * \brief Indicates whether the settings are valid
         * \returns Indication
         */
        bool is_valid();

        /**
         * \brief Calculation settings
         */
        std::shared_ptr<ExcludingCombineSettings> settings = std::make_shared<ExcludingCombineSettings>();

        /**
         * \brief Results of the combination
         */
        std::shared_ptr<Reliability::DesignPoint> designPoint = nullptr;

        /**
         * \brief Executes the combination
         */
        void run();
    };
}

