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

#include "SearchParameterSettings.h"
#include "../Logging/ValidationReport.h"

namespace Deltares::Optimization
{
    class SearchParameterSettingsSet
    {
    public:
        std::vector<SearchParameterSettings> Dimensions;

        void setDimensions(size_t nDimensions)
        {
            for (size_t i = Dimensions.size(); i < nDimensions; i++)
            {
                Dimensions.push_back(SearchParameterSettings());
            }
        }

        /**
         * \brief Gets the maximum  number of refinements in any dimension
         * \return Maximum number of refinements
         */
        int getMaxRefinements() const;

        /**
         * \brief Reports whether the settings have valid values
         * \param report Report in which the validity is reported
         */
        void validate(Logging::ValidationReport& report) const;
    };
}

