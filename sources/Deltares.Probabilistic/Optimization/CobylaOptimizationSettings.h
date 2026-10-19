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
#include "SearchParameterSettingsSet.h"
#include "../Model/Validatable.h"

namespace Deltares::Optimization
{
    class CobylaOptimizationSettings : public Models::Validatable
    {
    public:
        int MaxIterations = 10000;

        double EpsilonBeta = 0.001;

        /**
         * \brief Settings for individual parameters, such as the start value
         */
        SearchParameterSettingsSet SearchArea = SearchParameterSettingsSet();

        /**
         * \brief Settings for performing model runs
         */
        Models::RunSettings RunSettings = Models::RunSettings();

        /**
         * \brief Reports whether the settings have valid values
         * \param report Report in which the validity is reported
         */
        void validate(Logging::ValidationReport& report) const override;
    };
}

