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

#include "ReliabilityMethod.h"
#include "DesignPoint.h"
#include "Settings.h"
#include "LimitStateFunction.h"
#include "../Model/ModelProject.h"
#include "../Statistics/Stochast.h"

namespace Deltares
{
    namespace Reliability
    {
        /**
         * \brief Combines a model, stochastic variables and calculation settings, can perform a calculation and holds results
         */
        class ReliabilityProject : public Models::ModelProject
        {
        public:
            /**
             * \brief Method which performs a reliability calculation
             */
            std::shared_ptr<Reliability::ReliabilityMethod> reliabilityMethod = nullptr;

            /**
             * \brief Limit state function
             */
            std::shared_ptr<Reliability::LimitStateFunction> limitStateFunction = std::make_shared<Reliability::LimitStateFunction>();

            /**
             * \brief Calculation settings
             */
            std::shared_ptr<Reliability::Settings> settings = std::make_shared<Reliability::Settings>();

            /**
             * \brief Settings for performing a calculation
             * \remark Settings of the reliability calculation are held in the settings of the reliability method
             */
            std::shared_ptr<Models::RunSettings> runSettings = std::make_shared<Models::RunSettings>();

            /**
             * \brief Callback for progress during the calculation
             */
            std::shared_ptr<Models::ProgressIndicator> progressIndicator = nullptr;

            /**
             * \brief Results of the reliability calculation
             */
            std::shared_ptr<Reliability::DesignPoint> designPoint = nullptr;

            /**
             * \brief Results of the reliability calculation
             */
            std::shared_ptr<Statistics::Stochast> fragilityCurve = nullptr;

            /**
             * \brief Reports whether these settings have valid values
             * \param report Report in which the validity is reported
             */
            void validate(Logging::ValidationReport& report) override;

            /**
             * \brief Performs the reliability calculation
             * \return Design point
             */
            std::shared_ptr<Reliability::DesignPoint> getDesignPoint();

            /**
             * \brief Performs the reliability calculation
             * \return Design point
             */
            std::shared_ptr<Statistics::Stochast> getFragilityCurve();

            /**
             * \brief Runs the reliability calculation
             */
            void run() override;

            /**
             * \brief Sets the settings
             */
            void setSettings(std::shared_ptr<Models::ModelProjectSettings> newSettings) override
            {
                settings = static_pointer_cast<Settings>(newSettings);
            }
        };
    }
}

