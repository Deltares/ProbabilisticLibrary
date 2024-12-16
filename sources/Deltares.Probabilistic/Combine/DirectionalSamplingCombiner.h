// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of Streams.
//
// Streams is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// All names, logos, and references to "Deltares" are registered trademarks of
// Stichting Deltares and remain full property of Stichting Deltares at all times.
// All rights reserved.
//
#pragma once
#include <vector>
#include <memory>

#include "CombinedDesignPointModel.h"
#include "combiner.h"
#include "../Statistics/CorrelationMatrix.h"
#include "../Reliability/DesignPoint.h"
#include "../Reliability/DirectionalSamplingSettings.h"
#include "../Model/ProgressIndicator.h"

namespace Deltares
{
    namespace Reliability
    {
        /**
         * \brief Combines design points with directional sampling algorithm
         */
        class DirectionalSamplingCombiner : public Combiner
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
            std::shared_ptr<DesignPoint> combineDesignPoints(combineAndOr combineMethodType, std::vector<std::shared_ptr<DesignPoint>>& designPoints, std::shared_ptr<Statistics::SelfCorrelationMatrix> selfCorrelationMatrix = nullptr, std::shared_ptr<ProgressIndicator> progress = nullptr) override;

            /**
             * \brief The random number generator to be used
             */
            Deltares::Numeric::RandomValueGeneratorType randomGeneratorType = Deltares::Numeric::RandomValueGeneratorType::ModifiedKnuthSubtractive;
        private:
            void fillSettings(std::shared_ptr<CombinedDesignPointModel> model, std::shared_ptr<DirectionalSamplingSettings> settings);
        };
    }
}

