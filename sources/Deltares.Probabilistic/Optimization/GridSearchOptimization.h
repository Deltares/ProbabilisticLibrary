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

#include "GridSearchSettings.h"
#include "OptimizationMethod.h"
#include "OptimizationResult.h"
#include "SearchParameterSettingsSet.h"
#include "../Model/ModelSample.h"

namespace Deltares::Optimization
{
    class GridSearchOptimization : public OptimizationMethod
    {
    public:
        /**
         * \brief Settings
         */
       GridSearchSettings Settings;

        /**
         * \brief Finds the parameter combination which results in the minimum value
         * \param model Model to invoke, the minimum z-value will be used
         * \return Sample containing values which lead to the minimum value
         */
        OptimizationResult getOptimizedSample(Models::ZModel& model) override;

    private:
        /**
         * \brief Finds the parameter combination which results in the minimum value
         * \param searchArea Definition of parameter space and settings which will be searched
         * \param model Model to invoke, the minimum z-value will be used
         * \param minSample The minimum sample found from previous iterations (nullptr if initial)
         * \return Sample containing values which lead to the minimum value (if no sample leading to a lower value is found, minSample will be returned)
         */
        Models::ModelSample findGridExtreme(const SearchParameterSettingsSet& searchArea, Models::ZModel& model, Models::ModelSample& minSample);

        /**
         * \brief Indicates whether a sample is located on the edge of the search area
         * \param searchArea Definition of parameter space and settings which will be searched
         * \param sample Sample
         * \return Indication
         */
        static bool isSampleOnEdge(const SearchParameterSettingsSet& searchArea, const Models::ModelSample& sample);

        /**
         * \brief Moves the search area in such a way that the sample is not on the edge any more
         * \param searchArea Definition of parameter space and settings which will be searched
         * \param sample Sample on edge
         */
        static void moveSampleToCenter(SearchParameterSettingsSet& searchArea, const Models::ModelSample& sample);

        /**
         * \brief Indicates whether refinement is possible
         * \param searchArea Definition of parameter space and settings which will be searched
         * \param refinements Number of refinements performed so far
         * \return Indication
         */
        static bool canRefine(const SearchParameterSettingsSet& searchArea, int refinements);

        /**
         * \brief Refines the grid around the currently found minimum sample
         * \param searchArea Definition of parameter space and settings which will be searched
         * \param refinements Number of refinements performed so far
         * \param sample Minimum sample
         */
        static void refineGrid(SearchParameterSettingsSet& searchArea, int refinements, const Models::ModelSample& sample);

        /**
         * \brief Gets the tolerance for a parameter when determining whether a value is on the edge of a grid 
         * \param dimension Settings of the parameter
         * \return Tolerance
         */
        static double getTolerance(const SearchParameterSettings& dimension);

    private:

        int counter = 0;
        int reusedCounter = 0;

    };
}
