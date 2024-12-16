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
#include "../Reliability/ImportanceSamplingSettings.h"
#include "../Model/ProgressIndicator.h"

namespace Deltares
{
    namespace Reliability
    {
        /**
         * \brief Combines design points with importance sampling algorithm
         */
        class ImportanceSamplingCombiner : public Combiner
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
            /**
             * \brief Approximates the probability by assuming design points are independent
             * \param combineMethodType Series or parallel combination type
             * \param designPoints Design points to be combined
             * \return Approximated probability of failure
             */
            double getApproximatedProbability(combineAndOr combineMethodType, std::vector<std::shared_ptr<DesignPoint>>& designPoints);

            /**
             * \brief Inverts the design point, so the probability of failure will be the probability of non-failure of the non-inverted design point
             * \param designPoint Design point to be inverted
             */
            void invert(std::shared_ptr<DesignPoint>& designPoint);

            /**
             * \brief Combines design points with possibly inverted design points
             * \param combineMethodType Series (or-combination) or Parallel (and-combination) combination type
             * \param designPoints Design points to be combined
             * \param selfCorrelationMatrix Administration of correlations between different design points
             * \param progress Progress indicator (optional)
             * \return Design point resembling the combined reliability and alpha values
             */
            std::shared_ptr<DesignPoint> combineDesignPointsAdjusted(combineAndOr combineMethodType, std::shared_ptr<Statistics::SelfCorrelationMatrix> selfCorrelationMatrix, std::shared_ptr<ProgressIndicator> progress, std::vector<std::shared_ptr<DesignPoint>>& designPoints);

            /**
             * \brief Calculates the probability that a sample does not fail for previous design points when it fails to the current design point
             * \param selfCorrelationMatrix Administration of correlations between different design points
             * \param currentDesignPoint Current design point
             * \param previousDesignPoints Previous design point
             * \param startPoint Starting point of the calculation
             * \param stochasts All stochasts 
             * \param progress Progress indicator (optional)
             * \return Probability
             */
            std::shared_ptr<DesignPoint> getSeriesProbability(std::shared_ptr<Statistics::SelfCorrelationMatrix> selfCorrelationMatrix, std::shared_ptr<DesignPoint> currentDesignPoint, std::vector<std::shared_ptr<DesignPoint>>& previousDesignPoints, std::shared_ptr<DesignPoint> startPoint, std::vector<std::shared_ptr<Statistics::Stochast>>& stochasts, std::shared_ptr<ProgressIndicator> progress);

            /**
             * \brief Calculates the design point when combination type is parallel
             * \param selfCorrelationMatrix Administration of correlations between different design points
             * \param previousDesignPoints Previous design point
             * \param stochasts All stochasts
             * \param progress Progress indicator (optional)
             * \return Design point
             */
            std::shared_ptr<DesignPoint> getParallelProbability(std::shared_ptr<Statistics::SelfCorrelationMatrix> selfCorrelationMatrix, std::vector<std::shared_ptr<DesignPoint>>& previousDesignPoints, std::vector<std::shared_ptr<Statistics::Stochast>>& stochasts, std::shared_ptr<ProgressIndicator> progress);

            /**
             * \brief Fills the settings for series algorithm
             * \param startPoint Starting point
             * \param model Linearized model
             * \param settings Settings to be filled
             */
            void fillSettingsSeries(std::shared_ptr<DesignPoint> startPoint, std::shared_ptr<CombinedDesignPointModel> model, std::shared_ptr<ImportanceSamplingSettings> settings);

            /**
             * \brief Fills the settings for parallel algorithm
             * \param model Linearized model
             * \param settings Settings to be filled
             */
            void fillSettingsParallel(std::shared_ptr<CombinedDesignPointModel> model, std::shared_ptr<ImportanceSamplingSettings> settings, double factor);

        };
    }
}

