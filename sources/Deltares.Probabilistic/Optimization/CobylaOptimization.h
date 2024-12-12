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
#include <functional>
#include "CobylaOptimizationSettings.h"
#include "../Model/Sample.h"

namespace Deltares::Optimization
{
    class SearchDimension
    {
    public:
        double StartValue = 0.0;
        double LowerBound = -1e30;
        double UpperBound = 1e30;
    };

    class SearchArea
    {
    public:
        std::vector<SearchDimension> Dimensions;
    };

    class OptimizationSample
    {
    public:
        std::vector<double> Input;
        std::vector<double> Deviations;
        int numberOfSamples;
        double minimumValue;
        bool success;
    };

    class optimizationModel
    {
    public:
        virtual double GetZValue(const std::shared_ptr<Models::Sample> sample) const { return -1; };
        virtual double GetConstraintValue(const std::shared_ptr<Models::Sample> sample) const { return -1; };
        virtual unsigned GetNumberOfConstraints() const { return 0; };
    };

    /**
     * \brief Wrapper for Cobyla optimization algorithm
     * \note At this moment: this wrapper handles only zero or one constraints
     */
    class CobylaOptimization
    {
    public:
        CobylaOptimizationSettings settings;
        OptimizationSample GetCalibrationPoint(const SearchArea& searchArea, std::shared_ptr<optimizationModel> model) const;
    };
}

