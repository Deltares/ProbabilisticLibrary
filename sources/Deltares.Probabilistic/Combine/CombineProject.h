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

#include "CombineSettings.h"
#include "../Reliability/DesignPoint.h"
#include "../Statistics/SelfCorrelationMatrix.h"

namespace Deltares
{
    namespace Reliability
    {
        class CombineProject
        {
        public:
            /**
             * \brief Collection of all design points to be combined
             */
            std::vector<std::shared_ptr<Deltares::Reliability::DesignPoint>> designPoints;

            /**
             * \brief Calculation settings
             */
            std::shared_ptr<CombineSettings> settings = std::make_shared<CombineSettings>();

            /**
             * \brief Self correlations
             */
            std::shared_ptr<Statistics::SelfCorrelationMatrix> selfCorrelationMatrix = std::make_shared<Statistics::SelfCorrelationMatrix>();

            /**
             * \brief Correlations on which the input design points were based
             */
            std::shared_ptr<Statistics::BaseCorrelation> correlationMatrix = std::make_shared<Statistics::CorrelationMatrix>();

            /**
             * \brief Results of the combination
             */
            std::shared_ptr<Reliability::DesignPoint> designPoint = nullptr;

            /**
             * \brief Reports whether these settings have valid values
             * \param report Report in which the validity is reported
             */
            void validate(Logging::ValidationReport& report) const;

            /**
             * \brief Validates the project and puts the result in a validation report
             * \returns Validation report
             */
            Logging::ValidationReport getValidationReport() const;

            /**
             * \brief Indicates whether the settings are valid
             * \returns Indication
             */
            bool is_valid() const;

            /**
             * \brief Executes the combination
             */
            void run();
        };
    }
}

