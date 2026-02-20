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
#include "../Reliability/DesignPoint.h"
#include "../Statistics/CorrelationMatrix.h"
#include "../Statistics/SelfCorrelationMatrix.h"
#include "../Model/ProgressIndicator.h"
#include "../Reliability/ReliabilityProject.h"
#include "CombinedDesignPointModel.h"
#include "CombineType.h"

namespace Deltares::Reliability
{
    /**
     * \brief Super class for all design point combination algorithms
     */
    class Combiner
    {
    public:
        /**
         * \brief Combines a number of design points
         * \param combineMethodType Series (or-combination) or Parallel (and-combination) combination type
         * \param designPoints Design points to be combined
         * \param selfCorrelationMatrix Administration of correlations between different design points
         * \param progress Progress indicator (optional)
         * \return Design point resembling the combined reliability and alpha values
         */
        virtual std::shared_ptr<DesignPoint> combineDesignPoints(combineAndOr combineMethodType,
            std::vector<std::shared_ptr<DesignPoint>>& designPoints,
            const std::shared_ptr<Statistics::SelfCorrelationMatrix>& selfCorrelationMatrix,
            const std::shared_ptr<Models::ProgressIndicator>& progress) = 0;

    protected:
        /**
         * \brief Gets the linearized model representing the design points to be combined
         * \param combineMethodType Combination type
         * \param currentDesignPoint Current design point
         * \param previousDesignPoints Previous design points
         * \param stochasts All stochasts
         * \param selfCorrelationMatrix Administration of correlations between different design points
         * \return Linearized model
         */
        static std::shared_ptr<CombinedDesignPointModel> getModel(combineAndOr combineMethodType,
            const std::shared_ptr<DesignPoint>& currentDesignPoint,
            const std::vector<std::shared_ptr<DesignPoint>>& previousDesignPoints,
            const std::vector<std::shared_ptr<Statistics::Stochast>>& stochasts,
            const std::shared_ptr<Statistics::SelfCorrelationMatrix>& selfCorrelationMatrix);

        /**
         * \brief Gets a project containing the linearized model
         * \param model Linearized model
         * \param selfCorrelationMatrix Self correlation administration
         * \return Project
         */
        static std::shared_ptr<ReliabilityProject> getProject(const std::shared_ptr<CombinedDesignPointModel>& model,
            const std::shared_ptr<Statistics::SelfCorrelationMatrix>& selfCorrelationMatrix);

    };
}
