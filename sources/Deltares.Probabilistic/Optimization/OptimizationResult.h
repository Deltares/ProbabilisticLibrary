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

#include "../Model/ModelSample.h"
#include "../Model/Evaluation.h"
#include "../Logging/Message.h"
#include <cmath>

namespace Deltares::Optimization
{
    /**
     * \brief Contains the results of an optimization calculation
     */
    class OptimizationResult
    {
    public:
        /**
         * \brief Indicates whether the optimization has succeeded
         */
        bool succeeded = false;

        /**
         * \brief Minimum found model value
         */
        double minimumValue = std::nan("");

        /**
         * \brief Number of model runs made to achieve the result
         */
        int modelRuns = 0;

        /**
         * \brief Values corresponding with the minimum result
         */
        std::vector<double> values;

        /**
         * \brief List of evaluations calculated during optimization analysis
         */
        std::vector<std::shared_ptr<Models::Evaluation>> evaluations;

        /**
         * \brief List of messages raised during optimization analysis
         */
        std::vector<std::shared_ptr<Logging::Message>> messages;
    };
}

