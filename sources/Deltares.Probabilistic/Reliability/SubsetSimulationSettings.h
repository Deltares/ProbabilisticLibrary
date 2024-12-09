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

#include "DesignPointBuilder.h"
#include "StochastSettingsSet.h"
#include "../Model/RandomSettings.h"
#include "../Model/RunSettings.h"

namespace Deltares
{
    namespace Reliability
    {
        enum SampleMethodType { MarkovChain, AdaptiveConditional };

        /**
         * \brief Settings for the subset simulation algorithm
         */
        class SubsetSimulationSettings
        {
        public:
            /**
             * \brief The minimum samples to be examined
             */
            int MinimumSamples = 1000;

            /**
             * \brief The maximum samples to be examined
             */
            int MaximumSamples = 10000;

            /**
             * \brief The importance sampling algorithm stops when the calculated variation coefficient is less than this value
             */
            double VariationCoefficient = 0.05;

            /**
             * \brief Method type how the design point (alpha values) is calculated
             */
            DesignPointMethod designPointMethod = DesignPointMethod::CenterOfGravity;

            /**
             * \brief Defines the way new samples are generated
             */
            SampleMethodType SampleMethod = SampleMethodType::MarkovChain;

            /**
             * \brief Standard deviation in the Markov chain
             */
            double MarkovChainDeviation = 1;

            /**
             * \brief Fraction of the samples which will be used in the next iteration
             */
            double SubsetFraction = 0.1;

            /**
             * \brief Settings for generating random values
             */
            std::shared_ptr<Deltares::Models::RandomSettings> randomSettings = std::make_shared<Deltares::Models::RandomSettings>();

            /**
             * \brief Settings for performing model runs
             */
            std::shared_ptr<Models::RunSettings> RunSettings = std::make_shared<Models::RunSettings>();

            /**
             * \brief Settings for individual stochastic variables, such as the start value
             */
            std::shared_ptr<StochastSettingsSet> StochastSet = std::make_shared<StochastSettingsSet>();

            /**
             * \brief Indicates whether the settings have valid values
             * \return Indication
             */
            bool isValid()
            {
                return MinimumSamples >= 1 &&
                    MaximumSamples >= MinimumSamples &&
                    RunSettings->isValid();
            }

            static std::string getSampleMethodString(SampleMethodType method);
            static SampleMethodType getSampleMethod(std::string method);
        };
    }
}

