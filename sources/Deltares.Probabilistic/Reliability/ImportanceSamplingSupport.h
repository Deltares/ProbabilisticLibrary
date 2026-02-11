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
#include "ImportanceSamplingCluster.h"
#include "ImportanceSamplingSettings.h"
#include "ReliabilityMethod.h"
#include "../Model/Sample.h"

namespace Deltares::Reliability
{
    typedef std::function<void(std::shared_ptr<Models::Sample>)> RegisterSampleLambda;
    typedef std::function<bool(std::shared_ptr<ImportanceSamplingCluster>)> BreakLoopLambda;

    /**
     * \brief Supporting class for reliability and uncertainty importance sampling
     */
    class ImportanceSamplingSupport
    {
    public:
        static double getConvergence(double pf, double minWeight, int samples);
        static double getDimensionality(const std::vector<double>& factors);
        static double getSampleWeight(const std::shared_ptr<Models::Sample>& sample, const std::shared_ptr<Models::Sample>& center, double dimensionality, const std::vector<double>& factors);

        /**
         * \brief Gets the weight for the sample
         * \param modifiedSample the scaled sample
         * \param sample the original sample
         * \param dimensionality constant dependent on the number of stochasts
         * \return weight
         */
        static double getWeight(const std::shared_ptr<Models::Sample>& modifiedSample, const std::shared_ptr<Models::Sample>& sample, double dimensionality);
        static std::shared_ptr<Models::Sample> getOriginalSample(const std::shared_ptr<Models::Sample>& sample, const std::shared_ptr<Models::Sample>& center, const std::vector<double>& factors);
    };
}

