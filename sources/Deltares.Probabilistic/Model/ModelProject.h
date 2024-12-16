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
#include "../Statistics/Stochast.h"
#include "../Statistics/CorrelationMatrix.h"
#include "ZModel.h"

namespace Deltares
{
    namespace Models
    {
        /**
         * \brief Base class for projects containing a model
         */
        class ModelProject
        {
        public:
            /**
             * \brief Collection of all stochastic variables
             */
            std::vector<std::shared_ptr<Statistics::Stochast>> stochasts;

            /**
             * \brief Defines correlations between stochastic variables
             */
            std::shared_ptr<Statistics::CorrelationMatrix> correlationMatrix = nullptr;

            /**
             * \brief Deterministic model which calculates a z-value based on input values
             */
            std::shared_ptr<ZModel> model = nullptr;

            /**
             * \brief Updates the stochasts with the parameters of the model
             */
            void updateStochasts();
        private:
            std::unordered_map<std::string, std::shared_ptr<Statistics::Stochast>> existingStochasts;
        };
    }
}

