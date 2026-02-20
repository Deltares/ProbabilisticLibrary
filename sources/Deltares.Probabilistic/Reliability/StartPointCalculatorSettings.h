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
#include <vector>
#include "StochastSettingsSet.h"
#include "../Logging/ValidationReport.h"


namespace Deltares::Reliability
{
    enum class StartMethodType { FixedValue, RaySearch, SensitivitySearch, SphereSearch };

    /**
     * \brief Settings for the start point calculator
     */
    class StartPointCalculatorSettings
    {
    public:
        /**
         * \brief Type of start point calculation
         */
        StartMethodType StartMethod = StartMethodType::FixedValue;
        double MaximumLengthStartPoint = 6;
        double GradientStepSize = 4;
        double RadiusSphereSearch = 10;
        double dsdu = 1;
        bool allQuadrants = false;
        int maxStepsSphereSearch = 5;

        std::shared_ptr<StartPointCalculatorSettings> clone() const;

        void validate(Logging::ValidationReport& report) const;

        std::shared_ptr<StochastSettingsSet> StochastSet = std::make_shared<StochastSettingsSet>();
        std::vector<double> startVector;

        static std::string getStartPointMethodString(StartMethodType method);
        static StartMethodType getStartPointMethod(const std::string& method);
    };
}
