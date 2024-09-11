// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of the Probabilistic Library.
//
// The Probabilistic Library is free software: you can redistribute it and/or modify
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

#include "ImportanceSamplingSettings.h"
#include "../Optimization/ClusterSettings.h"
#include "StartPointCalculatorSettings.h"
#include "StochastSettingsSet.h"

namespace Deltares
{
    namespace Reliability
    {
        /**
         * \brief Settings for adaptive importance sampling algorithm
         */
        class AdaptiveImportanceSamplingSettings
        {
        public:
            /**
             * \brief Minimum loops of importance sampling to perform
             */
            int MinVarianceLoops = 1;

            /**
             * \brief Maximum number of importance sampling to perform
             */
            int MaxVarianceLoops = 5;

            /**
             * \brief Indicates whether adaptive sampling is based on (true) clustering or (false) moving the center point based on the last importance sampling design point
             */
            bool Clustering = false;

            /**
             * \brief The minimum number of samples which should have failed (Z < 0) to allow the center point of the next importance sampling call to be based on the design point of the last importance sampling call. If not achieved, the next importance sampling call will have an increased variance factor
             */
            int MinimumFailedSamples = 0;

            /**
             * \brief The initial variance factor to be applied to all stochasts
             */
            double VarianceFactor = 1.5;

            /**
             * \brief Increment in variance factor of all stochasts when moving the design point is not allowed (see MinimumFailedSamples)
             */
            double LoopVarianceIncrement = 0.5;

            /**
             * \brief Indicates whether a moved center should be adjusted so that it is positioned on the limit state
             */
            bool StartPointOnLimitState = false;

            /**
             * \brief When the center point is moved, it will be a multiple of this value. This is used to avoid tiny modifications. If this value is 0, the new center point is equal to the design point of the previous importance sampling call.
             * \remark When the center point is not moved, there will be no new importance sampling call
             */
            double StartValueStepSize = 0;

            /**
             * \brief Indicates whether the number of samples to be performed in a non final importance sampling call is determined automatically
             */
            bool AutoMaximumSamples = false;

            /**
             * \brief When the relative weight (compared to the sum of weights of all other failing samples) of sample is less than this value, the importance sampling call is terminated and a new center point is determined. Only applicable if AutoMaximumSamplesNoResult is true.
             */
            double EpsWeightSample = 0.1;

            /**
             * \brief The calls to importance sampling will be stopped until the fraction of failed samples (Z < 0) is between this value and 1 - this value
             */
            double FractionFailed = 0.1;

            /**
             * \brief When the design point has a reliability index exceeding this value, the variance loops are stopped
             */
            double MaxBeta = Statistics::StandardNormal::BetaMax;

            /**
             * \brief Settings for the clustering algorithm
             */
            std::shared_ptr<Optimization::ClusterSettings> clusterSettings = std::make_shared<Optimization::ClusterSettings>();

            /**
             * \brief Settings how to derive the first center
             */
            std::shared_ptr<StartPointCalculatorSettings> startPointSettings = std::make_shared<StartPointCalculatorSettings>();

            /**
             * \brief Settings of the underlying importance sampling algorithm
             * \remark These settings will not be changed by adaptive importance sampling. A copy is made to apply modifications.
             */
            std::shared_ptr<ImportanceSamplingSettings> importanceSamplingSettings = std::make_shared<Deltares::Reliability::ImportanceSamplingSettings>();

            bool isValid()
            {
                return MinVarianceLoops >= 1 &&
                    MaxVarianceLoops >= MinVarianceLoops &&
                    StartValueStepSize >= 0 &&
                    FractionFailed >= 0 && FractionFailed <= 0.5 &&
                    EpsWeightSample >= 0.0001 && EpsWeightSample <= 1 &&
                    startPointSettings->isValid() &&
                    clusterSettings->isValid() &&
                    importanceSamplingSettings->isValid();
            }
        };
    }
}

