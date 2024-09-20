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

#include "../Reliability/DesignPoint.h"
#include "../Statistics/SelfCorrelationMatrix.h"

namespace Deltares
{
    namespace Reliability
    {
        class LengthEffectProject
        {
        public:
            /**
             * \brief The design point for a section
             */
            std::shared_ptr<Deltares::Reliability::DesignPoint> designPointCrossSection;


            /**
             * \brief The breach length
             */
            double breachLength = -999.0;

            /**
             * \brief The length for the upscaling calculation
             */
            double length = 0.0;

            /**
             * \brief The correlation lengths for all stochasts
             */
            std::vector<double> correlationLengths = std::vector<double>();

            /**
             * \brief Self correlations as a simple vector
             */
            std::vector<double> self_correlations = std::vector<double>();

            /**
             * \brief Self correlations as SelfCorrelationMatrix
             */
            std::shared_ptr<Statistics::SelfCorrelationMatrix> selfCorrelationMatrix = std::make_shared<Statistics::SelfCorrelationMatrix>();

            /**
             * \brief Results of the length effect calculation
             */
            std::shared_ptr<Reliability::DesignPoint> designPoint = nullptr;

            /**
             * \brief Executes the length effect
             */
            void run();
        };
    }
}

