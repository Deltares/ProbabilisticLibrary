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
#include <memory>

#include "combiner.h"
#include "../Statistics/Scenario.h"

namespace Deltares
{
    namespace Reliability
    {
        /**
         * \brief Combines design points which have no overlap
         */
        class ExcludingCombiner : public Combiner
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
            std::shared_ptr<DesignPoint> combineDesignPoints(combineAndOr combineMethodType, std::vector<std::shared_ptr<DesignPoint>>& designPoints, std::shared_ptr<Statistics::SelfCorrelationMatrix> selfCorrelationMatrix = nullptr, std::shared_ptr<ProgressIndicator> progress = nullptr) override
            {
                throw probLibException("combineDesignPoints with scenarios should be called");
            }

            /**
             * \brief Combines a number of design points
             * \param scenarios Scenarios
             * \param designPoints Design points to be combined
             * \return Design point resembling the combined reliability and alpha values
             */
            std::unique_ptr<DesignPoint> combineDesignPoints(
                std::vector<std::shared_ptr<Statistics::Scenario>>& scenarios,
                std::vector<std::shared_ptr<Reliability::DesignPoint>>& designPoints);
        };
    }
} 


