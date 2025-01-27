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

#include "../Utils/probLibException.h"
#include "../Reliability/DesignPoint.h"
#include "../Statistics/Scenario.h"
#include "combiner.h"
#include "DirectionalSamplingCombiner.h"
#include "ExcludingCombiner.h"
#include "HohenbichlerNumIntCombiner.h"
#include "ImportanceSamplingCombiner.h"

namespace Deltares
{
    namespace Reliability
    {
        enum CombinerType {Hohenbichler, ImportanceSampling, DirectionalSampling};

        /**
         * \brief Combines design points
         */
        class DesignPointCombiner
        {
        public:
            DesignPointCombiner() {}
            DesignPointCombiner(CombinerType combinerType)
            {
                this->combinerType = combinerType;
            }
            DesignPointCombiner(CombinerType combinerType, Numeric::RandomValueGeneratorType randomGenerator)
            {
                this->combinerType = combinerType;
                this->generator = randomGenerator;
            }

            /**
             * \brief Specifies the combiner algorithm
             */
            CombinerType combinerType = CombinerType::ImportanceSampling;

            /**
             * \brief Combines a number of design points
             * \param combineMethodType Identifies series (or) or parallel (and) combination
             * \param designPoints Design points to be combined
             * \param selfCorrelationMatrix Defines auto correlations (optional)
             * \param progress Progress indicator (optional)
             * \return Combined design point
             */
            std::shared_ptr<DesignPoint> combineDesignPoints(combineAndOr combineMethodType, std::vector<std::shared_ptr<DesignPoint>>& designPoints, std::shared_ptr<Statistics::SelfCorrelationMatrix> selfCorrelationMatrix = nullptr, std::shared_ptr<ProgressIndicator> progress = nullptr)
            {
                const std::shared_ptr<Combiner> combiner = getCombiner();
                return combiner->combineDesignPoints(combineMethodType, designPoints, selfCorrelationMatrix, progress);
            }

            std::shared_ptr<DesignPoint> combineDesignPointsExcluding(std::vector<std::shared_ptr<Statistics::Scenario>>& scenarios, std::vector<std::shared_ptr<DesignPoint>>& designPoints, std::shared_ptr<Statistics::SelfCorrelationMatrix> selfCorrelationMatrix = nullptr, std::shared_ptr<ProgressIndicator> progress = nullptr)
            {
                std::unique_ptr<ExcludingCombiner> combiner = std::make_unique<ExcludingCombiner>();
                return combiner->combineDesignPoints(scenarios, designPoints, selfCorrelationMatrix);
            }

            static std::string getCombineTypeString(combineAndOr type);
            static combineAndOr getCombineType(std::string method);
            static std::string getCombinerMethodString(CombinerType type);
            static CombinerType getCombinerMethod(std::string method);

        private:
            std::shared_ptr<Combiner> getCombiner()
            {
                switch (combinerType)
                {
                case CombinerType::ImportanceSampling:
                {
                    auto impSamplingCombiner = std::make_shared<ImportanceSamplingCombiner>();
                    impSamplingCombiner->randomGeneratorType = generator;
                    return impSamplingCombiner;
                }
                case CombinerType::Hohenbichler:
                    return std::make_shared<HohenbichlerNumIntCombiner>();
                case CombinerType::DirectionalSampling:
                {
                    auto directionalSamplingCombiner = std::make_shared<DirectionalSamplingCombiner>();
                    directionalSamplingCombiner->randomGeneratorType = generator;
                    return directionalSamplingCombiner;
                }
                default: throw probLibException("Combiner type");
                }
            }
            Numeric::RandomValueGeneratorType generator = Numeric::RandomValueGeneratorType::MersenneTwister;
        };
    }
}

