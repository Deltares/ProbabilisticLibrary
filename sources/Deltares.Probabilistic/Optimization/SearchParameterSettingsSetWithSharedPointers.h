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
#include <memory>

#include "SearchParameterSettings.h"
#include "SearchParameterSettingsSet.h"

namespace Deltares::Optimization
{
    /**
     * Contains a collection of search parameters as shared pointers
     */
    class SearchParameterSettingsSetWithSharedPointers
    {
    public:
        /**
         * Collection of SearchParameterSettings as shared pointers
         */
        std::vector<std::shared_ptr<SearchParameterSettings>> Dimensions;

        /**
         * \brief Sets the number of expected search parameter settings, so that appending a SearchParameterSettings is done efficiently
         * \param nDimensions the number of search parameter settings
         */
        void setDimensions(int nDimensions)
        {
            for (int i = static_cast<int>(Dimensions.size()); i < nDimensions; i++)
            {
                Dimensions.push_back(std::make_shared<SearchParameterSettings>());
            }
        }

        /**
         * \brief Fills the SearchParameterSettings in a SearchParameterSettingsSet (without shared pointers)
         * \param searchParameterSettingsSet The search parameter settings set to be populated
         */
        void fillSearchParameterSettings(SearchParameterSettingsSet& searchParameterSettingsSet) const
        {
            searchParameterSettingsSet.Dimensions.clear();
            for (const auto& searchParameterSettings : Dimensions)
            {
                searchParameterSettingsSet.Dimensions.push_back(*searchParameterSettings.get());
            }
        }
    };
}

